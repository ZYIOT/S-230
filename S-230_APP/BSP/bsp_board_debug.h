// encoding = GBK 

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
    #define BSP_LOG_DEFAULT_LEVEL 4
// </e>
#define ENABLE_LOG_TRACE 1
// </h>

#if (0 == ENABLE_LOG_TRACE)
    #undef ENABLE_LOG_TRACE 
    #define BSP_LOG_DEFAULT_LEVEL 5
#endif 

// <c> 关闭看门狗 
 //#define DISABLE_WDG
// </c>

// </h>


//*** <<< end of configuration section >>> ***

#ifdef __cplusplus
}
#endif
#endif
