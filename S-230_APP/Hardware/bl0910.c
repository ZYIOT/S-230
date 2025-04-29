#include "bl0910.h"
#include "utils.h"

#define ADDR_VERSION 0x00
#define ADDR_WAVE_1 0x01
#define ADDR_WAVE_2 0x02
#define ADDR_WAVE_3 0x03
#define ADDR_WAVE_4 0x04
#define ADDR_WAVE_5 0x05
#define ADDR_WAVE_6 0x06
#define ADDR_WAVE_7 0x07
#define ADDR_WAVE_8 0x08
#define ADDR_WAVE_9 0x09
#define ADDR_WAVE_10 0x0A
#define ADDR_WAVE_V 0x0B
#define ADDR_RMS_1 0x0C
#define ADDR_RMS_2 0x0D
#define ADDR_RMS_3 0x0E
#define ADDR_RMS_4 0x0F
#define ADDR_RMS_5 0x10
#define ADDR_RMS_6 0x11
#define ADDR_RMS_7 0x12
#define ADDR_RMS_8 0x13
#define ADDR_RMS_9 0x14
#define ADDR_RMS_10 0x15
#define ADDR_RMS_V 0x16
#define ADDR_FAST_RMS_1 0x17
#define ADDR_FAST_RMS_2 0x18
#define ADDR_FAST_RMS_3 0x19
#define ADDR_FAST_RMS_4 0x1A
#define ADDR_FAST_RMS_5 0x1B
#define ADDR_FAST_RMS_6 0x1C
#define ADDR_FAST_RMS_7 0x1D
#define ADDR_FAST_RMS_8 0x1E
#define ADDR_FAST_RMS_9 0x1F
#define ADDR_FAST_RMS_10 0x20
#define ADDR_FAST_RMS_V 0x21
#define ADDR_WATT_1 0x22
#define ADDR_WATT_2 0x23
#define ADDR_WATT_3 0x24
#define ADDR_WATT_4 0x25
#define ADDR_WATT_5 0x26
#define ADDR_WATT_6 0x27
#define ADDR_WATT_7 0x28
#define ADDR_WATT_8 0x29
#define ADDR_WATT_9 0x2A
#define ADDR_WATT_10 0x2B
#define ADDR_WATT_V 0x2C
#define ADDR_VAR 0x2D
#define ADDR_VA 0x2E
#define ADDR_CF_CNT_1 0x2F
#define ADDR_CF_CNT_2 0x30
#define ADDR_CF_CNT_3 0x31
#define ADDR_CF_CNT_4 0x32
#define ADDR_CF_CNT_5 0x33
#define ADDR_CF_CNT_6 0x34
#define ADDR_CF_CNT_7 0x35
#define ADDR_CF_CNT_8 0x36
#define ADDR_CF_CNT_9 0x37
#define ADDR_CF_CNT_10 0x38
#define ADDR_CF_CNT_V 0x39
#define ADDR_CFQ_CNT 0x3A
#define ADDR_CFS_CNT 0x3B
#define ADDR_ANGLE_1 0x3C
#define ADDR_ANGLE_2 0x3D
#define ADDR_ANGLE_3 0x3E
#define ADDR_ANGLE_4 0x3F
#define ADDR_ANGLE_5 0x40
#define ADDR_ANGLE_6 0x41
#define ADDR_ANGLE_7 0x42
#define ADDR_ANGLE_8 0x43
#define ADDR_ANGLE_9 0x44
#define ADDR_ANGLE_10 0x45
#define ADDR_FAST_RMS_1_HOLD 0x46
#define ADDR_FAST_RMS_2_HOLD 0x47
#define ADDR_FAST_RMS_3_HOLD 0x48
#define ADDR_FAST_RMS_4_HOLD 0x49
#define ADDR_PF 0x4A
#define ADDR_LINE_WATTHR 0x4B
#define ADDR_LINE_VARHR 0x4C
#define ADDR_SIGN 0x4D
#define ADDR_PERIOD 0x4E
#define ADDR_STATUS1 0x54
#define ADDR_STATUS2 0x55
#define ADDR_STATUS3 0x56
#define ADDR_FAST_RMS_5_HOLD 0x57
#define ADDR_FAST_RMS_6_HOLD 0x58
#define ADDR_FAST_RMS_7_HOLD 0x59
#define ADDR_FAST_RMS_8_HOLD 0x5A
#define ADDR_FAST_RMS_9_HOLD 0x5B
#define ADDR_FAST_RMS_10_HOLD 0x5C
#define ADDR_VAR1 0x5D
#define ADDR_TPS1 0x5E
#define ADDR_TPS2 0x5F
#define ADDR_GAIN1 0x60
#define ADDR_GAIN2 0x61
#define ADDR_PHASE1_2 0x64
#define ADDR_PHASE3_4 0x65
#define ADDR_PHASE5_6 0x66
#define ADDR_PHASE7_8 0x67
#define ADDR_PHASE9_10 0x68
#define ADDR_PHASE11 0x69
#define ADDR_VAR_PHCAL_I 0x6A
#define ADDR_VAR_PHCAL_V 0x6B
#define ADDR_RMSGN_1 0x6C
#define ADDR_RMSGN_2 0x6D
#define ADDR_RMSGN_3 0x6E
#define ADDR_RMSGN_4 0x6F
#define ADDR_RMSGN_5 0x70
#define ADDR_RMSGN_6 0x71
#define ADDR_RMSGN_7 0x72
#define ADDR_RMSGN_8 0x73
#define ADDR_RMSGN_9 0x74
#define ADDR_RMSGN_10 0x75
#define ADDR_RMSGN_11 0x76
#define ADDR_RMSOS_1 0x77
#define ADDR_RMSOS_2 0x78
#define ADDR_RMSOS_3 0x79
#define ADDR_RMSOS_4 0x7A
#define ADDR_RMSOS_5 0x7B
#define ADDR_RMSOS_6 0x7C
#define ADDR_RMSOS_7 0x7D
#define ADDR_RMSOS_8 0x7E
#define ADDR_RMSOS_9 0x7F
#define ADDR_RMSOS_10 0x80
#define ADDR_RMSOS_11 0x81
#define ADDR_WA_LOS1_2 0x82
#define ADDR_WA_LOS3_4 0x83
#define ADDR_WA_LOS5_6 0x84
#define ADDR_WA_LOS7_8 0x85
#define ADDR_WA_LOS9_10 0x86
#define ADDR_VAR_LOS 0x87
#define ADDR_VAR_WATT_CREEP 0x88
#define ADDR_WA_CREEP2 0x89
#define ADDR_RMS_CREEP 0x8A
#define ADDR_FAST_RMS_CTRL 0x8B
#define ADDR_I_V_PKLVL 0x8C
#define ADDR_SAGCYC_ZXTOUT 0x8E
#define ADDR_SAGLVL_LINECYC 0x8F
#define ADDR_FLAG_CTRL 0x90
#define ADDR_FLAG_CTRL1 0x91
#define ADDR_FLAG_CTRL2 0x92
#define ADDR_ADC_PD 0x93
#define ADDR_TPS_CTRL 0x94
#define ADDR_TPS2_A_B 0x95
#define ADDR_MODE1 0x96
#define ADDR_MODE2 0x97
#define ADDR_MODE3 0x98
#define ADDR_MASK1 0x9A
#define ADDR_MASK2 0x9B
#define ADDR_RST_ENG 0x9D
#define ADDR_USR_WRPROT 0x9E
#define ADDR_SOFT_RESET 0x9F
#define ADDR_CHGN_1 0xA0
#define ADDR_CHGN_2 0xA1
#define ADDR_CHGN_3 0xA2
#define ADDR_CHGN_4 0xA3
#define ADDR_CHGN_5 0xA4
#define ADDR_CHGN_6 0xA5
#define ADDR_CHGN_7 0xA6
#define ADDR_CHGN_8 0xA7
#define ADDR_CHGN_9 0xA8
#define ADDR_CHGN_10 0xA9
#define ADDR_CHGN_11 0xAA
#define ADDR_CHOS_1 0xAB
#define ADDR_CHOS_2 0xAC
#define ADDR_CHOS_3 0xAD
#define ADDR_CHOS_4 0xAE
#define ADDR_CHOS_5 0xAF
#define ADDR_CHOS_6 0xB0
#define ADDR_CHOS_7 0xB1
#define ADDR_CHOS_8 0xB2
#define ADDR_CHOS_9 0xB3
#define ADDR_CHOS_10 0xB4
#define ADDR_CHOS_11 0xB5
#define ADDR_WATTGN_1 0xB6
#define ADDR_WATTGN_2 0xB7
#define ADDR_WATTGN_3 0xB8
#define ADDR_WATTGN_4 0xB9
#define ADDR_WATTGN_5 0xBA
#define ADDR_WATTGN_6 0xBB
#define ADDR_WATTGN_7 0xBC
#define ADDR_WATTGN_8 0xBD
#define ADDR_WATTGN_9 0xBE
#define ADDR_WATTGN_10 0xBF
#define ADDR_WATTOS_1 0xC0
#define ADDR_WATTOS_2 0xC1
#define ADDR_WATTOS_3 0xC2
#define ADDR_WATTOS_4 0xC3
#define ADDR_WATTOS_5 0xC4
#define ADDR_WATTOS_6 0xC5
#define ADDR_WATTOS_7 0xC6
#define ADDR_WATTOS_8 0xC7
#define ADDR_WATTOS_9 0xC8
#define ADDR_WATTOS_10 0xC9
#define ADDR_VARGN 0xCA
#define ADDR_VAROS 0xCB
#define ADDR_VAGN 0xCC
#define ADDR_VAOS 0xCD
#define ADDR_CFDIV 0xCE

