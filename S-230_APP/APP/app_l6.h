#ifndef __APP_L6_H
#define __APP_L6_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "rs485_l6.h"
#include "bsp_rs485.h"
#include "bsp_protocol.h"

extern rs485_l6_status_t rs485_l6_status;

#define APP_L6_initialized()        (rs485_l6_status.initialized == 1)
#define APP_L6_data_ready()         (rs485_l6_status.data_status == 0)
#define APP_L6_send_data_ready()    (rs485_l6_status.send_status == 1)	///	

#define APP_L6_force_saturability_ready() (rs485_l6_status.saturability_status == 1)	///	

#define APP_L6_is_broken()  (rs485_l6_status.reset_count >= 3 && rs485_l6_status.error_count >= 10)
#define APP_L6_force_read() (APP_L6_is_broken() || rs485_l6_status.force_status == 1)
#define APP_L6_can_reset()  (rs485_l6_status.reset_count < 3 && rs485_l6_status.error_count >= 10)

#define APP_L6_read()   RS485_L6_read(&rs485_port, &rs485_l6_status)
#define APP_L6_start()  RS485_L6_start(&rs485_port)
#define APP_L6_set_register(release, read_wait, read, charge, charge_wait) RS485_L6_set_register(&rs485_port, release, read_wait, read, charge, charge_wait);

#define APP_L6_release_time()       (app_config_l6.release)
#define APP_L6_read_wait_time()     (app_config_l6.read_wait + APP_L6_release_time())
#define APP_L6_read_time()          (app_config_l6.read + APP_L6_read_wait_time())
#define APP_L6_charge_time()        (app_config_l6.charge + APP_L6_read_time())	
#define APP_L6_charge_wait_time()   (app_config_l6.charge_wait * 60 + APP_L6_charge_time())

int APP_L6_init(void);
int APP_L6_status_pack(g2_server_l6_status_message_pt message);
int APP_L6_settings_write(uint8_t probe_id,g2_server_l6_settings_message_pt message);
void APP_L6_task_run(void *argument);


#ifdef __cplusplus
}
#endif

#endif
