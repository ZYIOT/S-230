#include "g2_server_packet.h"

static int left_shift_buffer(uint8_t *buf, size_t len)
{
	for (size_t i = 1; i < len; i++)
	{
		buf[i - 1] = buf[i];
	}
	
	return PROTOCOL_OK;
}

typedef int (*g2_server_parser_fun)(g2_server_port_pt port, g2_server_packet_pt packet);

static int _check_header(g2_server_packet_pt packet)
{
	if((packet->deviceID == (G2_SERVER_PROTOCOL_DEVICE_ID) 
		|| packet->deviceID == (0)) 
	&& (packet->code == (0x03) 
		|| packet->code == (0x83) 
		|| packet->code == (0x16) 
		|| packet->code == (0x96)) 
	&& (packet->header == (0x40)))
	{
		return PROTOCOL_OK;
	}
	
	return PROTOCOL_ERROR;
}


uint16_t getPacketLen(g2_server_packet_pt pt)
{
	uint16_t len = 0;

	len = pt->length;
	return len;
}


void setPacketLen(g2_server_packet_pt pt, uint16_t len)
{
	pt->length = len & 0xFF;
}


static uint16_t _header_deserialize(uint8_t *bytes, g2_server_packet_pt packet)
{
	uint16_t pos = 0;
	packet->deviceID = read_uint32_t_BE(&bytes[pos]); 
	pos += 4;
	packet->code = read_uint8_t_BE(&bytes[pos]); 
	pos += 1;
	packet->header = read_uint8_t_BE(&bytes[pos]); 
	pos += 1;
	packet->cmd_id = read_uint16_t_BE(&bytes[pos]); 
	pos += 2;
	packet->cmd = read_uint8_t_BE(&bytes[pos]); 
	pos += 1;
	packet->cmd_status = read_uint8_t_BE(&bytes[pos]); 
	pos += 1;
	packet->reversed = read_uint8_t_BE(&bytes[pos]); 
	pos += 1;
	packet->version = read_uint8_t_BE(&bytes[pos]); 
	pos += 1;
	packet->length = read_uint8_t_BE(&bytes[pos]); 
	pos += 1;
	
	return pos;
}

static int _find_header(g2_server_port_pt port, g2_server_packet_pt packet)
{
	static uint8_t buffer[G2_SERVER_HEADER_SIZE];
	static uint8_t new_header = 0;
	int rc = 0;
	
	if(new_header == 0)
	{
		memset(buffer, 0, G2_SERVER_HEADER_SIZE);
	}
	int len = port->length();
	if (len < 1 || (new_header == 0 && (len < G2_SERVER_HEADER_SIZE)))
	{
		return PROTOCOL_READ_ERROR;
	}
	while (1)
	{
		if (new_header == 0)
		{
			rc = port->read(buffer, G2_SERVER_HEADER_SIZE, G2_SERVER_READ_TIMEOUT);
		}
		else
		{
			rc = port->read(&buffer[G2_SERVER_HEADER_SIZE - 1], 1, G2_SERVER_READ_TIMEOUT);
		}
		new_header = 1;
		G2_SERVER_CHECK_READ_RC_AND_RETURN
		_header_deserialize(buffer, packet);
		if (_check_header(packet) == PROTOCOL_OK)
		{
			new_header = 0;
			return PROTOCOL_OK;
		}
		left_shift_buffer(buffer, G2_SERVER_HEADER_SIZE);
	}
}

static int _parse_extra(g2_server_port_pt port, g2_server_packet_pt packet)
{
	uint8_t buf[8] = {0};
	uint16_t len = 0;
	int rc = 0;

	len = getPacketLen(packet);
	if (len > 0)
	{
		rc = port->read(packet->buffer, len, G2_SERVER_READ_TIMEOUT);
		G2_SERVER_CHECK_READ_RC_AND_RETURN
	}
	rc = port->read(buf, 2, G2_SERVER_READ_TIMEOUT);
	G2_SERVER_CHECK_READ_RC_AND_RETURN
	packet->crc = read_uint16_t(buf); 

	return PROTOCOL_OK;
}

static int _parse_verify_crc(g2_server_port_pt port, g2_server_packet_pt packet)
{
	uint8_t buf[G2_SERVER_PROTOCOL_BUFFER_SIZE] = {0};
	uint16_t len = 0;
	uint16_t bufcrc = 0xFFFF;
	
	len = G2_SERVER_PACKET_serialize(packet, buf);	
	bufcrc = crc16(buf, len - 2);
	if (bufcrc != packet->crc)
	{
		return PROTOCOL_CRC_ERROR;
	}
	
	return PROTOCOL_OK;
}

static HARDWARE_SEMAPHORE_TYPE_T serialize_sem;
WINAPI_EXPORT int WINAPI G2_SERVER_init(void)
{
    serialize_sem = HARDWARE_CREATE_SEMAPHORE();
	return PROTOCOL_OK;
}