//电参数转换系数
//电压通道电阻分压采用20K*5+100进行分压，电阻精度0.5%
//电流通道采用1毫欧电阻进行电流采样，采用厚声的1毫欧采样电阻，实际大约0.946毫欧，系数进行修正
#define VOLTAGE_K 7808   //  13162*1*220/(1.1258*(20*5+0.1))=7789
#define CURRENT_K 55992  //	206000*0.306/1.1258=55992
#define POWER_K 3232     //	646.6*0.946*1*1000/(1.1258*1.1258*(300*5+1))=321.5 运算时放大10倍
#define ENERGY_K 8465    //1度电对应的脉冲数      		3600000*16*321.5/(4194304*0.032768*16)=8421
#define ENERGY_SUM_K 529 //Energy_K/16

// #define __RST_H HARDWARE_GPIO_WRITE_PIN(bl0910->rst_gpio, bl0910->rst_pin, GPIO_PIN_SET);
// #define __RST_L HARDWARE_GPIO_WRITE_PIN(bl0910->rst_gpio, bl0910->rst_pin, GPIO_PIN_RESET);
// #define __CS_H HARDWARE_GPIO_WRITE_PIN(bl0910->cs_gpio, bl0910->cs_pin, GPIO_PIN_SET);
// #define __CS_L HARDWARE_GPIO_WRITE_PIN(bl0910->cs_gpio, bl0910->cs_pin, GPIO_PIN_RESET);
#define __CHECK_RC_AND_RETURN \
    if (rc != 0)              \
    {                         \
        return rc;            \
    }
