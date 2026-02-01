
#ifndef INC_FND_H_
#define INC_FND_H_

#include "stm32f4xx_hal.h"


/* =========================================================
 * 7-Segment (1-digit)
 * - Common 핀 1개(SEG_COM)
 * - Ghost 방지: COM OFF -> segment 갱신 -> COM ON
 * - 좌우 미러 문제 발생 시 보정 옵션 제공
 * ========================================================= */

void FND_InitPins(void);
void FND_AllOff(void);
void FND_Display(uint8_t digit);


#endif /* INC_FND_H_ */
