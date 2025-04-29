#include "drivers/sensors/sensors.h"
#include "bsp_log.h"

static rs485_sensor_driver_pt sensors_driver[RS485_DRIVER_SENSOR_MAX_SIZE] = {NULL};    // 传感器driver列表 
static int sensor_total_drv = 0;    // 注册的传感器总数 

int RS485_DRIVER_regsiter(rs485_sensor_driver_pt sensors_drv)
{
    if (sensor_total_drv < RS485_DRIVER_SENSOR_MAX_SIZE && sensors_drv != NULL && sensors_drv->action != NULL)
    {
        sensors_driver[sensor_total_drv] = sensors_drv;
        sensor_total_drv++;
        return RS485_OK;
    }
    return RS485_ERROR;
}

// 注册可能使用到的(同driver)传感器的驱动 
int RS485_DRIVER_init(void)
{
#ifdef ENABLE_RS485_DRIVER_SENSORS_ZYIOT_DOY100
    RS485_DRIVER_zyiot_doy100_init();
#endif
#ifdef ENABLE_RS485_DRIVER_SENSORS_ZYIOT_PH201
    RS485_DRIVER_zyiot_ph201_init();
#endif
#ifdef ENABLE_RS485_DRIVER_SENSORS_PROBEST_DOY121
    RS485_DRIVER_probest_doy121_init();
#endif
#ifdef ENABLE_RS485_DRIVER_SENSORS_SZKLT_PH230
    RS485_DRIVER_szklt_ph230_init();
#endif
#ifdef ENABLE_RS485_DRIVER_SENSORS_FIVE_PHASE_DOY170
    RS485_DRIVER_five_phase_doy170_init();
#endif
#ifdef ENABLE_RS485_DIRVER_SENSOR_ANBULEILA_WL991
    RS485_DRIVER_level_anbuleila_init();
#endif
#ifdef ENABLE_RS485_DIRVER_SENSORS_CHEMINS_PH260
    RS485_DRIVER_chemins_ph260_init();
#endif
#ifdef ENABLE_RS485_DRIVER_SENSOR_DO_DEFAULT
    RS485_DRIVER_zyiot_do_default_init();
#endif
#ifdef ENABLE_RS485_DRIVER_SENSORS_CHEMINS_NHN760
    RS485_DRIVER_chemins_nhn760_init();
#endif
#ifdef ENABLE_RS485_DRIVER_SENSOR_XT_DO 
    RS485_DRIVER_xiantao_doy181_init();
#endif
#ifdef ENABLE_RS485_DRIVER_SENSOR_XT_TMP_PH_NH3
    RS485_DRIVER_xt_ph291_init();
#endif

    return RS485_OK;
}

// 通过传感器 ID 的 check_sensor 盲读传感器数据 来匹配驱动 
static int _scan_by_id_and_mode(rs485_sensor_pt sensor, rs485_recognition_mode_t recognition_mode)
{
    int rc = RS485_ERROR;
    rs485_sensor_driver_pt sensors_drv = sensors_driver[0];
    if (sensor->id == 0)
    {
        return RS485_ERROR;
    }
    for (int i = 0; i < sensor_total_drv; i++)
    {
        rc = RS485_ERROR;
        sensors_drv = sensors_driver[i];
        if (sensors_drv != NULL &&
            (sensors_drv->id == sensor->id) &&
            (sensors_drv->action != NULL) &&
            (sensors_drv->action->recognition_mode == recognition_mode) &&
            (sensors_drv->action->check_sensor) &&
            ((rc = sensors_drv->action->check_sensor(sensor, sensors_drv)) == RS485_OK))
        {   // 若存在2种传感器都包含了 check_sensor 方法(譬如DOY121和DOY170)在该判定方法就可能匹配错误 
            sensor->manufacturer = sensors_drv->manufacturer;
            sensor->model = sensors_drv->model;
            sensor->protocol = sensors_drv->protocol;
            // 更新 dev 匹配的 PN 
            sensor->sensor_change = strcmp(sensor->PN, sensors_drv->action->PN);
            if(0 != sensor->sensor_change)
            {
                BSP_LOG_debug("sensor change\r\n");
            }
            memset(sensor->PN, 0, 8);
            strcpy(sensor->PN, sensors_drv->action->PN);
            sensor->action = sensors_drv->action;
            BSP_LOG_debug("match id <%s>\r\n", sensor->PN);
            return RS485_OK;
        }
        if (rc == RS485_READ_ERROR) //如果传感器没有任何返回。就不再尝试
        {
            return RS485_READ_ERROR;
        }
    }
    return rc;
}

