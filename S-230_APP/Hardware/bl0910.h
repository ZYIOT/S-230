#ifndef __BL0910_H
#define __BL0910_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "gpio.h"
#include "spi.h"
#include "hardware_wrapper.h"
#include "board_config.h"

    typedef struct BL0910_TypeDef_s BL0910_TypeDef;
    typedef int (*spi_transmit_fun)(BL0910_TypeDef *bl0910, uint8_t *buf, uint16_t size, uint32_t timeout);
    typedef int (*spi_transmit_receive_fun)(BL0910_TypeDef *bl0910, uint8_t *tx, uint8_t *rx, uint16_t size, uint32_t timeout);
    typedef int (*spi_reset_fun)(BL0910_TypeDef *bl0910);

    struct BL0910_TypeDef_s
    {
        spi_reset_fun reset;
        spi_transmit_fun transmit;
        spi_transmit_receive_fun transmit_receive;
    };
/*
#define BL0910_MAP(F)          \
    F(FAST_RMS_1, to_current)  \
    F(FAST_RMS_2, to_current)  \
    F(FAST_RMS_3, to_current)  \
    F(FAST_RMS_4, to_current)  \
    F(FAST_RMS_5, to_current)  \
    F(FAST_RMS_6, to_current)  \
    F(FAST_RMS_7, to_voltage)  \
    F(FAST_RMS_8, to_voltage)  \
    F(FAST_RMS_9, to_voltage)  \
    F(FAST_RMS_10, to_voltage) \
    F(FAST_RMS_V, to_voltage)
*/

#define BL0910_MAP(F)     \
    F(RMS_1, to_current)  \
    F(RMS_2, to_current)  \
    F(RMS_3, to_current)  \
    F(RMS_4, to_current)  \
    F(RMS_5, to_current)  \
    F(RMS_6, to_current)  \
    F(RMS_7, to_current)  \
    F(RMS_8, to_current)  \
    F(RMS_9, to_current)  \
    F(RMS_10, to_current) \
    F(RMS_V, to_voltage)
    

#define BL0910_DATA_DEFINE(_name, _calculate) \
    uint32_t _name;

    typedef struct
    {
        BL0910_MAP(BL0910_DATA_DEFINE)
        // uint16_t RMS_V;  //XXX.X	V			0.1伏/LSB
        // uint16_t WATT_1; //XXXX.X	W		0.1瓦/LSB
        // uint16_t WATT_2;
        // uint16_t WATT_3;
        // uint16_t WATT_4;
        // uint16_t WATT_5;
        // uint16_t WATT_6;
        // uint16_t WATT_7;
        // uint16_t WATT_8;
        // uint16_t WATT_9;
        // uint16_t WATT_10;
        // uint16_t Energy_1; //XXXXXX	kWh   1度电/LSB，最大65535度电
        // uint16_t Energy_2;
        // uint16_t Energy_3;
        // uint16_t Energy_4;
        // uint16_t Energy_5;
        // uint16_t Energy_6;
        // uint16_t Energy_7;
        // uint16_t Energy_8;
        // uint16_t Energy_9;
        // uint16_t Energy_10;
        // uint16_t Energy_Sum;
        // uint16_t Period; //XX.XXHz		0.01Hz/LSB
        // uint16_t TPS1;   //XX.XX℃			0.01℃/LSB
        // uint16_t TPS2;
    } bl0910_data_t, *bl0910_data_pt;

    int BL0910_init(BL0910_TypeDef *bl0910);
    int BL0910_reset(BL0910_TypeDef *bl0910);
    int BL0910_system_reset(BL0910_TypeDef *bl0910);
    int BL0910_register_reset(BL0910_TypeDef *bl0910);
    int BL0910_read(BL0910_TypeDef *bl0910, bl0910_data_pt data);

#ifdef __cplusplus
}
#endif

#endif
