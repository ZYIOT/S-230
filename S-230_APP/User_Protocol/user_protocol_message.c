
#include "user_protocol_message.h"

int USER_PROTOCOL_role_message_init(uint8_t *role)
{
    return PROTOCOL_OK;
}

uint16_t USER_PROTOCOL_role_message_serialize(uint8_t *buf, uint8_t *role)
{
    uint16_t pos = 0;
    return pos;
}

uint16_t USER_PROTOCOL_role_message_deserialize(uint8_t *buf, uint8_t *role)
{
    uint16_t pos = 0;
    return pos;
}


int  USER_PROTOCOL_sim_iccid_message_init(uint8_t *sim_iccid)
{
    memset(sim_iccid, 0, 20);
    return PROTOCOL_OK;
}

uint16_t USER_PROTOCOL_sim_iccid_message_serialize(uint8_t *buf, uint8_t *sim_iccid)
{
    uint16_t pos = 0;
    memcpy(buf, sim_iccid, 20);
    pos += 20;
    return pos;
}


uint16_t USER_PROTOCOL_sim_iccid_message_deserialize(uint8_t *buf, uint8_t *sim_iccid)
{
    uint16_t pos = 0;
    memcpy(sim_iccid, buf, 20);
    pos += 20;
    return pos;
}



int  USER_PROTOCOL_sim_manufacturer_message_init(uint8_t *manufacturer)
{
    *manufacturer = 0;
    return PROTOCOL_OK;
}

uint16_t USER_PROTOCOL_sim_manufacturer_message_serialize(uint8_t *buf, uint8_t *manufacturer)
{
    uint16_t pos = 0;
    buf[0] = *manufacturer;
    pos += 1;
    return pos;
}


uint16_t USER_PROTOCOL_sim_manufacturer_message_deserialize(uint8_t *buf, uint8_t *manufacturer)
{
    uint16_t pos = 0;
    *manufacturer = buf[0];
    pos += 1;
    return pos;
}


int USER_PROTOCOL_device_id_message_init(uint8_t *device_id)
{
    return PROTOCOL_OK;
}

uint16_t USER_PROTOCOL_device_id_message_serialize(uint8_t *buf, uint8_t *device_id)
{
    uint16_t pos = 0;
    memcpy(buf, device_id, 4);
    pos += 4;
    return pos;
}

uint16_t USER_PROTOCOL_device_id_message_deserialize(uint8_t *buf, uint8_t *device_id)
{
    return PROTOCOL_OK;
}

