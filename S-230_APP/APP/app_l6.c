
#include "app_l6.h"
#include "bsp_rs485.h"
#include "adc.h"
#include "bsp_l6.h"
#include "bsp_wdg.h"
#include "app_log.h"

rs485_l6_status_t rs485_l6_status;

int APP_L6_Init(void)
{
    rs485_l6_status.device_status = 0;       // 设备状态 
    rs485_l6_status.data_status = 1;         // 获取数据状态 
    rs485_l6_status.bump_status = 0;         // 气泵状态 
    rs485_l6_status.solenoid_status = 0;     // 电磁阀状态 
    rs485_l6_status.liquid_level_status = 0; // 液位传感器状态 
    rs485_l6_status.release = 0;             // 泄气时间 
    rs485_l6_status.readWait = 0;           // 入水后多长时间允许读取数据（传感器在水中等待时长） 
    rs485_l6_status.read = 0;                // 读取数据持续时间 
    rs485_l6_status.charge = 0;              // 充气时间 
    rs485_l6_status.chargeWait = 0;         // 气吹自动等待时长(传感器在空气中时长) 
    rs485_l6_status.status = RS485_ERROR;    // 数据查询的状态 
    rs485_l6_status.error_code = 0;          // 485 通信的错误状态 
    rs485_l6_status.initialized = 0;         // 是否初始化 
    rs485_l6_status.errorCount = 0;
	rs485_l6_status.send_status = 0;
	rs485_l6_status.force_status = 0;  
	
	rs485_l6_status.release_status = 0;	// 泄气状态 
	rs485_l6_status.charge_status = 0;  // 充气状态 
	rs485_l6_status.release_start = 0; 	// 泄气状态 
	rs485_l6_status.charge_start = 0;  	// 充气状态 
	
	rs485_l6_status.force_status = 0;
		
	rs485_l6_status.start_status = 1;
		
	rs485_l6_status.saturability_status = 0;
		
	rs485_l6_status.saturability_alet_status = 0;
	rs485_l6_status.saturability_data = 0;
	rs485_l6_status.saturability_count = 0;

	rs485_l6_status.air_alet_status = 0;
	
	memset(rs485_l6_status.saturability, 0, Saturability_max_capacity);
    return APP_OK;
}

static int L6Before(void)
{
	BSP_AIRPUMP_on();
	HARDWARE_OS_DELAY_MS(10);
	BSP_SOLENOID1_off();
	HARDWARE_OS_DELAY_MS(10);
	BSP_SOLENOID2_on();
    return APP_OK;
}

static int L6Release(void)
{
    BSP_AIRPUMP_off();
	BSP_SOLENOID1_on();
	HARDWARE_OS_DELAY_MS(10);
	BSP_SOLENOID2_on();
	rs485_l6_status.release_status = 1;
	rs485_l6_status.charge_status = 0;
    return APP_OK;
}

static int L6Wait(void)
{
    BSP_AIRPUMP_off();
	HARDWARE_OS_DELAY_MS(10);
	BSP_SOLENOID1_off();
	HARDWARE_OS_DELAY_MS(10);
	BSP_SOLENOID2_off();
	rs485_l6_status.release_status = 0; // 泄气状态 
	rs485_l6_status.charge_status = 0;  // 充气状态 
    return APP_OK;
}

static int L6Charge(void)
{
	if(rs485_l6_status.solenoid_status)
	{
		BSP_AIRPUMP_off();
	}
	else 
	{
		BSP_AIRPUMP_on();
	}
	HARDWARE_OS_DELAY_MS(10);
	BSP_SOLENOID1_off();
	HARDWARE_OS_DELAY_MS(10);
	BSP_SOLENOID2_on();
	rs485_l6_status.release_status = 0;
	rs485_l6_status.charge_status = 1;
    return APP_OK;
}

static int L6ChargeCheck(void *ret)
{
	rs485_l6_status_pt l6_check_status = (rs485_l6_status_pt)ret;
	uint16_t PUMP_vaul = 0;
	rs485_l6_status.charge_start = 0;		
	PUMP_vaul= Get_Adc_Average(OILPUMP_ADC_CHANNEL, 1000);
	APP_LOG_Debug("pump ad: %d\r\n", PUMP_vaul);
	if((PUMP_vaul>MAX_PUMP_num)||(PUMP_vaul<MIN_PUMP_num)) 
	{
		l6_check_status->bump_status = 1;
	}
	else l6_check_status->bump_status = 0;

    return APP_OK;
}