#define BL0910_DEFAULT_DELAY() HARDWARE_OS_DELAY_MS(5)

#define _SPI_WRITE(addr, value)                      \
    do                                               \
    {                                                \
        rc = bl0910_spi_write(bl0910, addr, value);  \
        __CHECK_RC_AND_RETURN                        \
    } while (0);

#define _SPI_READ(addr, value)                       \
    do                                               \
    {                                                \
        rc = bl0910_spi_read(bl0910, addr, value);   \
        __CHECK_RC_AND_RETURN                        \
    } while (0);

static uint8_t chksum(uint8_t *buf, uint8_t size)
{
    uint8_t t = 0;
    uint8_t i;
    for (i = 0; i < size; ++i)
    {
        t += buf[i];
    }
    t = ~t;
    return t;
}

static int bl0910_spi_read(BL0910_TypeDef *bl0910, uint8_t addr, uint32_t *value)
{
    uint8_t tx[6] = {0};
    tx[0] = 0x82;
    tx[1] = addr;
    uint8_t rx[6] = {0};
    int rc = bl0910->transmit_receive(bl0910, tx, rx, 6, 100);
    __CHECK_RC_AND_RETURN
    rx[0] = 0x82;
    rx[1] = addr;
    if (chksum(rx, 5) != rx[5])
    {
        return -1;
    }
    *value = (read_uint32_t_BE(&(rx[2])) >> 8);
    return 0;
}

static int bl0910_spi_write(BL0910_TypeDef *bl0910, uint8_t addr, uint32_t value)
{
    uint8_t buf[6] = {0};
    buf[0] = 0x81;
    write_uint32_t_BE(value, &(buf[1]));
    buf[1] = addr;
    buf[5] = chksum(buf, 5);
    return bl0910->transmit(bl0910, buf, 6, 100);
}

