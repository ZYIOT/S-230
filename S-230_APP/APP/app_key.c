#include "app_key.h"
volatile static uint8_t last_key = 0;

int APP_KEY_get(void)
{
    uint8_t status = 0;
    uint32_t time = 0;
    int rc = 0;
    for (int i = 0; i < 4; i++)
    {
        rc = BSP_KEY_read(i, &status, &time);
        if (rc == 0)
        {
            if (status == 1)
            {
                if (last_key == (i + 1))
                {
                    return -1;
                }
                else
                {
                    last_key = i + 1;
                    return last_key;
                }
            }
        }
    }
    last_key = 0;
    return -1;
}
