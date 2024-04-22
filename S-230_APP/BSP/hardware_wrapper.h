#ifndef __BSP_WRAPPER_H
#define __BSP_WRAPPER_H

#ifdef __cplusplus
extern "C"
{
#endif
    #include "dwt_delay.h"
    #include "cmsis_os2.h"
    #include "FreeRTOS.h"
    #include "semphr.h"
    #include "task.h"
    #include "main.h"
    #include "bsp_gpio.h"

    #define HARDWARE_GPIO_WRITE_PIN(port, pin, state) HAL_GPIO_WritePin(port, pin, (state) > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET)
    #define HARDWARE_GPIO_WRITE_PIN_H(port, pin) HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET)
    #define HARDWARE_GPIO_WRITE_PIN_L(port, pin) HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET)
    #define HARDWARE_GPIO_READ_PIN(port, pin) HAL_GPIO_ReadPin(port, pin)
    #define HARDWARE_GPIO_SET_INPUT(port, pin) BSP_GPIO_set_pin_as_input(port, pin)
    #define HARDWARE_GPIO_SET_OUTPUT(port, pin) BSP_GPIO_set_pin_as_output(port, pin)

    #define HARDWARE_SEMAPHORE_TYPE_T SemaphoreHandle_t
    #define HARDWARE_CREATE_SEMAPHORE() xSemaphoreCreateMutex()
    #define HARDWARE_TAKE_SEMAPHORE(sem)            \
        while (xSemaphoreTake(sem, 10) != pdTRUE)   \
        {                                           \
            osDelay(10);                            \
        }

    // timeout 设置为 100*x ms 
    #define HARDWARE_TAKE_SEMAPHORE_TIMEOUT(sem, timeout)   \
        rc = APP_OK;                                        \
        sem_tick_cnt = 0;                                   \
        while(xSemaphoreTake(sem, 10) != pdTRUE)            \
        {                                                   \
            osDelay(90);                                    \
            sem_tick_cnt += 100;                            \
            if(sem_tick_cnt >= timeout)                     \
            {                                               \
                rc = APP_ERROR;                             \
                break;                                      \
            }                                               \
        }                                                   \

    #define HARDWARE_GIVE_SEMAPHORE(sem)    \
        xSemaphoreGive(sem)

    #define HARDWARE_DELETE_SEMAPHORE(sem)  \
        vSemaphoreDelete(sem);              \
        sem = NULL


    #if 0
        #define HARDWARE_TAKE_NOTIFY(timeout)   ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(timeout))
        #define HARDWARE_WAIT_NOTIFY()          ulTaskNotifyTake(pdTRUE, portMAX_DELAY)
    #else    
        #define HARDWARE_TAKE_NOTIFY(timeout)   ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(timeout))
        #define HARDWARE_WAIT_NOTIFY()          ulTaskNotifyTake(pdFALSE, portMAX_DELAY)
    #endif    
        #define HARDWARE_GIVE_NOTIFY(task)      xTaskNotifyGive((TaskHandle_t)task)

    #define HARDWARE_OS_DELAY_MS(ms) osDelay(ms)
    #define HARDWARE_HAL_DELAY_MS(ms) HAL_Delay(ms)
    #define HARDWARE_HAL_DELAY_US(us) DWT_delay(us)
    #define HARDWARE_GET_TICK() HAL_GetTick()


#ifdef __cplusplus
}
#endif
#endif
