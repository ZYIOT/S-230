#ifndef __BSP_WRAPPER_H
#define __BSP_WRAPPER_H

#ifdef __cplusplus
extern "C"
{
#endif


#define APP_FIRMWARE_MAJOR 2
#define APP_FIRMWARE_MINOR 0
#define APP_FIRMWARE_REVISION 0

#define APP_HARDWARE_MAJOR 1
#define APP_HARDWARE_MINOR 0
#define APP_HARDWARE_REVISION 0


#define RELAY_SIZE 1
#define PROBE_SIZE 1
#define PROBE_SENSOR_SIZE 6
#define INDICATOR_SIZE 8
#define LCD12864_ENABLE 1
#define RELAY_CHANNEL_SIZE 5
#define PN_CODE "Y-510"
#define SN_CODE 1109

  typedef enum
  {
    HAL_OK = 0x00U,
    HAL_ERROR = 0x01U,
    HAL_BUSY = 0x02U,
    HAL_TIMEOUT = 0x03U
  } HAL_StatusTypeDef;

#define HARDWARE_GPIO_WRITE_PIN(port, pin, state)
#define HARDWARE_GPIO_WRITE_PIN_H(port, pin)
#define HARDWARE_GPIO_WRITE_PIN_L(port, pin)
#define HARDWARE_GPIO_READ_PIN(port, pin)
#define HARDWARE_GPIO_SET_INPUT(port, pin)
#define HARDWARE_GPIO_SET_OUTPUT(port, pin)

#define HARDWARE_SEMAPHORE_TYPE_T int
#define HARDWARE_CREATE_SEMAPHORE() 0
#define HARDWARE_TAKE_SEMAPHORE(sem)
#define HARDWARE_GIVE_SEMAPHORE(sem)

#define HARDWARE_OS_DELAY_MS(ms)
#define HARDWARE_HAL_DELAY_MS(ms)
#define HARDWARE_HAL_DELAY_US(us)
#define HARDWARE_GET_TICK() 0

#include "app.h"
#ifdef __cplusplus
}
#endif
#endif
