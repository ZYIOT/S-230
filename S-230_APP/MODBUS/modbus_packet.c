#include "modbus_packet.h"

static int left_shift_buffer(uint8_t *buf, size_t len)
{
  for (size_t i = 1; i < len; i++)
  {
    buf[i - 1] = buf[i];
  }
  return MODBUS_OK;
}

typedef int (*modbus_parser_fun)(modbus_port_pt port, modbus_packet_pt packet);

static void _get_packet_length(modbus_packet_pt packet)
{
  uint16_t len = 0;
  if (packet->exception == 1)
  {
    packet->length = 1;
    return;
  }
  switch (packet->code)
  {
  case MODBUS_MESSAGE_READ_COILS:
  case MODBUS_MESSAGE_READ_DISCRETE_INPUTS:
  case MODBUS_MESSAGE_READ_HOLDING_REGISTERS:
  case MODBUS_MESSAGE_READ_INPUT_REGISTERS:
  case MODBUS_MESSAGE_READ_EXCEPTION_STATUS:
  case MODBUS_MESSAGE_GET_COMMON_EVENT_LOG:
  case MODBUS_MESSAGE_REPORT_SERVER_ID:
    len = 1;
    break;
  case MODBUS_MESSAGE_WRITE_SINGLE_COIL:
  case MODBUS_MESSAGE_WRITE_SINGLE_REGISTER:
  case MODBUS_MESSAGE_WRITE_MULTIPLE_COILS:
  case MODBUS_MESSAGE_WRITE_MULTIPLE_REGISTERS:
  case MODBUS_MESSAGE_IAP:
  case MODBUS_MESSAGE_IAP_CHECK:
    len = 4;
    break;
  case MODBUS_MESSAGE_DIAGNOSTICS:
  case MODBUS_MESSAGE_GET_COMMON_EVENT_COUNTER:
    len = 2;
    break;
  default:
    break;
  }
  packet->length = len;
}

int read_remain_buffer(modbus_port_pt port, modbus_packet_pt packet)
{
  if (packet->exception == 1)
    return MODBUS_OK;

  int len = 0;
  int pos = 1;
  int rc = 0;
  switch (packet->code)
  {
  case MODBUS_MESSAGE_READ_COILS:
  case MODBUS_MESSAGE_READ_DISCRETE_INPUTS:
  case MODBUS_MESSAGE_GET_COMMON_EVENT_LOG:
  case MODBUS_MESSAGE_REPORT_SERVER_ID:
  case MODBUS_MESSAGE_READ_HOLDING_REGISTERS:
  case MODBUS_MESSAGE_READ_INPUT_REGISTERS:
    len = packet->buffer[0];
    break;
  case MODBUS_MESSAGE_DIAGNOSTICS:
    pos = 2;
    len = read_uint16_t_BE(&(packet->buffer[0])) * 2;
    break;
  }
  if (len > 0)
  {
    packet->length += len;
    rc = port->read(&packet->buffer[pos], len, MODBUS_READ_TIMEOUT);
    MODBUS_CHECK_READ_RC_AND_RETURN
  }
  return MODBUS_OK;
}

static int _check_header(modbus_packet_pt packet)
{
  uint8_t code = packet->code & 0x7F;
  if ((packet->addr == (packet->id)) &&
      (
          //  code ==  (MODBUS_MESSAGE_READ_COILS) ||
          //  code ==  (MODBUS_MESSAGE_READ_DISCRETE_INPUTS) ||
          code == (MODBUS_MESSAGE_READ_HOLDING_REGISTERS) ||
          //  code ==  (MODBUS_MESSAGE_READ_INPUT_REGISTERS) ||
          //  code ==  (MODBUS_MESSAGE_WRITE_SINGLE_COIL) ||
          code == (MODBUS_MESSAGE_WRITE_SINGLE_REGISTER) ||
          //  code ==  (MODBUS_MESSAGE_READ_EXCEPTION_STATUS) ||
          //  code ==  (MODBUS_MESSAGE_DIAGNOSTICS) ||
          //  code ==  (MODBUS_MESSAGE_GET_COMMON_EVENT_COUNTER) ||
          //  code ==  (MODBUS_MESSAGE_GET_COMMON_EVENT_LOG) ||
          //  code ==  (MODBUS_MESSAGE_WRITE_MULTIPLE_COILS) ||
          code == (MODBUS_MESSAGE_WRITE_MULTIPLE_REGISTERS
                   //  code ==  (MODBUS_MESSAGE_REPORT_SERVER_ID) ||
                   // code ==  (MODBUS_MESSAGE_IAP) ||
                   // code ==  (MODBUS_MESSAGE_IAP_CHECK
                   )))
  {
    _get_packet_length(packet);
    return MODBUS_OK;
  }
  return MODBUS_ERROR;
}

