#include "main.h"
#include "bsp_bl0910.h"
#include "spi.h"
#include "hardware_wrapper.h"

#define __RST_H HARDWARE_GPIO_WRITE_PIN(BL0910_NRST_GPIO_Port, BL0910_NRST_Pin, GPIO_PIN_SET);
#define __RST_L HARDWARE_GPIO_WRITE_PIN(BL0910_NRST_GPIO_Port, BL0910_NRST_Pin, GPIO_PIN_RESET);
#define __CS_H HARDWARE_GPIO_WRITE_PIN(BL0910_CS_GPIO_Port, BL0910_CS_Pin, GPIO_PIN_SET);
#define __CS_L HARDWARE_GPIO_WRITE_PIN(BL0910_CS_GPIO_Port, BL0910_CS_Pin, GPIO_PIN_RESET);

static int spi_transmit(BL0910_TypeDef *bl0910, uint8_t *buf, uint16_t size, uint32_t timeout)
{
    __CS_L
    int rc = HAL_SPI_Transmit(&hspi3, buf, size, timeout);
    // __CS_H
    return rc;
}
static int spi_transmit_receive(BL0910_TypeDef *bl0910, uint8_t *tx, uint8_t *rx, uint16_t size, uint32_t timeout)
{
    __CS_L
    int rc = HAL_SPI_TransmitReceive(&hspi3, tx, rx,size, timeout);
    // rc = HAL_SPI_Receive(&hspi3, rx, 4, timeout);
    // __CS_H
    return rc;
}

static int spi_reset(BL0910_TypeDef *bl0910)
{
    __RST_L
    HARDWARE_OS_DELAY_MS(200);
    __RST_H
    HARDWARE_OS_DELAY_MS(200);
    return APP_OK;
}

BL0910_TypeDef bl0910 = {
    .reset = &spi_reset,
    .transmit_receive = &spi_transmit_receive,
    .transmit = &spi_transmit};