static int L6ReleaseCheck(void *ret)
{    
	rs485_l6_status_pt l6_check_status = (rs485_l6_status_pt)ret;
	uint16_t solenoid_vaul1,solenoid_vaul2 = 0;
	rs485_l6_status.release_start = 0;	
	solenoid_vaul1= Get_Adc_Average(Solenoid1_ADC_CHANNEL, 1000);
	solenoid_vaul2= Get_Adc_Average(Solenoid2_ADC_CHANNEL, 1000);	
	APP_LOG_Debug("fm1 ad: %d\r\n", solenoid_vaul1);
	APP_LOG_Debug("fm2 ad: %d\r\n", solenoid_vaul2);
	if((solenoid_vaul1>MAX_SOLENOID_num)||(solenoid_vaul1<MIN_SOLENOID_num)||(solenoid_vaul2>MAX_SOLENOID_num)||(solenoid_vaul2<MIN_SOLENOID_num)) 
	{
		l6_check_status->solenoid_status = 1;
	}
	else l6_check_status->solenoid_status = 0;

    return APP_OK;
}

// 新版电路板丝印上，P5为电磁阀2，P6为电磁阀1，与原理图中标注相反。
void APP_L6_task_run(void *argument)
{
	uint8_t last_data_status;
	uint8_t last_release_status;
	uint8_t last_charge_status ;
	static uint8_t last_pump_status = 1;
	static uint32_t l6_work_tick = 0;
	BSP_BUZZ_off();
	L6Before();
	HARDWARE_OS_DELAY_MS(1000);
	l6_work_tick = HARDWARE_GET_TICK();
    for (;;)
    {
		if(1 == last_pump_status)
		{
			if(HARDWARE_GET_TICK() - l6_work_tick < 9 * 1000)
			{
				L6Before();
			}
			else
			{
				last_pump_status = 0;
				l6_work_tick = 0;
			}
		}
		else
		{
			if(rs485_l6_status.start_status == 1)
			{
				l6_work_tick = HARDWARE_GET_TICK();
				rs485_l6_status.start_status = 0;				 
			}		
			if(HARDWARE_GET_TICK() - l6_work_tick < APP_L6_ReleaseTime() * 1000)
			{
				last_release_status = rs485_l6_status.release_status;
				L6Release();	
				if((last_release_status == 0) && (rs485_l6_status.release_status == 1))
				{
					rs485_l6_status.release_start = 1;
				}			
				APP_LOG_Debug("air blow:release\r\n"); 
			}
			else 
			{
				if(HARDWARE_GET_TICK() - l6_work_tick < APP_L6_ReadTime() * 1000)
				{		
					L6Wait();
					rs485_l6_status.data_status = 0;	
					APP_LOG_Debug("air blow:release wait\r\n"); 	
				}
				else
				{						
					if(HARDWARE_GET_TICK() - l6_work_tick < APP_L6_ChargeTime() * 1000)
					{
						last_charge_status = rs485_l6_status.charge_status;	
						last_data_status = rs485_l6_status.data_status;
						rs485_l6_status.data_status = 1;	
						if((last_data_status ==  0) && (rs485_l6_status.data_status == 1))
						{ 	
							rs485_l6_status.send_status =1;
						}								
						L6Charge();
						if((last_charge_status == 0) && (rs485_l6_status.charge_status == 1))
						{
							rs485_l6_status.charge_start = 1;
						}	
						APP_LOG_Debug("air blow:charge\r\n");
					}
					else 
					{
						if(HARDWARE_GET_TICK() - l6_work_tick < APP_L6_ChargeWaitTime() * 1000)
						{	
							L6Wait();
							APP_LOG_Debug("air blow:charge wait\r\n");	
						}
						else 
						{
							l6_work_tick = HARDWARE_GET_TICK();
							rs485_l6_status.force_status = 0;
						}
					}
				}
			}		
		}
		if(rs485_l6_status.charge_start == 1)
		{
			L6ChargeCheck(&rs485_l6_status);		
		}		
		if(rs485_l6_status.release_start == 1)
		{
			L6ReleaseCheck(&rs485_l6_status);	
		}
		if(rs485_l6_status.solenoid_status||rs485_l6_status.bump_status)
		{
			rs485_l6_status.device_status = 1;
		}
		else rs485_l6_status.device_status = 0;
		HARDWARE_OS_DELAY_MS(1000);
	}
}


int APP_L6_StatusPack(g2_server_l6_status_message_pt message)
{
#ifndef SUPPORT_L6_ENABLE
    if (rs485_l6_status.status == RS485_OK)
#endif	
    {
        message->config = 0xf800;
        message->device_status = rs485_l6_status.device_status;
        message->data_status = rs485_l6_status.data_status;
        message->bump_status = rs485_l6_status.bump_status;
        message->solenoid_status = rs485_l6_status.solenoid_status;
        message->liquid_level_status = rs485_l6_status.liquid_level_status & 0xff;
    }
    return APP_OK;
}

int APP_L6_SettingsWrite(uint8_t probeID, g2_server_l6_settings_message_pt message)
{
	g_appConfigL6.release = message->release;
	g_appConfigL6.readWait = message->readWait;
	g_appConfigL6.read = message->read;
	g_appConfigL6.charge = message->charge;
	g_appConfigL6.chargeWait = message->chargeWait;
	int rc = APP_CONFIG_L6Write(probeID - 1, &g_appConfigL6);
	APP_CHECK_RC(rc)

	return APP_OK;
}