static uint16_t _header_deserialize(uint8_t *bytes, modbus_packet_pt packet)
{
  uint16_t pos = 0;
  packet->addr = read_uint8_t_BE(&bytes[pos]);
  pos += 1;
  packet->code = read_uint8_t_BE(&bytes[pos]);
  pos += 1;
  packet->exception = ((packet->code & 0x80) == 0x80) ? 1 : 0;
  return pos;
}

#include "app_log.h"
static int _find_header(modbus_port_pt port, modbus_packet_pt packet)
{
  static uint8_t buffer[MODBUS_HEADER_SIZE];
  static uint8_t new_header = 0;
  int rc = 0;
  if (new_header == 0)
  {
    memset(buffer, 0, MODBUS_HEADER_SIZE);
  }

  int len = port->length();
  // APP_LOG_Debug("Rlen=%d\r\n", len);
  // if (len < 1 || (new_header == 0 && (len < MODBUS_HEADER_SIZE)))
  // {
  //   return MODBUS_READ_ERROR;
  // }
  while (1)
  {
    if (new_header == 0)
    {
      rc = port->read(buffer, MODBUS_HEADER_SIZE, MODBUS_READ_TIMEOUT);
    }
    else
    {
      rc = port->read(&buffer[MODBUS_HEADER_SIZE - 1], 1, MODBUS_READ_TIMEOUT);
    }
    MODBUS_CHECK_READ_RC_AND_RETURN
    new_header = 1;
    _header_deserialize(buffer, packet);
    if (_check_header(packet) == MODBUS_OK)
    {
      new_header = 0;
      return MODBUS_OK;
    }
    left_shift_buffer(buffer, MODBUS_HEADER_SIZE);
  }
}

static int _parse_extra(modbus_port_pt port, modbus_packet_pt packet)
{
  uint8_t buf[8] = {0};
  int rc = 0;
  if (packet->length > 0)
  {
    rc = port->read(packet->buffer, packet->length, MODBUS_READ_TIMEOUT);
    MODBUS_CHECK_READ_RC_AND_RETURN
    rc = read_remain_buffer(port, packet);
    if (rc < 0)
    {
      return MODBUS_READ_ERROR;
    }
  }
  rc = port->read(buf, 2, MODBUS_READ_TIMEOUT);
  MODBUS_CHECK_READ_RC_AND_RETURN
  packet->crc = read_uint16_t(buf);
  return MODBUS_OK;
}

static int _parse_verify_crc(modbus_port_pt port, modbus_packet_pt packet)
{
  uint8_t buf[MODBUS_MODBUS_BUFFER_SIZE] = {0};
  uint16_t len = MODBUS_PACKET_serialize(packet, buf);
  if (crc16(buf, len - 2) != packet->crc)
  {
    return MODBUS_CRC_ERROR;
  }
  return MODBUS_OK;
}

WINAPI_EXPORT int WINAPI MODBUS_init(void)
{
  return MODBUS_OK;
}

WINAPI_EXPORT int WINAPI MODBUS_PACKET_init(modbus_packet_pt packet)
{
  packet->addr = packet->id;
  packet->code = 3;
  memset(packet->buffer, 0, MODBUS_MAX_BUFFER_SIZE);
  packet->crc = 0;
  packet->length = 0;
  packet->exception = 0;
  packet->parsed = NULL;
  return MODBUS_OK;
}

WINAPI_EXPORT uint16_t WINAPI MODBUS_PACKET_serialize(modbus_packet_pt packet, uint8_t bytes[MODBUS_MODBUS_BUFFER_SIZE])
{
  uint16_t pos = 0;
  write_uint8_t_BE(packet->addr, &bytes[pos]);
  pos += 1;
  write_uint8_t_BE(packet->code, &bytes[pos]);
  pos += 1;
  memcpy(&bytes[pos], packet->buffer, packet->length);
  pos += packet->length;
  write_uint16_t(packet->crc, &bytes[pos]);
  pos += 2;
  return pos;
}

WINAPI_EXPORT int WINAPI MODBUS_PACKET_read(modbus_port_pt port, modbus_packet_pt packet)
{
  int rc = 0;
  rc = _find_header(port, packet);
  MODBUS_CHECK_RC
  rc = _parse_extra(port, packet);
  MODBUS_CHECK_RC
  rc = _parse_verify_crc(port, packet);
  MODBUS_CHECK_RC
  packet->exception = ((packet->code & 0x80) == 0x80) ? 1 : 0;
  return rc;
}

WINAPI_EXPORT int WINAPI MODBUS_PACKET_write(modbus_port_pt port, modbus_packet_pt packet)
{
  uint8_t buf[MODBUS_MODBUS_BUFFER_SIZE] = {0};
  packet->addr = packet->id;
  uint16_t len = MODBUS_PACKET_serialize(packet, buf);
  if (len <= 2)
  {
    return MODBUS_ERROR;
  }
  uint16_t crc = crc16(buf, len - 2);
  write_uint16_t(crc, &buf[len - 2]);
  port->write(buf, len, MODBUS_WRITE_TIMEOUT);
  return MODBUS_OK;
}
