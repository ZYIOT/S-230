/* encoding = GBK */
#ifndef __BSP_CONFIG_H
#define __BSP_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

//*** <<< Use Configuration Wizard in Context Menu >>> ***

// <h> 主板定义 
// <e3> BOOT软件版本信息 
// <o> MAJOR
// <o1> MINOR 
// <o2> REVISION 
#define BOOT_VERSION_H	3
#define BOOT_VERSION_M	0
#define BOOT_VERSION_L	1
// </e>
#define ENABLE_EDIT_BOOT_VERSION  0

// <e3> APP默认软件版本信息 
// <o> DEFAULT_MAJOR
// <o1> DEFAULT_MINOR 
// <o2> DEFAULT_REVISION 
#define APP_VERSION_H_DEFAULT	3
#define APP_VERSION_M_DEFAULT	0
#define APP_VERSION_L_DEFAULT	0
// </e>
#define ENABLE_EDIT_APP_DEFAULT_VERSION  0

// <e3> 硬件信息 
// <o> MAJOR
// <o1> MINOR 
// <o2> REVISION 
#define BOOT_HARDWARE_MAJOR 	1
#define BOOT_HARDWARE_MINOR 	0
#define BOOT_HARDWARE_REVISION 	0
// </e>
#define ENABLE_EDIT_HARDWARE_VERSION  0

// </h>



//*** <<< end of configuration section >>> ***

#ifdef __cplusplus
}
#endif

#endif
