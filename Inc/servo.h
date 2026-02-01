

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include <stdint.h>

#include "stm32f4xx_hal.h"

/* =========================================================
 * Servo + Swing control
 * - PWM: TIM3 CH1 (ex: PA6)
 * - Swing ON: 40~140도 왕복
 * - Swing OFF: 현재 각도 유지(각도 업데이트 중단)
 * ========================================================= */

void Servo_Init(TIM_HandleTypeDef *htim, uint32_t channel,
                uint16_t min_us, uint16_t max_us);

void Servo_WriteDeg(uint8_t deg);

/* Swing API */
void Swing_Init(int16_t start_deg, int16_t left_deg, int16_t right_deg);
void Swing_SetOn(uint8_t on);
uint8_t Swing_IsOn(void);
void Swing_Task(void);

#endif /* INC_SERVO_H_ */
