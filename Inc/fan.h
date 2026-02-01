

#ifndef INC_FAN_H_
#define INC_FAN_H_

#include <stdint.h>

#include "stm32f4xx_hal.h"

/* 팬 초기화: 내부 상태 초기화 + 팬 OFF 적용 */
void Fan_Init(void);

/* 팬 단계 적용: step = 0 / 1 / 4 / 8 */
void Fan_ApplyStep(uint8_t step);

/* 현재 단계 반환 */
uint8_t Fan_GetStep(void);

/* 단계 계산 유틸 */
uint8_t Fan_NextUp(uint8_t cur);
uint8_t Fan_NextDown(uint8_t cur);


#endif /* INC_FAN_H_ */
