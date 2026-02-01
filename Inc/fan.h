

#ifndef INC_FAN_H_
#define INC_FAN_H_

#include <stdint.h>

#include "stm32f4xx_hal.h"

/* =========================================================
 * Fan control
 * - PWM: TIM2 CH1 (ex: PA0)
 * - Step: 0 / 1 / 4 / 8
 * - LED BAR: 0 / 1 / 4 / 8개 점등
 * ========================================================= */

typedef enum {
    FAN_STEP_0 = 0,
    FAN_STEP_1 = 1,
    FAN_STEP_4 = 4,
    FAN_STEP_8 = 8
} FanStep_t;

void     Fan_Init(TIM_HandleTypeDef *htim, uint32_t channel);
void     Fan_ApplyStep(FanStep_t step);
FanStep_t Fan_GetStep(void);
FanStep_t Fan_NextUp(FanStep_t cur);
FanStep_t Fan_NextDown(FanStep_t cur);

#endif /* INC_FAN_H_ */
