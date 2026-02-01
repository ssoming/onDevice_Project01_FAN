
#ifndef INC_FND_H_
#define INC_FND_H_

#include "stm32f4xx_hal.h"



#define FND_COUNT 7


typedef struct
{
  GPIO_TypeDef  *port;
  uint16_t      number;
  GPIO_PinState onState;
  GPIO_PinState offState;
}FND_CONTROL;

void FND_AllOff();
void FND_Display(uint8_t digit);

#endif /* INC_FND_H_ */
