
#include "bsp_gpio.h"

void BSP_GPIO_set_pin_as_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	uint8_t i;
	for (i = 0x00; i < 0x10; i++)
	{
		if (GPIO_Pin & (1 << i))
		{
			GPIOx->MODER &= ~(0x03 << (2 * i));
		}
	}
}

void BSP_GPIO_set_pin_as_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	uint8_t i;
	for (i = 0x00; i < 0x10; i++)
	{
		if (GPIO_Pin & (1 << i))
		{
			GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x01 << (2 * i));
		}
	}
}

void BSP_GPIO_set_pin_as_analog(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	uint8_t i;
	for (i = 0x00; i < 0x10; i++)
	{
		if (GPIO_Pin & (1 << i))
		{
			GPIOx->MODER |= (0x03 << (2 * i));
		}
	}
}

void BSP_GPIO_set_pin_as_alternate(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	uint8_t i;

	for (i = 0; i < 0x10; i++)
	{
		if ((GPIO_Pin & (1 << i)) == 0)
		{
			continue;
		}

		GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x02 << (2 * i));
	}
}
