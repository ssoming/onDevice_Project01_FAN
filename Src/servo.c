
#include "servo.h"
#include "tim.h"

/* ===== 내부 상태 ===== */
static uint8_t  s_swing_on = 0;
static int16_t  s_swing_deg = 90;
static int8_t   s_swing_dir = 1;
static uint32_t s_last_ms = 0;

/* ===== 서보 파라미터 ===== */
#define SERVO_MIN_US   700u
#define SERVO_MAX_US   2300u
#define SW_LEFT_DEG    40
#define SW_RIGHT_DEG   140
#define SWING_PERIOD_MS 30u   /* 속도 조절 포인트 */

/* ===== 내부 유틸 ===== */
static void Servo_ApplyDeg(uint8_t deg)
{
    if (deg > 180u) deg = 180u;

    uint32_t us = SERVO_MIN_US
                + ((uint32_t)(SERVO_MAX_US - SERVO_MIN_US) * deg) / 180u;

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, us);
}

/* ===== 외부 API ===== */
void Servo_Init(void)
{
    s_swing_on = 0u;
    s_swing_deg = 90;
    s_swing_dir = 1;
    s_last_ms = HAL_GetTick();

    Servo_ApplyDeg((uint8_t)s_swing_deg);
}

void Servo_WriteDeg(uint8_t deg)
{
    s_swing_deg = deg;
    Servo_ApplyDeg(deg);
}

void Servo_SwingOn(uint8_t on)
{
    if (on)
    {
        s_swing_on = 1u;
        s_last_ms = HAL_GetTick();

        if (s_swing_deg < SW_LEFT_DEG)  s_swing_deg = SW_LEFT_DEG;
        if (s_swing_deg > SW_RIGHT_DEG) s_swing_deg = SW_RIGHT_DEG;

        Servo_ApplyDeg((uint8_t)s_swing_deg);
    }
    else
    {
        /* OFF → PWM 유지 = 위치 hold */
        s_swing_on = 0u;
    }
}

uint8_t Servo_IsSwingOn(void)
{
    return s_swing_on;
}

void Servo_Task(void)
{
    if (!s_swing_on) return;

    uint32_t now = HAL_GetTick();
    if (now - s_last_ms < SWING_PERIOD_MS) return;
    s_last_ms = now;

    s_swing_deg += (int16_t)(s_swing_dir * 2);

    if (s_swing_deg >= SW_RIGHT_DEG)
    {
        s_swing_deg = SW_RIGHT_DEG;
        s_swing_dir = -1;
    }
    else if (s_swing_deg <= SW_LEFT_DEG)
    {
        s_swing_deg = SW_LEFT_DEG;
        s_swing_dir = 1;
    }

    Servo_ApplyDeg((uint8_t)s_swing_deg);
}
