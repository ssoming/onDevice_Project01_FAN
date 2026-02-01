/*
 * button.h
 *
 *  Created on: Jan 23, 2026
 *      Author: ming
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>				// "" 보다 <> 가 범위 넒음


typedef struct
{
	GPIO_TypeDef		*port;
	uint16_t				number;
	GPIO_PinState 	onState;
  uint32_t prevTime;      // ★ 추가

}BUTTON_CONTROL;

bool buttonGetPressed(uint8_t num);



#endif /* INC_BUTTON_H_ */
