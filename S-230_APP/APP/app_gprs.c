#include "app_gprs.h"

#include "app.h"
#include "bsp_protocol.h"
#include "app_network.h"
#include "app_log.h"
#include "bsp_power_key.h"

#define SIM_ICCID_LEN   (20)
static uint8_t sim_iccid[SIM_ICCID_LEN+5] = "00000000000000000000";
HARDWARE_SEMAPHORE_TYPE_T sim_iccid_sem = NULL;
static uint8_t manufacturer = UNKONW_MANUFACTURER;
static uint8_t get_sim_info_flow = IDLE_FLOW;
static app_gprs_status_t gprs_model_status = {0, 0, 0};

#define GPRS_MODEL_SET_NORMAL(model)    \
    model.status = 1;\
    model.errcnt = 0;\
    model.rstcnt = 0
#define GPRS_MODEL_CAN_RESET(model)     (model.rstcnt < 3 && model.errcnt >= 120)

static void _set_get_sim_info_flow(uint8_t flow)
{
    get_sim_info_flow = flow;
}

static void _gprs_task_sem_init(void)
{
    if(NULL == sim_iccid_sem)
    {
        sim_iccid_sem = HARDWARE_CREATE_SEMAPHORE();
    }
}

static void _app_gprs_status_init(void)
{
    gprs_model_status.status = -1;
    gprs_model_status.errcnt = 0;
    gprs_model_status.rstcnt = 0;
}

int APP_GPRS_init(void)
{
    _app_gprs_status_init();
    _set_get_sim_info_flow(GET_SIM_ICCID_FLOW);
    BSP_USER_PROTOCOL_init();
}

int USER_PROTOCOL_write_role_message_process(user_protocol_packet_pt packet)
{
    return APP_OK;
}

int USER_PROTOCOL_read_sim_iccid_message_process(user_protocol_packet_pt packet)
{
    uint8_t *pbuf = (uint8_t *)packet->parsed;
    memset(sim_iccid, 0, SIM_ICCID_LEN+5);
    memcpy(sim_iccid, pbuf, SIM_ICCID_LEN);
    uint8_t len = strlen(sim_iccid);
    if((len > 0) && (len <=20))
    {
        _set_get_sim_info_flow(GET_SIM_MANUFACTURER_FLOW);
    }
    else
    {
        _set_get_sim_info_flow(GET_SIM_ICCID_FLOW);
    }
    return APP_OK;
}

int USER_PROTOCOL_read_sim_manufacturer_message_process(user_protocol_packet_pt packet)
{
    uint8_t *pbuf = (uint8_t *)packet->parsed;
    manufacturer = pbuf[0];
    _set_get_sim_info_flow(IDLE_FLOW);
	_gprs_task_sem_init();
    HARDWARE_GIVE_SEMAPHORE(sim_iccid_sem);
    return APP_OK;
}


static void _get_sim_info_process(uint8_t flow, user_protocol_packet_pt packet)
{
    static uint8_t sim_info_connected_server_flag = 0;
    if(CONNECTED_STATUS_CONNECTED == GET_APP_SERVER_ONCE_CONNECTED())
    {
        sim_info_connected_server_flag |= 0x1;
    }
    else
    {
        sim_info_connected_server_flag = 0x0;
    }
    switch(flow)
    {
        case GET_SIM_ICCID_FLOW:
            if(0x1 == sim_info_connected_server_flag)
            {   
                sim_info_connected_server_flag |= 0x2;
                BSP_USER_PROTOCOL_send_read_sim_iccid_message(packet);
            }
            break;

        case GET_SIM_MANUFACTURER_FLOW:
            BSP_USER_PROTOCOL_send_read_sim_manufaturer_message(packet);
            break;
        
        case IDLE_FLOW:
            if(0x1 == sim_info_connected_server_flag)
            {
                _set_get_sim_info_flow(GET_SIM_ICCID_FLOW);
            }
            break;
        default:
            break;
    }
}

static int _check_gprs_model_status(void)
{
    if(1 == gprs_model_status.status)
    {
        return APP_OK;
    }
    else if(-1 == gprs_model_status.status)
    {
        gprs_model_status.errcnt += 1;
        if(gprs_model_status.errcnt >= 120)
        {
            gprs_model_status.errcnt = 120;
            if((gprs_model_status.rstcnt >= 3) && (2 != gprs_model_status.status))
            {
                gprs_model_status.status = 2;
                POWER_GPRS_OFF();
                HARDWARE_OS_DELAY_MS(5000);
                POWER_GPRS_ON();
                APP_LOG_debug("GPRS model ERROR!!!\r\n");
            }
        }
    }
    if(GPRS_MODEL_CAN_RESET(gprs_model_status))
    {
        POWER_GPRS_OFF();
        HARDWARE_OS_DELAY_MS(5000);
        POWER_GPRS_ON();
        gprs_model_status.rstcnt += 1;
        gprs_model_status.errcnt = 0;
        gprs_model_status.status = 0;
        APP_LOG_debug("power off/on GPRS model\r\n");
    }
    return APP_ERROR;
}

void APP_GPRS_task_run(void *argument)
{
    int rc = 0;
    user_protocol_packet_t packet = {0};
    APP_GPRS_init();
    for (;;)
    {
        USER_PROTOCOL_PACKET_init(&packet);
        rc = BSP_USER_PROTOCOL_PACKET_read(&packet);
        if (rc == PROTOCOL_OK)
        {
            GPRS_MODEL_SET_NORMAL(gprs_model_status);
            if (BSP_USER_PROTOCOL_receive_packet(&packet) == PROTOCOL_NOT_SUPPORT)
            {
            }
        }
        else
        {
            if(gprs_model_status.status <= 0)
            {
                gprs_model_status.status = -1;
            }
        }
        _get_sim_info_process(get_sim_info_flow, &packet);
        _check_gprs_model_status();
        HARDWARE_OS_DELAY_MS(500);
    }
}


int G2_SERVER_read_iccid_message_process(g2_server_packet_pt packet)
{
    g2_server_sim_iccid_message_t message;
    uint8_t iccid_len = 0;
    iccid_len = strlen(sim_iccid);
    if(iccid_len > 20)
    {
        iccid_len = 20;
    }
    message.len = iccid_len + 1;
    message.manufacturer = manufacturer;
    strcpy(message.iccid, sim_iccid);
    BSP_PROTOCOL_send_read_iccid_message(packet, &message);
    return PROTOCOL_OK;
}



int APP_GPRS_read_iccid_message_process(void)
{
    int rc = APP_OK;
	if(NULL != sim_iccid_sem)
    {
		HARDWARE_TAKE_SEMAPHORE(sim_iccid_sem);
        g2_server_packet_t packet;
        G2_SERVER_PACKET_init(&packet);
        rc = G2_SERVER_read_iccid_message_process(&packet);
        HARDWARE_GIVE_SEMAPHORE(sim_iccid_sem);
        HARDWARE_DELETE_SEMAPHORE(sim_iccid_sem);
    }
    return rc;
}