int BL0910_init(BL0910_TypeDef *bl0910)
{
    int rc = 0;
    BL0910_reset(bl0910);
    _SPI_WRITE(ADDR_USR_WRPROT, 0x5555);
    __CHECK_RC_AND_RETURN
    BL0910_DEFAULT_DELAY();
    _SPI_WRITE(ADDR_MODE3, 0x80000); //模式 1U10I
    BL0910_DEFAULT_DELAY();
    // 电流通道[1:10],1倍增益，电压通道[1],1倍增益
    _SPI_WRITE(ADDR_GAIN1, 0);
    BL0910_DEFAULT_DELAY();
    _SPI_WRITE(ADDR_GAIN2, 0);
    BL0910_DEFAULT_DELAY();
    uint32_t value;
    //为确保稳定工作，取单相50W（0.25A）对应的阈值,累计时间 320ms
    //_SPI_WRITE(ADDR_FAST_RMS_CTRL, 0xA00525);  //100mA 
    //_SPI_WRITE(ADDR_FAST_RMS_CTRL, 0xA0062C);//120mA 
    _SPI_WRITE(ADDR_FAST_RMS_CTRL, 0xA007B7);//150mA 
    //_SPI_WRITE(ADDR_FAST_RMS_CTRL, 0xA007FF);
    BL0910_DEFAULT_DELAY();
    _SPI_READ(ADDR_FAST_RMS_CTRL, &value);
    BL0910_DEFAULT_DELAY();
    //电参数运算系统复位，Reg3B~2F清零
    _SPI_WRITE(ADDR_RST_ENG, 0x1FFF);
    BL0910_DEFAULT_DELAY();
    _SPI_WRITE(ADDR_SOFT_RESET, 0x5A5A5A);
    BL0910_DEFAULT_DELAY();
    HARDWARE_OS_DELAY_MS(2);
    _SPI_WRITE(ADDR_USR_WRPROT, 0);
    BL0910_DEFAULT_DELAY();
    return APP_OK;
}

int BL0910_reset(BL0910_TypeDef *bl0910)
{
    return bl0910->reset(bl0910);
}

// 系统复位——只复位数字部分的状态机和寄存器 
int BL0910_system_reset(BL0910_TypeDef *bl0910)
{
    int rc = 0;
    _SPI_WRITE(ADDR_USR_WRPROT, 0x5555);
    uint32_t value;
    _SPI_READ(ADDR_USR_WRPROT, &value);
    BL0910_DEFAULT_DELAY();
    _SPI_WRITE(ADDR_SOFT_RESET, 0x5a5a5a);
    BL0910_DEFAULT_DELAY();
    return APP_OK;
}

// 用户读写寄存器复位——复位 R[60:9F]  R[A0:D0] 
int BL0910_register_reset(BL0910_TypeDef *bl0910)
{
    int rc = 0;
    _SPI_WRITE(ADDR_USR_WRPROT, 0x5555);
    uint32_t value;
    _SPI_READ(ADDR_USR_WRPROT, &value);
    BL0910_DEFAULT_DELAY();
    _SPI_WRITE(ADDR_SOFT_RESET, 0x55aa55);
    BL0910_DEFAULT_DELAY();
    return APP_OK;
}

#include "bsp_log.h"
// I 为实际互感器CT采样电流 
// 互感器变比 625 
// Vi 为输入BL0910的电流值(A) = I = 625 * Vi 
// GainI 为设定的电流增益 
// Vref 为 1.097V 
// 采样电阻 R1+R2 为 5.1R*2 
// 电流有效值寄存器值 = value 
// 则电流与寄存器值的变比 Ki = value / I = (12875 * GainI * (R1 + R2)) / (Vref * 625)
static uint32_t to_current(uint32_t value, bl0910_data_pt data)
{
    //return value * 1000 / 55992;  
    //return value ; 	
	//return value / 192;   //(206000*10.2)/(16*1.097*625)
    // BSP_LOG_debug("C:%06x\r\n", value);
    value /= 192;
    return value;
}
// PT 源端采样电阻为 R25 R26 R35 R36 R37 
// 采样电阻 R46 
// GainV 为设定的电压增益 
// 电压与寄存器的变比 Kv = value / V = (13162 * GainV * R46) / (Vref * (R25 + R26 + R35 + R36 + R37)) 
static uint32_t to_voltage(uint32_t value, bl0910_data_pt data)
{
    return value/11998;
}

#define BL0910_READ_DATA(_name, _calculate)  \
    _SPI_READ(ADDR_##_name, &(data->_name)); \
    data->_name = _calculate(data->_name, data);

int BL0910_read(BL0910_TypeDef *bl0910, bl0910_data_pt data)
{
    int rc = 0;
    BL0910_MAP(BL0910_READ_DATA)
    return APP_OK;
}
