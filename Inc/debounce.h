
#ifndef INC_DEBOUNCE_H_
#define INC_DEBOUNCE_H_


#include "stm32f4xx_hal.h"
#include <stdint.h>

uint8_t debouncePressed(GPIO_PinState currentState,
                         GPIO_PinState *lastState,
                         uint32_t *lastTime,
                         uint32_t delay);


#endif /* INC_DEBOUNCE_H_ */
