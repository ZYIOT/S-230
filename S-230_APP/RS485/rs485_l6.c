#include "rs485_l6.h"

static uint16_t RS485_L6_status_message_deserialize(uint8_t *bytes, rs485_l6_status_pt message)
{
    uint16_t pos = 0;
    message->device_status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->data_status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->bump_status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->solenoid_status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->liquid_level_status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->release = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->readWait = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->read = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->charge = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->chargeWait = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
}

static int _parse_value_error(uint8_t error_code, void *ret, void *args)
{
    rs485_l6_status_pt l6_status = (rs485_l6_status_pt)ret;
    l6_status->error_code = error_code;
    l6_status->status = RS485_OK;
    l6_status->initialized = 1;
    if (l6_status->errorCount < 0xFF00)
    {
        l6_status->errorCount += 1;
    }
    return RS485_OK;
}

static int _parse_value(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    rs485_l6_status_pt l6_status = (rs485_l6_status_pt)ret;
    uint8_t last_data_status = l6_status->data_status;
    if(len != 20)
    {
        return RS485_OK;
    }
    RS485_L6_status_message_deserialize(buffer, l6_status); // 解析 L6 当前的状态 
    l6_status->error_code = 0;
    l6_status->status = RS485_OK;
    l6_status->initialized = 1;
    if (l6_status->device_status == 0)
    {
        l6_status->errorCount = 0;
        l6_status->reset_count = 0;
    }
    else
    {
        if (l6_status->errorCount < 0xFF00)
        {
            l6_status->errorCount += 1;
        }
    }
    if((last_data_status ==  0) && (l6_status->data_status == 1))   // L6 从<可获取数据>变为<不可获取数据>，即吹气后的等待状态 
    {
        l6_status->send_status = 1;
        l6_status->force_status = 0;		
        // if (l6_status->send_status < 0xFF00)
		// {	
        //     l6_status->send_status += 1;
        // }
    }
    return RS485_OK;
}

int RS485_L6_read(rs485_port_pt rs485, rs485_l6_status_pt l6_status)
{   
    int rc = RS485_read_register(rs485, RS485_L6_ADDRESS, 0, 10, l6_status, _parse_value, _parse_value_error, NULL);
    if (rc != RS485_OK)
    {
        l6_status->status = rc;
       // if (l6_status->errorCount < 0xFF00)
		if (l6_status->errorCount < 10)
        {
            l6_status->errorCount += 1;
        }
        return rc;
    }
	else
	{
		l6_status->errorCount=0;
		l6_status->reset_count=0;
        return RS485_OK;
	}  
}

int RS485_L6_set_register(rs485_port_pt rs485, uint16_t release, uint16_t readWait, uint16_t read, uint16_t charge, uint16_t chargeWait)
{
    uint8_t buffer[10] = {0};
    write_uint16_t_BE(release, &(buffer[0]));
    write_uint16_t_BE(readWait, &(buffer[2]));
    write_uint16_t_BE(read, &(buffer[4]));
    write_uint16_t_BE(charge, &(buffer[6]));
    write_uint16_t_BE(chargeWait, &(buffer[8]));
    return RS485_write_multiple_registers(rs485, RS485_L6_ADDRESS, 5, 5, buffer, NULL, NULL, NULL, NULL);
}

int RS485_L6_start(rs485_port_pt rs485)
{
    return RS485_write_register(rs485, RS485_L6_ADDRESS, 13, 1, NULL, NULL, NULL, NULL);
}
