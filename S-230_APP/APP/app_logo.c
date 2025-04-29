
#include "app_log.h"
#include "app_logo.h"

// 显示公司 Logo 
void APP_LOGO_Show(void)
{
    char *logo = "\r\n\
 _______   _____    _____ \r\n\
|__  /\\ \\ / /_ _|__|_   _|\r\n\
  / /  \\ V / | |/ _ \\| |\r\n\
 / /_   | |  | | (_) | |\r\n\
/____|  |_| |___\\___/|_|\r\n\
\r\n\
http://www.zytdwl.cn\
\r\n\r\n";
    APP_LOG_Show(logo);
}
