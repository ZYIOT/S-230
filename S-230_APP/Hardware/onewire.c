
#include "onewire.h"

#define ONEWIRE_LOW HARDWARE_GPIO_WRITE_PIN((OWx)->gpio, (OWx)->pin, GPIO_PIN_RESET);
#define ONEWIRE_HIGH HARDWARE_GPIO_WRITE_PIN((OWx)->gpio, (OWx)->pin, GPIO_PIN_SET);
#define ONEWIRE_INPUT HARDWARE_GPIO_SET_INPUT(OWx->gpio, (OWx)->pin);
#define ONEWIRE_OUTPUT HARDWARE_GPIO_SET_OUTPUT(OWx->gpio, (OWx)->pin);
#define ONEWIRE_READ HARDWARE_GPIO_READ_PIN(OWx->gpio, OWx->pin)

void ONEWIRE_init(ONEWIRE_t *OWx)
{
	
}

int ONEWIRE_reset(ONEWIRE_t *OWx)
{
	uint8_t i;
	ONEWIRE_LOW
	ONEWIRE_OUTPUT
	HARDWARE_HAL_DELAY_US(480);
	ONEWIRE_INPUT
	HARDWARE_HAL_DELAY_US(70);
	i = ONEWIRE_READ;
	HARDWARE_HAL_DELAY_US(410);
	return i == 0 ? 0 : -1;
}

void ONEWIRE_write_bit(ONEWIRE_t *OWx, uint8_t bit)
{
	ONEWIRE_LOW
	ONEWIRE_OUTPUT
	if (bit)
	{
		HARDWARE_HAL_DELAY_US(10);
		ONEWIRE_INPUT
		HARDWARE_HAL_DELAY_US(55);
		ONEWIRE_INPUT
	}
	else
	{
		HARDWARE_HAL_DELAY_US(65);
		ONEWIRE_INPUT
		HARDWARE_HAL_DELAY_US(5);
		ONEWIRE_INPUT
	}
}

uint8_t ONEWIRE_read_bit(ONEWIRE_t *OWx)
{
	uint8_t bit = 0;

	ONEWIRE_LOW
	ONEWIRE_OUTPUT
	HARDWARE_HAL_DELAY_US(3);

	ONEWIRE_INPUT
	HARDWARE_HAL_DELAY_US(10);

	if (ONEWIRE_READ)
	{
		bit = 1;
	}
	HARDWARE_HAL_DELAY_US(50);
	return bit;
}

void ONEWIRE_write_byte(ONEWIRE_t *OWx, uint8_t byte)
{
	uint8_t i = 8;
	while (i--)
	{
		ONEWIRE_write_bit(OWx, byte & 0x01);
		byte >>= 1;
	}
}

uint8_t ONEWIRE_read_byte(ONEWIRE_t *OWx)
{
	uint8_t i = 8, byte = 0;
	while (i--)
	{
		byte >>= 1;
		byte |= (ONEWIRE_read_bit(OWx) << 7);
	}

	return byte;
}

uint8_t ONEWIRE_CRC8(uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0, inbyte, i, mix;

	while (len--)
	{
		inbyte = *addr++;
		for (i = 8; i; i--)
		{
			mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix)
			{
				crc ^= 0x8C;
			}
			inbyte >>= 1;
		}
	}

	return crc;
}