static int _set_do_ph_sensor_driver_default(rs485_sensor_pt sensor)
{
    if (sensor->id == 0)
    {
        return RS485_ERROR;
    }
    uint8_t index = 0;
    uint8_t modelId = 0;
    rs485_sensor_driver_pt sensors_drv = NULL;
    if(RS485_DRIVER_SENSOR_ID_DO == sensor->id) // 如果是 DO 传感器，则默认传感器型号为 DO-Y100 
    {
#ifdef ENABLE_DO_SENSOR_DEFAULT   
        modelId = RS485_SENSOR_MODEL_ID_DOY170;
#else
        modelId = RS485_SENSOR_MODEL_ID_DOY100;
#endif        
    }
    else if(RS485_DRIVER_SENSOR_ID_PH == sensor->id) // 如果是 PH 传感器，则默认传感器型号为 PH-201 
    {
        modelId = RS485_SENSOR_MODEL_ID_PH201;
    }
    for(index = 0; index < sensor_total_drv; index++)
    {
        if(modelId == sensors_driver[index]->model)
        {
            break;
        }
    }
    if((sensor_total_drv == index) || (0 == modelId))
    {
        return RS485_ERROR;
    }
    sensors_drv = sensors_driver[index];
    {
        sensor->manufacturer = sensors_drv->manufacturer;
        sensor->model = sensors_drv->model;
        sensor->protocol = sensors_drv->protocol;
        // 更新 dev 匹配的 PN 
        sensor->sensor_change = strcmp(sensor->PN, sensors_drv->action->PN);
        if(0 != sensor->sensor_change)
        {
            BSP_LOG_debug("sensor change\r\n");
        }
        memset(sensor->PN, 0, 8);
        strcpy(sensor->PN, sensors_drv->action->PN);
        sensor->action = sensors_drv->action;
        BSP_LOG_debug("DO/PH Match default <%s>\r\n", sensor->PN);
        return RS485_OK;
    }
}


// 通过传感器固定位置的PN比较
int RS485_DRIVER_match(rs485_sensor_pt sensor)
{
    if (sensor->id == 0)
    {
        return RS485_ERROR;
    }
    // 先尝试从固定地址读取PN值 
    uint8_t buffer[RS485_SENSOR_MAX_PN_LEN] = {0};
    int rc = RS485_read_register(sensor->port, sensor->id, COMMON_PN_REG, 4, buffer, RS485_DRIVER_copy_buffer, NULL, NULL);
    // printf("match rc = %d, %d\r\n", rc, strlen((char *)buffer));
    RS485_CHECK_RC(rc);
    int len = strlen((char *)buffer);
    if (len <= 4) //我们定义的PN长度都大于4个字符
    {
        return RS485_ERROR;
    }
    len--;
    rs485_sensor_driver_pt sensors_drv = sensors_driver[0];
    for (int i = 0; i < sensor_total_drv; i++)
    {
        sensors_drv = sensors_driver[i];
        if (NULL != sensors_drv &&
            sensors_drv->id == sensor->id &&
            NULL != sensors_drv->action &&
            sensors_drv->action->recognition_mode == RECOGNITION_MODE_AUTO &&
            NULL != sensors_drv->action->match_pn && 
            0 == sensors_drv->action->match_pn(sensor, (char *)buffer))
        {
            sensor->manufacturer = sensors_drv->manufacturer;
            sensor->model = sensors_drv->model;
            sensor->protocol = sensors_drv->protocol;
            // 更新drv中对应的PN 
            strncpy(sensors_drv->action->PN, (char *)buffer, RS485_SENSOR_MAX_PN_LEN);
            sensor->sensor_change = strcmp(sensor->PN, sensors_drv->action->PN);
            if(0 != sensor->sensor_change)
            {
                BSP_LOG_debug("sensor change\r\n");
            }
            memset(sensor->PN, 0, RS485_SENSOR_MAX_PN_LEN);
            strcpy(sensor->PN, sensors_drv->action->PN);
            sensor->action = sensors_drv->action;
            BSP_LOG_debug("Match sensor <%s>\r\n", sensor->PN);
            return RS485_OK;
        }
    }
    return RS485_ERROR;
}

// 通过盲读来匹配(非中易协议的)传感器
int RS485_DRIVER_scan(rs485_sensor_pt sensor)
{
    int rc = 0;
    if (sensor->id == 0)
    {
        return RS485_ERROR;
    }

    rs485_recognition_mode_t recognition_modes[] = {
        RECOGNITION_MODE_AUTO,
        RECOGNITION_MODE_COMPATIBLE,
    };
    for (int i = 0; i < 1; i++)
    {
        rc = _scan_by_id_and_mode(sensor, recognition_modes[i]);
        // printf("scan rc = %d\r\n", rc);
        if (rc == RS485_READ_ERROR) //如果传感器没有任何返回。就不再尝试
        {
            return RS485_READ_ERROR;
        } 
        if (rc == RS485_OK)
        {
            return RS485_OK;
        }
    }
    return RS485_ERROR;
}

