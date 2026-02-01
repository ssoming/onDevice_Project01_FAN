
#ifndef INC_TIMER_APP_H_
#define INC_TIMER_APP_H_

#include <stdint.h>

#include "stm32f4xx_hal.h"

/* =========================================================
 * Countdown timer (BT 입력: 1~9분)
 * - Timer_SetMinutes(1~9)
 * - Timer_Task1s(): 1초마다 감소/표시값 업데이트
 * - 만료 시 1 반환 (main에서 All_StopEverything 호출)
 * ========================================================= */

void     Timer_Cancel(void);
void     Timer_SetMinutes(uint8_t min);
uint32_t Timer_GetRemainSec(void);

/* 1초 주기 처리: 만료 시 1, 아니면 0 반환 */
uint8_t  Timer_Task1s(uint8_t *out_disp_digit);

#endif /* INC_TIMER_APP_H_ */
