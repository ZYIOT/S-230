#ifndef _APP_LCD_H
#define _APP_LCD_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct 
{
    uint8_t sensorHasConfig;
    int8_t lcdSensorStatus[3];
    int8_t lcdSensorDataStatus[3];
}t_LcdSensorShow, *pt_LcdSensorShow;

#define APP_LCD_INIT()              BspLcdInit()
#define APP_LCD_CLR_SCREEN()        BSP_LCD_CLR_SCREEN()
#define APP_LCD_SHOW_HOME_PAGE()    BspLcdShowHomePage()
#define APP_LCD_SHOW_TOPIC()        BspLcdShowTopic()
#define APP_LCD_SHOW_TITLE()        BspLcdShowTitle()


#ifdef SUPPORT_IRED  
#define IF_IRED_CHECK_VALID()       (IRED_CHECK_VALID == BSP_IRED_CHECK_STATUS())
#endif

int AppLcdShowAllItem(void);
int AppLcdRefresh(void);
void APP_TFTLCD_task_run(void *argument);


#ifdef SUPPORT_IRED  
#define LCD_LIGHT_ON_TIMEOUT    (30 * 1000UL)
void APP_IRED_task_run(void *argument);
#endif

#ifdef __cplusplus
}
#endif

#endif