// 通过传感器的PN来匹配 
int RS485_DRIVER_match_by_pn(rs485_sensor_pt sensor, char *pn)
{
    if (sensor->id == 0)
    {
        return RS485_ERROR;
    }
    rs485_sensor_driver_pt sensors_drv = sensors_driver[0];
    for (int i = 0; i < sensor_total_drv; i++)
    {
        sensors_drv = sensors_driver[i];
        if (sensors_drv != NULL &&
            sensors_drv->id == sensor->id &&
            sensors_drv->action->recognition_mode == RECOGNITION_MODE_AUTO &&
            strncmp(sensors_drv->action->PN, (char *)pn, strlen(sensors_drv->action->PN)) == 0)
        {
            sensor->manufacturer = sensors_drv->manufacturer;
            sensor->model = sensors_drv->model;
            sensor->protocol = sensors_drv->protocol;
            sensor->sensor_change = strcmp(sensor->PN, sensors_drv->action->PN);
            if(0 != sensor->sensor_change)
            {
                BSP_LOG_debug("sensor change\r\n");
            }
            memset(sensor->PN, 0, 8);
            strcpy(sensor->PN, sensors_drv->action->PN);
            sensor->action = sensors_drv->action;
            BSP_LOG_debug("Match PN <%s>\r\n", sensor->PN);
            return RS485_OK;
        }
    }
    return RS485_ERROR;
}

// 通过传感器的id、model来匹配 
int RS485_DRIVER_match_by_model(rs485_sensor_pt sensor, uint8_t theory)
{
    uint8_t model = sensor->model;
    if (sensor->id == 0)
    {
        return RS485_ERROR;
    }
    rs485_sensor_driver_pt sensors_drv = sensors_driver[0];
    for (int i = 0; i < sensor_total_drv; i++)
    {
        sensors_drv = sensors_driver[i];
        if (sensors_drv != NULL &&
            sensors_drv->id == sensor->id &&
            sensors_drv->action->recognition_mode == RECOGNITION_MODE_AUTO &&
            NULL != sensors_drv->action->match_model && 
            0 == sensors_drv->action->match_model(sensor, model, theory))
        {
            sensor->manufacturer = sensors_drv->manufacturer;
            sensor->model = sensors_drv->model;
            sensor->protocol = sensors_drv->protocol;
            sensor->action = sensors_drv->action;
            memset(sensor->PN, 0, RS485_SENSOR_MAX_PN_LEN);
            strcpy(sensor->PN, sensors_drv->action->PN);
            BSP_LOG_debug("Update model <%s>\r\n", sensor->PN);
            // if((RS485_SENSOR_MODEL_ID_DOY121 != model) && (RS485_SENSOR_MODEL_ID_WL991 != model))   // DOY121/WL991 具有独享一个 model 值
            // {
            //     // 先尝试从固定地址读取PN值 
            //     uint8_t buffer[RS485_SENSOR_MAX_PN_LEN] = {0};
            //     int rc = RS485_read_register(sensor->port, sensor->id, COMMON_PN_REG, 4, buffer, RS485_DRIVER_copy_buffer, NULL, NULL);
            //     RS485_CHECK_RC(rc);
            //     int len = strlen((char *)buffer);
            //     if (len <= 4) //我们定义的PN长度都大于4个字符
            //     {
            //         return RS485_ERROR;
            //     }
            //     if(0 != strncmp(sensors_drv->action->pn_match_name, buffer, strlen(sensors_drv->action->pn_match_name)))
            //     {
            //         return RS485_ERROR;
            //     }
            //     strncpy(sensors_drv->action->PN, (char *)buffer, RS485_SENSOR_MAX_PN_LEN);
            //     memset(sensor->PN, 0, RS485_SENSOR_MAX_PN_LEN);
            //     strcpy(sensor->PN, sensors_drv->action->PN);
            //     BSP_LOG_debug("Match model <%s>\r\n", sensor->PN);
            // }
            return RS485_OK;
        }
    }
    // 如果通过 model 也无法匹配 
    {
        if((RS485_DRIVER_SENSOR_ID_DO == sensor->id) 
        || (RS485_DRIVER_SENSOR_ID_PH == sensor->id))
        {
            _set_do_ph_sensor_driver_default(sensor);
        }
    }
    return RS485_ERROR;
}

int RS485_DRIVER_copy_buffer(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    memcpy((uint8_t *)ret, buffer, len);
    return RS485_OK;
}

// // 已经匹配兼容模式，如果兼容模式的厂商或型号与设置的不匹配，则不成功
// int RS485_DRIVER_check_sensor_manual(rs485_sensor_pt rs485, rs485_sensor_driver_pt sensors_drv)
// {
//     if (rs485->action == NULL)
//     {
//         return RS485_ERROR;
//     }
//     if (sensors_drv != NULL &&
//         sensors_drv->id == rs485->id &&
//         rs485->manufacturer_config > 0 &&
//         sensors_drv->manufacturer == rs485->manufacturer_config &&
//         sensors_drv->manufacturer == rs485->manufacturer &&
//         rs485->model_config > 0 &&
//         sensors_drv->model == rs485->model_config &&
//         sensors_drv->model == rs485->model &&
//         (rs485->protocol_config == 0 || (sensors_drv->protocol == rs485->protocol_config && sensors_drv->protocol == rs485->protocol)))
//     {
//         rs485->PN = sensors_drv->action->name;
//         return RS485_OK;
//     }
//     return RS485_ERROR;
// }
