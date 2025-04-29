#include "g2_server_message.h"

WINAPI_EXPORT int WINAPI G2_SERVER_device_id_message_init(g2_server_device_id_message_pt message)
{
    message->deviceID = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_id_message_serialize(uint8_t *bytes, g2_server_device_id_message_pt message)
{
    uint16_t pos = 0;
    write_uint32_t_BE(message->deviceID, &bytes[pos]);
    pos += 4;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_id_message_deserialize(uint8_t *bytes, g2_server_device_id_message_pt message)
{
    uint16_t pos = 0;
    message->deviceID = read_uint32_t_BE(&bytes[pos]);
    pos += 4;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_device_version_message_init(g2_server_device_version_message_pt message)
{
    memset(message->hardware, 0, 3);
    memset(message->firmware, 0, 3);
    memset(message->PN, 0, 10);
    memset(message->SN, 0, 11);
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_version_message_serialize(uint8_t *bytes, g2_server_device_version_message_pt message)
{
    uint16_t pos = 0;
    memcpy(&bytes[pos], message->hardware, 3);
    pos += 3;
    memcpy(&bytes[pos], message->firmware, 3);
    pos += 3;
    memcpy(&bytes[pos], message->PN, 10);
    pos += 10;
    memcpy(&bytes[pos], message->SN, 11);
    pos += 11;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_version_message_deserialize(uint8_t *bytes, g2_server_device_version_message_pt message)
{
    uint16_t pos = 0;
    memcpy(message->hardware, &bytes[pos], 3);
    pos += 3;
    memcpy(message->firmware, &bytes[pos], 3);
    pos += 3;
    memcpy(message->PN, &bytes[pos], 10);
    pos += 10;
    memcpy(message->SN, &bytes[pos], 11);
    pos += 11;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_probe_id_message_init(g2_server_probe_id_message_pt message)
{
    message->probeID = 1;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_probe_id_message_serialize(uint8_t *bytes, g2_server_probe_id_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_probe_id_message_deserialize(uint8_t *bytes, g2_server_probe_id_message_pt message)
{
    uint16_t pos = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_probe_ids_message_init(g2_server_probe_ids_message_pt message)
{
    message->count = 0;
    message->probeID = 1;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_probe_ids_message_serialize(uint8_t *bytes, g2_server_probe_ids_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->count, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_probe_ids_message_deserialize(uint8_t *bytes, g2_server_probe_ids_message_pt message)
{
    uint16_t pos = 0;
    message->count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_relay_id_message_init(g2_server_relay_id_message_pt message)
{
    message->relayID = 1;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_id_message_serialize(uint8_t *bytes, g2_server_relay_id_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_id_message_deserialize(uint8_t *bytes, g2_server_relay_id_message_pt message)
{
    uint16_t pos = 0;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_relay_ids_message_init(g2_server_relay_ids_message_pt message)
{
    message->count = 0;
    message->relayID = 1;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_ids_message_serialize(uint8_t *bytes, g2_server_relay_ids_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->count, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_ids_message_deserialize(uint8_t *bytes, g2_server_relay_ids_message_pt message)
{
    uint16_t pos = 0;
    message->count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_config_data_message_init(g2_server_sensor_config_data_message_pt message)
{
    message->manufacturer = 0;
    message->type = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_config_data_message_serialize(uint8_t *bytes, g2_server_sensor_config_data_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->manufacturer, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->type, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_config_data_message_deserialize(uint8_t *bytes, g2_server_sensor_config_data_message_pt message)
{
    uint16_t pos = 0;
    message->manufacturer = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->type = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_config_message_init(g2_server_sensor_config_message_pt message)
{
    message->probeID = 1;
    message->config = 0;
    int i = 0;
    for(i = 0; i < PROBE_SENSOR_SIZE; i++){
        G2_SERVER_sensor_config_data_message_init(&message->data[i]);
    }
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_config_message_serialize(uint8_t *bytes, g2_server_sensor_config_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->config, bits); i++){
        uint8_t index = bits[i] - 1;
        if (index < PROBE_SENSOR_SIZE)
        {
            len = G2_SERVER_sensor_config_data_message_serialize(&bytes[pos], &message->data[index]);
        }
        else
        {
            len = 0;
        }
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_config_message_deserialize(uint8_t *bytes, g2_server_sensor_config_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->config, bits); i++){
        uint8_t index = bits[i] - 1;
        if (index < PROBE_SENSOR_SIZE)
        {
            len = G2_SERVER_sensor_config_data_message_deserialize(&bytes[pos], &message->data[index]);
        }
        else
        {
            len = 2;
        }
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_write_sensor_config_message_init(g2_server_write_sensor_config_message_pt message)
{
    message->oper = G2_SERVER_OPERATION_CODE_WRITE;
    G2_SERVER_sensor_config_message_init(&message->sensor);
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_config_message_serialize(uint8_t *bytes, g2_server_write_sensor_config_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->oper, &bytes[pos]);
    pos += 1;
    len = G2_SERVER_sensor_config_message_serialize(&bytes[pos], &message->sensor);
    pos += len;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_config_message_deserialize(uint8_t *bytes, g2_server_write_sensor_config_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->oper = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    len = G2_SERVER_sensor_config_message_deserialize(&bytes[pos], &message->sensor);
    pos += len;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_delete_sensor_config_message_init(g2_server_delete_sensor_config_message_pt message)
{
    message->oper = G2_SERVER_OPERATION_CODE_DELETE;
    message->probeID = 1;
    message->config = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_config_message_serialize(uint8_t *bytes, g2_server_delete_sensor_config_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->oper, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_config_message_deserialize(uint8_t *bytes, g2_server_delete_sensor_config_message_pt message)
{
    uint16_t pos = 0;
    message->oper = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_limit_data_message_init(g2_server_sensor_limit_data_message_pt message)
{
    message->relay_channel = 0;
    message->reversed_relay_channel = 0;
    message->type = 0;
    message->max = 0;
    message->min = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_data_message_serialize(uint8_t *bytes, g2_server_sensor_limit_data_message_pt message)
{
    uint16_t pos = 0;
    write_uint16_t_BE(message->relay_channel, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->reversed_relay_channel, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->type, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->max, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->min, &bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_data_message_deserialize(uint8_t *bytes, g2_server_sensor_limit_data_message_pt message)
{
    uint16_t pos = 0;
    message->relay_channel = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->reversed_relay_channel = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->type = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->max = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->min = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_limit_gradient_message_init(g2_server_sensor_limit_gradient_message_pt message)
{
    message->relayID = 1;
    message->gradient = 0;
    int i = 0;
    for(i = 0; i < 8; i++){
        G2_SERVER_sensor_limit_data_message_init(&message->data[i]);
    }
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_gradient_message_serialize(uint8_t *bytes, g2_server_sensor_limit_gradient_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->gradient, &bytes[pos]);
    pos += 1;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(((uint16_t)message->gradient)<< 8, bits); i++){
        len = G2_SERVER_sensor_limit_data_message_serialize(&bytes[pos], &message->data[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_gradient_message_deserialize(uint8_t *bytes, g2_server_sensor_limit_gradient_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->gradient = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(((uint16_t)message->gradient)<< 8, bits); i++){
        len = G2_SERVER_sensor_limit_data_message_deserialize(&bytes[pos], &message->data[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_limit_message_init(g2_server_sensor_limit_message_pt message)
{
    message->probeID = 1;
    message->config = 0;
    int i = 0;
    for(i = 0; i < INDICATOR_SIZE; i++){
        G2_SERVER_sensor_limit_gradient_message_init(&message->sensors[i]);
    }
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_message_serialize(uint8_t *bytes, g2_server_sensor_limit_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->config, bits); i++){
        len = G2_SERVER_sensor_limit_gradient_message_serialize(&bytes[pos], &message->sensors[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_message_deserialize(uint8_t *bytes, g2_server_sensor_limit_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->config, bits); i++){
        len = G2_SERVER_sensor_limit_gradient_message_deserialize(&bytes[pos], &message->sensors[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_write_sensor_limit_message_init(g2_server_write_sensor_limit_message_pt message)
{
    message->oper = G2_SERVER_OPERATION_CODE_WRITE;
    G2_SERVER_sensor_limit_message_init(&message->sensor);
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_limit_message_serialize(uint8_t *bytes, g2_server_write_sensor_limit_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->oper, &bytes[pos]);
    pos += 1;
    len = G2_SERVER_sensor_limit_message_serialize(&bytes[pos], &message->sensor);
    pos += len;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_limit_message_deserialize(uint8_t *bytes, g2_server_write_sensor_limit_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->oper = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    len = G2_SERVER_sensor_limit_message_deserialize(&bytes[pos], &message->sensor);
    pos += len;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_delete_sensor_limit_message_init(g2_server_delete_sensor_limit_message_pt message)
{
    message->oper = G2_SERVER_OPERATION_CODE_DELETE;
    message->probeID = 1;
    message->config = 0;
    message->relayID = 1;
    message->gradient = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_limit_message_serialize(uint8_t *bytes, g2_server_delete_sensor_limit_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->oper, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->gradient, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_limit_message_deserialize(uint8_t *bytes, g2_server_delete_sensor_limit_message_pt message)
{
    uint16_t pos = 0;
    message->oper = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->gradient = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_alert_data_message_init(g2_server_sensor_alert_data_message_pt message)
{
    message->max = 0;
    message->min = 0;
    message->threshold = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_alert_data_message_serialize(uint8_t *bytes, g2_server_sensor_alert_data_message_pt message)
{
    uint16_t pos = 0;
    write_uint16_t_BE(message->max, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->min, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->threshold, &bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_alert_data_message_deserialize(uint8_t *bytes, g2_server_sensor_alert_data_message_pt message)
{
    uint16_t pos = 0;
    message->max = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->min = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->threshold = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_alert_message_init(g2_server_sensor_alert_message_pt message)
{
    message->probeID = 1;
    message->config = 0;
    int i = 0;
    for(i = 0; i < INDICATOR_SIZE; i++){
        G2_SERVER_sensor_alert_data_message_init(&message->data[i]);
    }
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_alert_message_serialize(uint8_t *bytes, g2_server_sensor_alert_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->config, bits); i++){
        len = G2_SERVER_sensor_alert_data_message_serialize(&bytes[pos], &message->data[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_alert_message_deserialize(uint8_t *bytes, g2_server_sensor_alert_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->config, bits); i++){
        len = G2_SERVER_sensor_alert_data_message_deserialize(&bytes[pos], &message->data[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_write_sensor_alert_message_init(g2_server_write_sensor_alert_message_pt message)
{
    message->oper = G2_SERVER_OPERATION_CODE_WRITE;
    G2_SERVER_sensor_alert_message_init(&message->sensor);
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_alert_message_serialize(uint8_t *bytes, g2_server_write_sensor_alert_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->oper, &bytes[pos]);
    pos += 1;
    len = G2_SERVER_sensor_alert_message_serialize(&bytes[pos], &message->sensor);
    pos += len;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_alert_message_deserialize(uint8_t *bytes, g2_server_write_sensor_alert_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->oper = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    len = G2_SERVER_sensor_alert_message_deserialize(&bytes[pos], &message->sensor);
    pos += len;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_delete_sensor_alert_message_init(g2_server_delete_sensor_alert_message_pt message)
{
    message->oper = G2_SERVER_OPERATION_CODE_DELETE;
    message->probeID = 1;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_alert_message_serialize(uint8_t *bytes, g2_server_delete_sensor_alert_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->oper, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_alert_message_deserialize(uint8_t *bytes, g2_server_delete_sensor_alert_message_pt message)
{
    uint16_t pos = 0;
    message->oper = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_relay_job_time_message_init(g2_server_relay_job_time_message_pt message)
{
    message->startHour = 0;
    message->startMinute = 0;
    message->endHour = 0;
    message->endMinute = 0;
    message->type = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_job_time_message_serialize(uint8_t *bytes, g2_server_relay_job_time_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->startHour, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->startMinute, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->endHour, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->endMinute, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->type, &bytes[pos]);
    pos += 1;
  return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_job_time_message_deserialize(uint8_t *bytes, g2_server_relay_job_time_message_pt message)
{
    uint16_t pos = 0;
    message->startHour = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->startMinute = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->endHour = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->endMinute = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->type = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_relay_jobs_task_message_init(g2_server_relay_jobs_task_message_pt message)
{
    message->hasLimit = 0;
    message->relay_channel = 0;
    message->probeID = 1;
    message->sensor_config = 0;
    message->value = 0;
    message->threshold = 0;
    message->executeType = 0;
    message->time_config = 0;
    int i = 0;
    for(i = 0; i < 4; i++){
        G2_SERVER_relay_job_time_message_init(&message->times[i]);
    }
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_task_message_serialize(uint8_t *bytes, g2_server_relay_jobs_task_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->hasLimit, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->relay_channel, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->sensor_config, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->value, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->threshold, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->executeType, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->time_config, &bytes[pos]);
    pos += 1;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(((uint16_t)message->time_config)<< 8, bits); i++){
        len = G2_SERVER_relay_job_time_message_serialize(&bytes[pos], &message->times[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_task_message_deserialize(uint8_t *bytes, g2_server_relay_jobs_task_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->hasLimit = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->relay_channel = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->sensor_config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->value = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->threshold = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->executeType = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->time_config = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(((uint16_t)message->time_config)<< 8, bits); i++){
        len = G2_SERVER_relay_job_time_message_deserialize(&bytes[pos], &message->times[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_relay_jobs_message_init(g2_server_relay_jobs_message_pt message)
{
    message->relayID = 1;
    message->job_config = 0;
    int i = 0;
    for(i = 0; i < 8; i++){
        G2_SERVER_relay_jobs_task_message_init(&message->tasks[i]);
    }
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_message_serialize(uint8_t *bytes, g2_server_relay_jobs_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->job_config, &bytes[pos]);
    pos += 2;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->job_config, bits); i++){
        len = G2_SERVER_relay_jobs_task_message_serialize(&bytes[pos], &message->tasks[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_message_deserialize(uint8_t *bytes, g2_server_relay_jobs_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->job_config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->job_config, bits); i++){
        len = G2_SERVER_relay_jobs_task_message_deserialize(&bytes[pos], &message->tasks[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_write_relay_jobs_message_init(g2_server_write_relay_jobs_message_pt message)
{
    message->oper = G2_SERVER_OPERATION_CODE_WRITE;
    G2_SERVER_relay_jobs_message_init(&message->relay);
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_relay_jobs_message_serialize(uint8_t *bytes, g2_server_write_relay_jobs_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->oper, &bytes[pos]);
    pos += 1;
    len = G2_SERVER_relay_jobs_message_serialize(&bytes[pos], &message->relay);
    pos += len;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_relay_jobs_message_deserialize(uint8_t *bytes, g2_server_write_relay_jobs_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->oper = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    len = G2_SERVER_relay_jobs_message_deserialize(&bytes[pos], &message->relay);
    pos += len;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_delete_relay_jobs_message_init(g2_server_delete_relay_jobs_message_pt message)
{
    message->oper = G2_SERVER_OPERATION_CODE_DELETE;
    message->relayID = 1;
    message->config = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_relay_jobs_message_serialize(uint8_t *bytes, g2_server_delete_relay_jobs_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->oper, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_relay_jobs_message_deserialize(uint8_t *bytes, g2_server_delete_relay_jobs_message_pt message)
{
    uint16_t pos = 0;
    message->oper = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_salinity_message_init(g2_server_salinity_message_pt message)
{
    message->probeID = 1;
    message->salinity = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_salinity_message_serialize(uint8_t *bytes, g2_server_salinity_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->salinity, &bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_salinity_message_deserialize(uint8_t *bytes, g2_server_salinity_message_pt message)
{
    uint16_t pos = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->salinity = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_calibrate_message_init(g2_server_calibrate_message_pt message)
{
    message->probeID = 1;
    message->config = 0;
    message->type = 0;
    message->params = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_calibrate_message_serialize(uint8_t *bytes, g2_server_calibrate_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->type, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->params, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_calibrate_message_deserialize(uint8_t *bytes, g2_server_calibrate_message_pt message)
{
    uint16_t pos = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->type = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->params = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_time_message_init(g2_server_time_message_pt message)
{
    message->year = 0;
    message->month = 0;
    message->day = 0;
    message->week = 0;
    message->hour = 0;
    message->minute = 0;
    message->second = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_time_message_serialize(uint8_t *bytes, g2_server_time_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->year, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->month, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->day, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->week, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->hour, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->minute, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->second, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_time_message_deserialize(uint8_t *bytes, g2_server_time_message_pt message)
{
    uint16_t pos = 0;
    message->year = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->month = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->day = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->week = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->hour = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->minute = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->second = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
  return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_time_without_week_message_init(g2_server_time_without_week_message_pt message)
{
    message->year = 0;
    message->month = 0;
    message->day = 0;
    message->hour = 0;
    message->minute = 0;
    message->second = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_time_without_week_message_serialize(uint8_t *bytes, g2_server_time_without_week_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->year, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->month, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->day, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->hour, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->minute, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->second, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_time_without_week_message_deserialize(uint8_t *bytes, g2_server_time_without_week_message_pt message)
{
    uint16_t pos = 0;
    message->year = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->month = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->day = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->hour = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->minute = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->second = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_relay_manual_message_init(g2_server_relay_manual_message_pt message)
{
    message->relayID = 1;
    message->relay_channel = 0;
    message->type = 0;
    message->duration = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_manual_message_serialize(uint8_t *bytes, g2_server_relay_manual_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->relay_channel, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->type, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->duration, &bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_manual_message_deserialize(uint8_t *bytes, g2_server_relay_manual_message_pt message)
{
    uint16_t pos = 0;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->relay_channel = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->type = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->duration = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_relay_manual_enable_message_init(g2_server_relay_manual_enable_message_pt message)
{
    message->relayID = 1;
    message->relay_channel = 0;
    message->enable = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_manual_enable_message_serialize(uint8_t *bytes, g2_server_relay_manual_enable_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->relay_channel, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->enable, &bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_manual_enable_message_deserialize(uint8_t *bytes, g2_server_relay_manual_enable_message_pt message)
{
    uint16_t pos = 0;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->relay_channel = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->enable = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_system_message_init(g2_server_system_message_pt message)
{
    message->config = 0;
    message->display = 0;
    message->display_refresh = 0;
    message->push_data_interval = 0;
    message->push_signal_interval = 0;
    message->sensor_wireless_interval = 0;
    message->relay_wireless_interval = 0;
    message->relay_release_time = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_system_message_serialize(uint8_t *bytes, g2_server_system_message_pt message)
{
    uint16_t pos = 0;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->display, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->display_refresh, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->push_data_interval, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->push_signal_interval, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->sensor_wireless_interval, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->relay_wireless_interval, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->relay_release_time, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_system_message_deserialize(uint8_t *bytes, g2_server_system_message_pt message)
{
    uint16_t pos = 0;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->display = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->display_refresh = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->push_data_interval = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->push_signal_interval = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->sensor_wireless_interval = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->relay_wireless_interval = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->relay_release_time = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_connect_message_init(g2_server_connect_message_pt message)
{
    message->reason = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_connect_message_serialize(uint8_t *bytes, g2_server_connect_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->reason, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_connect_message_deserialize(uint8_t *bytes, g2_server_connect_message_pt message)
{
    uint16_t pos = 0;
    message->reason = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_connect_enable_message_init(g2_server_connect_enable_message_pt message)
{
    message->enable = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_connect_enable_message_serialize(uint8_t *bytes, g2_server_connect_enable_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->enable, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_connect_enable_message_deserialize(uint8_t *bytes, g2_server_connect_enable_message_pt message)
{
    uint16_t pos = 0;
    message->enable = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_limit_enable_message_init(g2_server_sensor_limit_enable_message_pt message)
{
    message->probeID = 1;
    message->config = 0;
    message->enable = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_enable_message_serialize(uint8_t *bytes, g2_server_sensor_limit_enable_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->enable, &bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_enable_message_deserialize(uint8_t *bytes, g2_server_sensor_limit_enable_message_pt message)
{
    uint16_t pos = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->enable = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_device_enable_message_init(g2_server_device_enable_message_pt message)
{
    message->type = 0;
    message->node_id = 0;
    message->enable = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_enable_message_serialize(uint8_t *bytes, g2_server_device_enable_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->type, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->node_id, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->enable, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_enable_message_deserialize(uint8_t *bytes, g2_server_device_enable_message_pt message)
{
    uint16_t pos = 0;
    message->type = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->node_id = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->enable = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_device_capability_message_init(g2_server_device_capability_message_pt message)
{
    message->config = 0;
    message->sensor_size = 0;
    message->probe_size_per_sensor = 0;
    message->gradient_size_per_probe = 0;
    message->calibrate_jobs_per_sensor = 0;
    message->relay_size = 0;
    message->channel_size_per_relay = 0;
    message->job_size_per_channel = 0;
    message->time_size_per_job = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_capability_message_serialize(uint8_t *bytes, g2_server_device_capability_message_pt message)
{
    uint16_t pos = 0;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->sensor_size, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->probe_size_per_sensor, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->gradient_size_per_probe, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->calibrate_jobs_per_sensor, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->relay_size, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->channel_size_per_relay, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->job_size_per_channel, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->time_size_per_job, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_capability_message_deserialize(uint8_t *bytes, g2_server_device_capability_message_pt message)
{
    uint16_t pos = 0;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->sensor_size = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->probe_size_per_sensor = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->gradient_size_per_probe = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->calibrate_jobs_per_sensor = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->relay_size = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->channel_size_per_relay = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->job_size_per_channel = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->time_size_per_job = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}


WINAPI_EXPORT int WINAPI G2_SERVER_relay_data_message_init(g2_server_relay_data_message_pt message)
{
    message->relayID = 1;
    G2_SERVER_time_without_week_message_init(&message->time);
    message->config = 0;
    message->limit_status = 0;
    message->times_status = 0;
    message->manual_status = 0;
    message->remote_manual_status = 0;
    message->local_manual_status = 0;
    message->all_off_manual_status = 0;
    message->output_status = 0;
    message->relay_output_status = 0;
    message->relay_broken_status = 0;
    message->relay_leakage_status = 0;
    message->relay_loss_phase_status = 0;
    message->relay_overload_status = 0;
    message->relay_ouv_status = 0;
    message->relay_loss_phase_protect_status = 0;
    message->relay_overload_protect_status = 0;
    message->relay_ouv_protect_status = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_data_message_serialize(uint8_t *bytes, g2_server_relay_data_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    len = G2_SERVER_time_without_week_message_serialize(&bytes[pos], &message->time);
    pos += len;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    if(config_bit_is_set(message->config,1)){
        write_uint16_t_BE(message->limit_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,2)){
        write_uint16_t_BE(message->times_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,3)){
        write_uint16_t_BE(message->manual_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,4)){
        write_uint16_t_BE(message->remote_manual_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,5)){
        write_uint16_t_BE(message->local_manual_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,6)){
        write_uint16_t_BE(message->all_off_manual_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,7)){
        write_uint16_t_BE(message->output_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,8)){
        write_uint16_t_BE(message->relay_output_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,9)){
        write_uint16_t_BE(message->relay_broken_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,10)){
        write_uint16_t_BE(message->relay_leakage_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,11)){
        write_uint16_t_BE(message->relay_loss_phase_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,12)){
        write_uint16_t_BE(message->relay_overload_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,13)){
        write_uint16_t_BE(message->relay_ouv_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,14)){
        write_uint16_t_BE(message->relay_loss_phase_protect_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,15)){
        write_uint16_t_BE(message->relay_overload_protect_status, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,16)){
        write_uint16_t_BE(message->relay_ouv_protect_status, &bytes[pos]);
        pos += 2;
    }
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_data_message_deserialize(uint8_t *bytes, g2_server_relay_data_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    len = G2_SERVER_time_without_week_message_deserialize(&bytes[pos], &message->time);
    pos += len;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    if(config_bit_is_set(message->config,1)){
        message->limit_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,2)){
        message->times_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,3)){
        message->manual_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,4)){
        message->remote_manual_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,5)){
        message->local_manual_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,6)){
        message->all_off_manual_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,7)){
        message->output_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,8)){
        message->relay_output_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,9)){
        message->relay_broken_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,10)){
        message->relay_leakage_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,11)){
        message->relay_loss_phase_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,12)){
        message->relay_overload_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,13)){
        message->relay_ouv_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,14)){
        message->relay_loss_phase_protect_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,15)){
        message->relay_overload_protect_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,16)){
        message->relay_ouv_protect_status = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    return pos;
}




WINAPI_EXPORT int WINAPI G2_SERVER_sensor_data_indicator_message_init(g2_server_sensor_data_indicator_message_pt message)
{
    message->value = 0;
    message->value_mv = 0;
    message->probe_status = 0;
    message->data_status = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_data_indicator_message_serialize(uint8_t *bytes, g2_server_sensor_data_indicator_message_pt message)
{
    uint16_t pos = 0;
    write_uint16_t_BE(message->value, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->value_mv, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->probe_status, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->data_status, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_data_indicator_message_deserialize(uint8_t *bytes, g2_server_sensor_data_indicator_message_pt message)
{
    uint16_t pos = 0;
    message->value = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->value_mv = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->probe_status = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->data_status = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_data_message_init(g2_server_sensor_data_message_pt message)
{
    message->probeID = 1;
    G2_SERVER_time_without_week_message_init(&message->time);
    message->pump_status = 0;
    message->pump_broken_status = 0;
    message->pump_leakage_status = 0;
    message->config = 0;
    int i = 0;
    for(i = 0; i < INDICATOR_SIZE; i++){
        G2_SERVER_sensor_data_indicator_message_init(&message->indicators[i]);
    }
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_data_message_serialize(uint8_t *bytes, g2_server_sensor_data_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    len = G2_SERVER_time_without_week_message_serialize(&bytes[pos], &message->time);
    pos += len;
    write_uint16_t_BE(message->pump_status, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->pump_broken_status, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->pump_leakage_status, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->config, bits); i++){
        len = G2_SERVER_sensor_data_indicator_message_serialize(&bytes[pos], &message->indicators[bits[i] - 1]);
        pos += len;
    }
  return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_data_message_deserialize(uint8_t *bytes, g2_server_sensor_data_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    len = G2_SERVER_time_without_week_message_deserialize(&bytes[pos], &message->time);
    pos += len;
    message->pump_status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->pump_broken_status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->pump_leakage_status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->config, bits); i++){
        len = G2_SERVER_sensor_data_indicator_message_deserialize(&bytes[pos], &message->indicators[bits[i] - 1]);
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_device_used_capability_message_init(g2_server_device_used_capability_message_pt message)
{
    message->sensor_count = 0;
    message->relay_count = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_used_capability_message_serialize(uint8_t *bytes, g2_server_device_used_capability_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->sensor_count, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->relay_count, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_used_capability_message_deserialize(uint8_t *bytes, g2_server_device_used_capability_message_pt message)
{
    uint16_t pos = 0;
    message->sensor_count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->relay_count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_relay_config_message_init(g2_server_relay_config_message_pt message)
{
    message->relayID = 1;
    message->channel_count = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_config_message_serialize(uint8_t *bytes, g2_server_relay_config_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->channel_count, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_config_message_deserialize(uint8_t *bytes, g2_server_relay_config_message_pt message)
{
    uint16_t pos = 0;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->channel_count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_write_relay_config_message_init(g2_server_write_relay_config_message_pt message)
{
    message->oper = G2_SERVER_OPERATION_CODE_WRITE;
    G2_SERVER_relay_config_message_init(&message->relay);
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_relay_config_message_serialize(uint8_t *bytes, g2_server_write_relay_config_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    write_uint8_t_BE(message->oper, &bytes[pos]);
    pos += 1;
    len = G2_SERVER_relay_config_message_serialize(&bytes[pos], &message->relay);
    pos += len;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_relay_config_message_deserialize(uint8_t *bytes, g2_server_write_relay_config_message_pt message)
{
    uint16_t pos = 0;
    uint16_t len = 0;
    message->oper = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    len = G2_SERVER_relay_config_message_deserialize(&bytes[pos], &message->relay);
    pos += len;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_delete_relay_config_message_init(g2_server_delete_relay_config_message_pt message)
{
    message->oper = G2_SERVER_OPERATION_CODE_WRITE;
    message->relayID = 1;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_relay_config_message_serialize(uint8_t *bytes, g2_server_delete_relay_config_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->oper, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_relay_config_message_deserialize(uint8_t *bytes, g2_server_delete_relay_config_message_pt message)
{
    uint16_t pos = 0;
    message->oper = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_relay_jobs_enable_message_init(g2_server_relay_jobs_enable_message_pt message)
{
    message->relayID = 1;
    message->config = 0;
    message->enable = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_enable_message_serialize(uint8_t *bytes, g2_server_relay_jobs_enable_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->relayID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->enable, &bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_enable_message_deserialize(uint8_t *bytes, g2_server_relay_jobs_enable_message_pt message)
{
    uint16_t pos = 0;
    message->relayID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->enable = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_pump_message_init(g2_server_pump_message_pt message)
{
    message->probeID = 1;
    message->channel = 0;
    message->status = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_pump_message_serialize(uint8_t *bytes, g2_server_pump_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->channel, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->status, &bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_pump_message_deserialize(uint8_t *bytes, g2_server_pump_message_pt message)
{
    uint16_t pos = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->channel = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_gprs_signal_message_init(g2_server_gprs_signal_message_pt message)
{
    message->rssi = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_gprs_signal_message_serialize(uint8_t *bytes, g2_server_gprs_signal_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->rssi, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_gprs_signal_message_deserialize(uint8_t *bytes, g2_server_gprs_signal_message_pt message)
{
    uint16_t pos = 0;
    message->rssi = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_l6_settings_message_init(g2_server_l6_settings_message_pt message)
{
    message->probeID = 1;
    message->config = 0;
    message->release = 20;
    message->readWait = 90;
    message->read = 50;
    message->charge = 20;
    message->chargeWait = 6;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_l6_settings_message_serialize(uint8_t *bytes, g2_server_l6_settings_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    if(config_bit_is_set(message->config,1)){
        write_uint8_t_BE(message->release, &bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,2)){
        write_uint8_t_BE(message->readWait, &bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,3)){
        write_uint8_t_BE(message->read, &bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,4)){
        write_uint8_t_BE(message->charge, &bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,5)){
        write_uint8_t_BE(message->chargeWait, &bytes[pos]);
        pos += 1;
    }
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_l6_settings_message_deserialize(uint8_t *bytes, g2_server_l6_settings_message_pt message)
{
    uint16_t pos = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    if(config_bit_is_set(message->config,1)){
        message->release = read_uint8_t_BE(&bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,2)){
        message->readWait = read_uint8_t_BE(&bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,3)){
        message->read = read_uint8_t_BE(&bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,4)){
        message->charge = read_uint8_t_BE(&bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,5)){
        message->chargeWait = read_uint8_t_BE(&bytes[pos]);
        pos += 1;
    }
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_l6_status_message_init(g2_server_l6_status_message_pt message)
{
    message->probeID = 1;
    message->config = 0;
    message->device_status = 0;
    message->data_status = 0;
    message->bump_status = 0;
    message->solenoid_status = 0;
    message->liquid_level_status = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_l6_status_message_serialize(uint8_t *bytes, g2_server_l6_status_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    if(config_bit_is_set(message->config,1)){
        write_uint8_t_BE(message->device_status, &bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,2)){
        write_uint8_t_BE(message->data_status, &bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,3)){
        write_uint8_t_BE(message->bump_status, &bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,4)){
        write_uint8_t_BE(message->solenoid_status, &bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,5)){
        write_uint8_t_BE(message->liquid_level_status, &bytes[pos]);
        pos += 1;
    }
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_l6_status_message_deserialize(uint8_t *bytes, g2_server_l6_status_message_pt message)
{
    uint16_t pos = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    if(config_bit_is_set(message->config,1)){
        message->device_status = read_uint8_t_BE(&bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,2)){
        message->data_status = read_uint8_t_BE(&bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,3)){
        message->bump_status = read_uint8_t_BE(&bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,4)){
        message->solenoid_status = read_uint8_t_BE(&bytes[pos]);
        pos += 1;
    }
    if(config_bit_is_set(message->config,5)){
        message->liquid_level_status = read_uint8_t_BE(&bytes[pos]);
        pos += 1;
    }
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_environment_message_init(g2_server_environment_message_pt message)
{
    message->config = 0;
    message->temperature = 0;
    message->humidity = 0;
    message->voltage = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_environment_message_serialize(uint8_t *bytes, g2_server_environment_message_pt message)
{
    uint16_t pos = 0;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;
    if(config_bit_is_set(message->config,1)){
        write_int16_t_BE(message->temperature, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,2)){
        write_int16_t_BE(message->humidity, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,3)){
        write_int16_t_BE(message->voltage, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,4)){
        write_uint16_t_BE(message->phaseVoltageA, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,5)){
        write_uint16_t_BE(message->phaseVoltageB, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,6)){
        write_uint16_t_BE(message->phaseVoltageC, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,7)){
        write_uint16_t_BE(message->channelCurrent1, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,8)){
        write_uint16_t_BE(message->channelCurrent2, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,9)){
        write_uint16_t_BE(message->channelCurrent3, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,10)){
        write_uint16_t_BE(message->channelCurrent4, &bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,11)){
        write_uint16_t_BE(message->channelCurrent5, &bytes[pos]);
        pos += 2;
    }
  return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_environment_message_deserialize(uint8_t *bytes, g2_server_environment_message_pt message)
{
    uint16_t pos = 0;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    if(config_bit_is_set(message->config,1)){
        message->temperature = read_int16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,2)){
        message->humidity = read_int16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,3)){
        message->voltage = read_int16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,4)){
        message->phaseVoltageA = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,5)){
        message->phaseVoltageB = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,6)){
        message->phaseVoltageC = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,7)){
        message->channelCurrent1 = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,8)){
        message->channelCurrent2 = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,9)){
        message->channelCurrent3 = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,10)){
        message->channelCurrent4 = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    if(config_bit_is_set(message->config,11)){
        message->channelCurrent5 = read_uint16_t_BE(&bytes[pos]);
        pos += 2;
    }
    return pos;
}


WINAPI_EXPORT int WINAPI G2_SERVER_self_diagnosis_message_init(g2_server_self_diagnosis_message_pt message)
{
    message->type = 0;
    message->status = 0;
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_self_diagnosis_message_serialize(uint8_t *bytes, g2_server_self_diagnosis_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->type, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->status, &bytes[pos]);
    pos += 1;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_self_diagnosis_message_deserialize(uint8_t *bytes, g2_server_self_diagnosis_message_pt message)
{
    uint16_t pos = 0;
    message->type = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->status = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
}
	


/* 固件升级相关数据解析函数 */

WINAPI_EXPORT int WINAPI G2_SERVER_firmware_start_message_init(void)
{
    return PROTOCOL_OK;
}


WINAPI_EXPORT uint16_t WINAPI G2_SERVER_firmware_start_message_serialize(uint8_t *bytes, void *message)
{
    int16_t pos = 0;
    
    return pos;
}


WINAPI_EXPORT uint16_t WINAPI G2_SERVER_firmware_start_message_deserialize(uint8_t *bytes, g2_server_firmware_start_message_pt message)
{
    uint16_t pos = 0;
    
    // 处理接收到的数据
    message->deviceType = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->versionH = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->versionM = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->versionL = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->fileLen = read_uint32_t_BE(&bytes[pos]);
    pos += 4;
    message->fileCrc = read_uint32_t_BE(&bytes[pos]);
    pos += 4;
    
    return pos;
}


WINAPI_EXPORT int WINAPI G2_SERVER_firmware_data_message_init(void)
{
    return PROTOCOL_OK;
}


WINAPI_EXPORT uint16_t WINAPI G2_SERVER_send_firmware_data_ask_message_serialize(uint8_t *bytes, g2_server_firmware_data_ask_pt message)
{
    uint16_t pos = 0;

    write_uint32_t_BE(message->read_offset, &bytes[pos]);
    pos += 4;
    write_uint32_t_BE(message->read_datalen, &bytes[pos]);
    pos += 4;

    return pos;	
}


WINAPI_EXPORT uint16_t WINAPI G2_SERVER_firmware_data_message_deserialize(uint8_t *bytes, g2_server_firmware_data_message_pt message)
{
    uint16_t pos = 0;

    message->data_offset = read_uint32_t_BE(&bytes[pos]);
    pos += 4;
    message->data_len = G2_SERVER_GetRequestDataLength();
    message->data = &bytes[pos];
    pos += message->data_len;
    
    return pos;
}



WINAPI_EXPORT int WINAPI G2_SERVER_send_firmware_update_ack_message_init(void)
{
    return PROTOCOL_OK;
}


WINAPI_EXPORT uint16_t WINAPI G2_SERVER_send_firmware_update_ack_message_serialize(uint8_t *bytes, g2_server_firmware_update_ack_pt message)
{
    int16_t pos = 0;
    
    write_uint8_t_BE(message->file_process_stage, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->file_process_result, &bytes[pos]);
    pos += 1;
    
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_send_firmware_update_ack_message_deserialize(void)
{
    int16_t pos = 0;
    
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_info_message_serialize(uint8_t *bytes, g2_server_sensor_info_message_pt message)
{
    uint16_t pos = 0;
    memcpy(&bytes[pos], (uint8_t *)message->hardware, 3);
    pos += 3;
    memcpy(&bytes[pos], (uint8_t *)message->firmware, 3);
    pos += 3;
    memcpy(&bytes[pos], (uint8_t *)message->PN, 10);
    pos += 10;
    memcpy(&bytes[pos], (uint8_t *)message->SN, 8);
    pos += 8;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_info_message_deserialize(uint8_t *bytes, g2_server_sensor_info_message_pt message)
{
    uint16_t pos = 0;
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_device_sensors_message_init(g2_server_device_sensors_message_pt message)
{
    message->type = 0x21;
    message->probeID = 1;
    message->config = 0x0000;
    g2_server_sensor_info_message_pt sensor_info = message->sensors_info;
    for(uint8_t i=0; i<PROBE_SENSOR_SIZE; i++)
    {
        memset((uint8_t *)sensor_info, 0, sizeof(g2_server_sensor_info_message_t));
        sensor_info++;
    }
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_sensors_message_serialize(uint8_t *bytes, g2_server_device_sensors_message_pt message)
{
    int16_t pos = 0;
    uint16_t len = 0;    
    write_uint8_t_BE(message->type, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->config, &bytes[pos]);
    pos += 2;

    int i = 0;
    uint8_t bits[16];
    for(i = 0; i < config_bit_1_bits(message->config, bits); i++)
    {
        if(1 != bits[i])
        {
            len = G2_SERVER_sensor_info_message_serialize(&bytes[pos], &message->sensors_info[bits[i] - 1]);
        }
        else  // 不上�? DO 传感器中包含的温度配置信�? 
        {
            len = 0;
        }
        pos += len;
    }
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_sensors_message_deserialize(uint8_t *bytes, g2_server_device_sensors_message_pt message)
{
    int16_t pos = 0;
    
    return pos;
}

WINAPI_EXPORT int WINAPI G2_SERVER_iccid_message_init(g2_server_sim_iccid_message_pt message)
{
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_iccid_message_serialize(uint8_t *bytes, g2_server_sim_iccid_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->manufacturer, &bytes[pos]);
    pos += 1;
    memset(&bytes[pos], 0, 20);
    memcpy(&bytes[pos], message->iccid, message->len);
    pos += 20;
    return pos;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_iccid_message_deserialize(uint8_t *bytes, g2_server_sim_iccid_message_pt message)
{
    uint16_t pos = 0;
    return pos;
}


/* 继电器通道功率和保护配置相�? */
WINAPI_EXPORT int WINAPI G2_SERVER_relay_power_config_message_init(g2_server_relay_power_config_message_pt message)
{
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_power_config_message_serialize(uint8_t *bytes, g2_server_relay_power_config_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(0xF800 , &bytes[pos]);
    pos += 2;
    g2_server_channel_power_pt power = NULL;
    
    for(uint8_t j=0; j<RELAY_SIZE; j++)
    {
        power = (g2_server_channel_power_pt)(&message->power[j][0]);
        for(uint8_t i=0; i< RELAY_CHANNEL_SIZE; i++)
        {
            write_uint8_t_BE(power->phaseConfig, &bytes[pos]);
            pos += 1;
            write_uint8_t_BE(power->power, &bytes[pos]);
            pos += 1;
            write_uint8_t_BE(power->lossPhaseProtectEnable, &bytes[pos]);
            pos += 1;
            write_uint8_t_BE(power->overloadProtectEnable, &bytes[pos]);
            pos += 1;
            write_uint8_t_BE(power->ouvProtectEnable, &bytes[pos]);
            pos += 1;
            power++;
        }
    }
    return pos;
}


WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_power_config_message_deserialize(uint8_t *bytes, g2_server_relay_power_config_message_pt message)
{
    uint16_t pos = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    g2_server_channel_power_pt relay_channel_power = &(message->power[0][0]);
    for(uint8_t j=0; j<RELAY_SIZE; j++)
    {
        for(uint8_t i=0; i< RELAY_CHANNEL_SIZE; i++)
        {
            if(config_bit_is_set(message->config, (j * RELAY_SIZE) + (i + 1)))
            {
                relay_channel_power->phaseConfig = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
                relay_channel_power->power = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
                relay_channel_power->lossPhaseProtectEnable = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
                relay_channel_power->overloadProtectEnable = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
                relay_channel_power->ouvProtectEnable = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
            }
            relay_channel_power++;
        }
    }
    return pos;
}


/* 继电器通道负载类型配置相关 */
WINAPI_EXPORT int WINAPI G2_SERVER_loader_config_message_init(g2_server_loader_config_message_pt message)
{
    return PROTOCOL_OK;
}

WINAPI_EXPORT uint16_t WINAPI G2_SERVER_loader_config_message_serialize(uint8_t *bytes, g2_server_loader_config_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->probeID, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(0xF800 , &bytes[pos]);
    pos += 2;
    g2_server_channel_power_pt power = NULL;
    
    for(uint8_t j=0; j<RELAY_SIZE; j++)
    {
        power = (g2_server_channel_power_pt)(&message->power[j][0]);
        for(uint8_t i=0; i< RELAY_CHANNEL_SIZE; i++)
        {
            write_uint8_t_BE(power->loaderType, &bytes[pos]);
            pos += 1;
            write_uint8_t_BE(power->phaseConfig, &bytes[pos]);
            pos += 1;
            write_uint8_t_BE(power->power, &bytes[pos]);
            pos += 1;
            write_uint8_t_BE(power->lossPhaseProtectEnable, &bytes[pos]);
            pos += 1;
            write_uint8_t_BE(power->overloadProtectEnable, &bytes[pos]);
            pos += 1;
            write_uint8_t_BE(power->ouvProtectEnable, &bytes[pos]);
            pos += 1;
            power++;
        }
    }
    return pos;
}


WINAPI_EXPORT uint16_t WINAPI G2_SERVER_loader_config_message_deserialize(uint8_t *bytes, g2_server_loader_config_message_pt message)
{
    uint16_t pos = 0;
    message->probeID = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->config = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    g2_server_channel_power_pt relay_channel_power = &(message->power[0][0]);
    for(uint8_t j=0; j<RELAY_SIZE; j++)
    {
        for(uint8_t i=0; i< RELAY_CHANNEL_SIZE; i++)
        {
            if(config_bit_is_set(message->config, (j * RELAY_SIZE) + (i + 1)))
            {
                relay_channel_power->loaderType = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
                relay_channel_power->phaseConfig = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
                relay_channel_power->power = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
                relay_channel_power->lossPhaseProtectEnable = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
                relay_channel_power->overloadProtectEnable = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
                relay_channel_power->ouvProtectEnable = read_uint8_t_BE(&bytes[pos]);
                pos += 1;
            }
            relay_channel_power++;
        }
    }
    return pos;
}


WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_warnning_message_init(g2_server_device_warnning_message_pt message)
{
    message->type = 0;
    message->content = 0;
    return PROTOCOL_OK;
}


WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_warnning_message_serialize(uint8_t *bytes, g2_server_device_warnning_message_pt message)
{
    uint16_t pos = 0;
    write_uint8_t_BE(message->type, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->content, &bytes[pos]);
    pos += 1;
    return pos;
}

