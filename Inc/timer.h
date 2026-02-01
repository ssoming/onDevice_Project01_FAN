
#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdint.h>

#include "stm32f4xx_hal.h"

/* 시간 만료 시 호출될 콜백 타입 */
typedef void (*TimerExpiredCallback_t)(void);

/* 초기화: 만료 콜백 등록 (NULL이면 만료 시 아무 것도 안 함) */
void Timer_Init(TimerExpiredCallback_t cb);

/* 분(1~9) 세팅: min*60초로 시작, FND에 min 표시 */
void Timer_SetMinutes(uint8_t min);

/* 타이머 취소: remain=0, FND off */
void Timer_Cancel(void);

/* main loop에서 계속 호출: 1초마다 감소 + FND 갱신 + 만료 콜백 */
void Timer_Task1s_AndUpdateFND(void);

/* 상태 조회 */
uint8_t  Timer_IsRunning(void);
uint32_t Timer_GetRemainSec(void);

#endif /* INC_TIMER_H_ */
