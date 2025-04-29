#include "app_gprs.h"

#include "app.h"
#include "bsp_protocol.h"
#include "app_network.h"
#include "app_log.h"
#include "bsp_power_key.h"

#define SIM_ICCID_LEN   (20)
static uint8_t s_simIccid[SIM_ICCID_LEN+5] = "00000000000000000000";
static HARDWARE_SEMAPHORE_TYPE_T s_simIccidSem = NULL;
static uint8_t s_manufacturer = UNKONW_MANUFACTURER;
static uint8_t s_getSimInfoFlow = IDLE_FLOW;
static APP_GPRS_Status_t s_gprsModelStatus = {0, 0, 0};

#define GPRS_MODEL_SET_NORMAL(model)    \
    model.status = 1;\
    model.errcnt = 0;\
    model.rstcnt = 0
#define GPRS_MODEL_CAN_RESET(model)     (model.rstcnt < 3 && model.errcnt >= 120)

static void SetGetSimInfoFlow(uint8_t flow)
{
    s_getSimInfoFlow = flow;
}

static void GprsTaskSemInit(void)
{
    if(NULL == s_simIccidSem)
    {
        s_simIccidSem = HARDWARE_CREATE_SEMAPHORE();
    }
}

static void AppGprsStatusInit(void)
{
    s_gprsModelStatus.status = -1;
    s_gprsModelStatus.errcnt = 0;
    s_gprsModelStatus.rstcnt = 0;
}

int APP_GPRS_Init(void)
{
    AppGprsStatusInit();
    SetGetSimInfoFlow(GET_SIM_ICCID_FLOW);
    BSP_USER_PROTOCOL_init();
}

int USER_PROTOCOL_WriteRoleMessageProcess(user_protocol_packet_pt packet)
{
    return APP_OK;
}

int USER_PROTOCOL_ReadSimIccidMessageProcess(user_protocol_packet_pt packet)
{
    uint8_t *pbuf = (uint8_t *)packet->parsed;
    memset(s_simIccid, 0, SIM_ICCID_LEN+5);
    memcpy(s_simIccid, pbuf, SIM_ICCID_LEN);
    uint8_t len = strlen(s_simIccid);
    if((len > 0) && (len <=20))
    {
        SetGetSimInfoFlow(GET_SIM_MANUFACTURER_FLOW);
    }
    else
    {
        SetGetSimInfoFlow(GET_SIM_ICCID_FLOW);
    }
    return APP_OK;
}

int USER_PROTOCOL_ReadSimManufacturerMessageProcess(user_protocol_packet_pt packet)
{
    uint8_t *pbuf = (uint8_t *)packet->parsed;
    s_manufacturer = pbuf[0];
    SetGetSimInfoFlow(IDLE_FLOW);
	GprsTaskSemInit();
    HARDWARE_GIVE_SEMAPHORE(s_simIccidSem);
    return APP_OK;
}


static void GetSimInfoProcess(uint8_t flow, user_protocol_packet_pt packet)
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
                SetGetSimInfoFlow(GET_SIM_ICCID_FLOW);
            }
            break;
        default:
            break;
    }
}

static int CheckGprsModelStatus(void)
{
    if(1 == s_gprsModelStatus.status)
    {
        return APP_OK;
    }
    else if(-1 == s_gprsModelStatus.status)
    {
        s_gprsModelStatus.errcnt += 1;
        if(s_gprsModelStatus.errcnt >= 120)
        {
            s_gprsModelStatus.errcnt = 120;
            if((s_gprsModelStatus.rstcnt >= 3) && (2 != s_gprsModelStatus.status))
            {
                s_gprsModelStatus.status = 2;
                POWER_GPRS_OFF();
                HARDWARE_OS_DELAY_MS(5000);
                POWER_GPRS_ON();
                APP_LOG_Debug("GPRS model ERROR!!!\r\n");
            }
        }
    }
    if(GPRS_MODEL_CAN_RESET(s_gprsModelStatus))
    {
        POWER_GPRS_OFF();
        HARDWARE_OS_DELAY_MS(5000);
        POWER_GPRS_ON();
        s_gprsModelStatus.rstcnt += 1;
        s_gprsModelStatus.errcnt = 0;
        s_gprsModelStatus.status = 0;
        APP_LOG_Debug("power off/on GPRS model\r\n");
    }
    return APP_ERROR;
}

void APP_GPRS_task_run(void *argument)
{
    int rc = 0;
    user_protocol_packet_t packet = {0};
    APP_GPRS_Init();
    for (;;)
    {
        USER_PROTOCOL_PACKET_init(&packet);
        rc = BSP_USER_PROTOCOL_PACKET_read(&packet);
        if (rc == PROTOCOL_OK)
        {
            GPRS_MODEL_SET_NORMAL(s_gprsModelStatus);
            if (BSP_USER_PROTOCOL_receive_packet(&packet) == PROTOCOL_NOT_SUPPORT)
            {
            }
        }
        else
        {
            if(s_gprsModelStatus.status <= 0)
            {
                s_gprsModelStatus.status = -1;
            }
        }
        GetSimInfoProcess(s_getSimInfoFlow, &packet);
        CheckGprsModelStatus();
        HARDWARE_OS_DELAY_MS(500);
    }
}


int G2_SERVER_ReadIccidMessageProcess(g2_server_packet_pt packet)
{
    g2_server_sim_iccid_message_t message;
    uint8_t iccid_len = 0;
    iccid_len = strlen(s_simIccid);
    if(iccid_len > 20)
    {
        iccid_len = 20;
    }
    message.len = iccid_len + 1;
    message.manufacturer = s_manufacturer;
    strcpy(message.iccid, s_simIccid);
    BSP_PROTOCOL_send_read_iccid_message(packet, &message);
    return PROTOCOL_OK;
}



int APP_GPRS_ReadIccidMessageProcess(void)
{
    int rc = APP_OK;
	if(NULL != s_simIccidSem)
    {
		HARDWARE_TAKE_SEMAPHORE(s_simIccidSem);
        g2_server_packet_t packet;
        G2_SERVER_PACKET_init(&packet);
        rc = G2_SERVER_ReadIccidMessageProcess(&packet);
        HARDWARE_GIVE_SEMAPHORE(s_simIccidSem);
        HARDWARE_DELETE_SEMAPHORE(s_simIccidSem);
    }
    return rc;
}
