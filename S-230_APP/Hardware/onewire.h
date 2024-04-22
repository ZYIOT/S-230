
#ifndef __HARDWARE_ONEWIRE_H
#define __HARDWARE_ONEWIRE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_gpio.h"
#include "hardware_wrapper.h"

#define ONEWIRE_CMD_RSCRATCHPAD 0xBE
#define ONEWIRE_CMD_WSCRATCHPAD 0x4E
#define ONEWIRE_CMD_CPYSCRATCHPAD 0x48
#define ONEWIRE_CMD_RECEEPROM 0xB8
#define ONEWIRE_CMD_RPWRSUPPLY 0xB4
#define ONEWIRE_CMD_SEARCHROM 0xF0
#define ONEWIRE_CMD_READROM 0x33
#define ONEWIRE_CMD_MATCHROM 0x55
#define ONEWIRE_CMD_SKIPROM 0xCC

  typedef struct
  {
    GPIO_TypeDef *gpio;
    uint16_t pin;
  } ONEWIRE_t;

  void ONEWIRE_init(ONEWIRE_t *OWx);
  int ONEWIRE_reset(ONEWIRE_t *OWx);
  uint8_t ONEWIRE_read_byte(ONEWIRE_t *OWx);
  void ONEWIRE_write_byte(ONEWIRE_t *OWx, uint8_t byte);
  void ONEWIRE_write_bit(ONEWIRE_t *OWx, uint8_t bit);
  uint8_t ONEWIRE_read_bit(ONEWIRE_t *OWx);
  uint8_t ONEWIRE_CRC8(uint8_t *addr, uint8_t len);
#ifdef __cplusplus
}
#endif

#endif
