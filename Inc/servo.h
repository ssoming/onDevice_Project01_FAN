

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include <stdint.h>

#include "stm32f4xx_hal.h"

/* 초기화 (PWM 시작은 main에서 수행) */
void Servo_Init(void);

/* 각도 직접 지정 (0~180) */
void Servo_WriteDeg(uint8_t deg);

/* Swing ON/OFF */
void Servo_SwingOn(uint8_t on);

/* 주기 호출 태스크 (main loop에서 호출) */
void Servo_Task(void);

/* 상태 조회 */
uint8_t Servo_IsSwingOn(void);


#endif /* INC_SERVO_H_ */