WINAPI_EXPORT int WINAPI G2_SERVER_PACKET_init(g2_server_packet_pt packet)
{
	packet->deviceID = G2_SERVER_PROTOCOL_DEVICE_ID; 
	packet->code = G2_SERVER_MESSAGE_OPERATION_READ; 
	packet->header = 0x40; 
	packet->cmd_id = 0; 
	packet->cmd = 0; 
	packet->cmd_status = 0xd0; 
	packet->reversed = 0; 
	packet->version = 1; 
	packet->length = 0; 
	memset(packet->buffer, 0, G2_SERVER_MAX_BUFFER_SIZE); 
	packet->crc = 0; 
	packet->exception = 0; 
	packet->parsed = NULL;
	
	return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_PACKET_serialize(g2_server_packet_pt packet, uint8_t bytes[G2_SERVER_PROTOCOL_BUFFER_SIZE])
{
	uint16_t pos = 0;
	uint16_t len = 0;

	write_uint32_t_BE(packet->deviceID, &bytes[pos]); 
	pos += 4;
	write_uint8_t_BE(packet->code, &bytes[pos]); 
	pos += 1;
	write_uint8_t_BE(packet->header, &bytes[pos]); 
	pos += 1;
	write_uint16_t_BE(packet->cmd_id, &bytes[pos]); 
	pos += 2;
	write_uint8_t_BE(packet->cmd, &bytes[pos]); 
	pos += 1;
	write_uint8_t_BE(packet->cmd_status, &bytes[pos]); 
	pos += 1;
	write_uint8_t_BE(packet->reversed, &bytes[pos]); 
	pos += 1;
	write_uint8_t_BE(packet->version, &bytes[pos]); 
	pos += 1;
	write_uint8_t_BE(packet->length, &bytes[pos]); 
	pos += 1;
	len = getPacketLen(packet);
	memcpy(&bytes[pos], packet->buffer, len);	
	pos += len;
	write_uint16_t(packet->crc, &bytes[pos]); 
	pos += 2;

	return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_PACKET_read(g2_server_port_pt port, g2_server_packet_pt packet)
{
	uint16_t len = 0;
	int rc = 0;
	
	rc = _find_header(port, packet);
	G2_SERVER_CHECK_RC
	rc = _parse_extra(port, packet);
	G2_SERVER_CHECK_RC
	rc = _parse_verify_crc(port, packet);
	G2_SERVER_CHECK_RC
	len = getPacketLen(packet);	
	if(packet->code == G2_SERVER_MESSAGE_OPERATION_WRITE 
	&& len > 0 
	&& packet->buffer[0] == G2_SERVER_OPERATION_CODE_DELETE )
	{
		packet->code=G2_SERVER_MESSAGE_OPERATION_DELETE;
	}
	packet->exception = ((packet->code & 0x80) == 0x80) ? 1 : 0;
	
	return rc;
}

#include "app_log.h"
uint32_t sendInCnt = 0;
uint32_t sendOutCnt = 0;
static uint8_t sendBuf[G2_SERVER_PROTOCOL_BUFFER_SIZE] = {0};
WINAPI_EXPORT int WINAPI G2_SERVER_PACKET_write(g2_server_port_pt port, g2_server_packet_pt packet)
{
	sendInCnt += 1;
	// APP_LOG_Debug("PROTOCOL send in cnt = %d\r\n", sendInCnt);
    HARDWARE_TAKE_SEMAPHORE(serialize_sem);		// 加锁，避免函数重入导致 sendBuf 数据错乱 
	packet->deviceID = G2_SERVER_PROTOCOL_DEVICE_ID;
	if(packet->code == G2_SERVER_MESSAGE_OPERATION_DELETE)
	{
		packet->code=G2_SERVER_MESSAGE_OPERATION_WRITE;
	}
	packet->version = 1;
	uint16_t len = G2_SERVER_PACKET_serialize(packet, sendBuf);
	if(len <= 2)
	{
    	HARDWARE_GIVE_SEMAPHORE(serialize_sem);
		return PROTOCOL_ERROR;
	}
	uint16_t crc = crc16(sendBuf, len - 2);
	write_uint16_t(crc, &sendBuf[len - 2]);
	port->write(sendBuf, len, G2_SERVER_WRITE_TIMEOUT);
#ifdef SUPPORT_UART5_CONFIG_OPTION
	if((G2_SERVER_MESSAGE_DEVICE_ID == packet->cmd)
	|| (G2_SERVER_MESSAGE_VERSION == packet->cmd)
	|| (G2_SERVER_MESSAGE_SENSOR_CONFIG == packet->cmd)
	|| (G2_SERVER_MESSAGE_TIME == packet->cmd)
	|| (G2_SERVER_MESSAGE_RELAY_DATA == packet->cmd)
	|| (G2_SERVER_MESSAGE_PUMP == packet->cmd))
	{
		BSP_PROTOCOL_pc_send_message(sendBuf, len);
	}
#endif
    HARDWARE_GIVE_SEMAPHORE(serialize_sem);
	sendOutCnt += 1;
	// APP_LOG_Debug("PROTOCOL send out cnt = %d\r\n", sendOutCnt);
	return PROTOCOL_OK;
}


