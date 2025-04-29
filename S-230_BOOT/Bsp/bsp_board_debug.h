/* encoding = GBK */
#ifndef __BSP_BOARD_DEBUG_H__
#define __BSP_BOARD_DEBUG_H__

#ifdef __cplusplus
extern "C"
{
#endif

//*** <<< Use Configuration Wizard in Context Menu >>> ***
// <h> 调试相关宏定义 

// <h> Log打印相关配置 
// <e1> 使能Log功能  
    // <o> Log等级  <0=> NONE    <1=> ERROR  <2=> WARN  <3=> INFO  <4=> DEBUG  <5=> TRACE 
    #define BSP_LOG_DEFAULT_LEVEL 5
// </e>
#define ENABLE_LOG_TRACE 1
// </h>

#if (0 == ENABLE_LOG_TRACE)
    #undef ENABLE_LOG_TRACE 
    #define BSP_LOG_DEFAULT_LEVEL 5
#endif 


// <c> 关闭WDG 
// #define DISABLE_WDG
// </c>

// <o>  使能BOOT跳转到APP(默认开启使能) 
#define DEBUG_JUMP_TO_APP_ENABLE		1

// </h>


//*** <<< end of configuration section >>> ***

#ifdef __cplusplus
}
#endif

#endif