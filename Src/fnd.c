
#include "fnd.h"


FND_CONTROL fnd[7] =
		{
				{GPIOA, GPIO_PIN_15, GPIO_PIN_SET, GPIO_PIN_RESET},

				{GPIOB, GPIO_PIN_7, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOC, GPIO_PIN_13, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOC, GPIO_PIN_1, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOC, GPIO_PIN_0, GPIO_PIN_SET, GPIO_PIN_RESET},

				{GPIOC, GPIO_PIN_2, GPIO_PIN_SET, GPIO_PIN_RESET},
				{GPIOC, GPIO_PIN_3, GPIO_PIN_SET, GPIO_PIN_RESET},
		};

const uint8_t fndMask[10] =
{
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F,  // 8
	0x6F  // 9
};

void FND_AllOff(void)
{
  for (uint8_t i = 0; i < FND_COUNT; i++)
  {
    HAL_GPIO_WritePin(fnd[i].port, fnd[i].number, fnd[i].offState);
  }
}

void FND_Display(uint8_t digit)
{
  if (digit > 9) return;

  uint8_t mask = fndMask[digit];

  for (uint8_t seg = 0; seg < FND_COUNT; seg++)
  {
    GPIO_PinState state = (mask & (1u << seg)) ? fnd[seg].onState : fnd[seg].offState;
    HAL_GPIO_WritePin(fnd[seg].port, fnd[seg].number, state);
  }
}
