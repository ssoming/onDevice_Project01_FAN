#include "servo.h"

/* 기본 값(원하면 main에서 Init 인자로 바꿔도 됨) */
static TIM_HandleTypeDef *s_htim = NULL;
static uint32_t s_ch = 0;
static uint16_t s_min_us = 700u;
static uint16_t s_max_us = 2300u;

/* Swing 상태 */
static uint8_t  s_on = 0u;
static int16_t  s_deg = 90;
static int8_t   s_dir = 1;
static uint32_t s_last_ms = 0;

static int16_t  s_left = 40;
static int16_t  s_right = 140;

void Servo_Init(TIM_HandleTypeDef *htim, uint32_t channel,
                uint16_t min_us, uint16_t max_us)
{
    s_htim = htim;
    s_ch = channel;
    s_min_us = min_us;
    s_max_us = max_us;

    /* PWM은 main에서 Start해도 되고 여기서 해도 됨 */
    /* HAL_TIM_PWM_Start(s_htim, s_ch); */
}

void Servo_WriteDeg(uint8_t deg)
{
    if (deg > 180u) deg = 180u;

    uint32_t us = (uint32_t)s_min_us
                + ((uint32_t)(s_max_us - s_min_us) * (uint32_t)deg) / 180u;

    __HAL_TIM_SET_COMPARE(s_htim, s_ch, us);
}

/* ---------------------------------------------------------
 * Swing: 초기화 및 범위 설정
 * --------------------------------------------------------- */
void Swing_Init(int16_t start_deg, int16_t left_deg, int16_t right_deg)
{
    s_deg = start_deg;
    s_left = left_deg;
    s_right = right_deg;

    if (s_left < 0) s_left = 0;
    if (s_right > 180) s_right = 180;
    if (s_left > s_right) { int16_t t = s_left; s_left = s_right; s_right = t; }

    if (s_deg < s_left) s_deg = s_left;
    if (s_deg > s_right) s_deg = s_right;

    Servo_WriteDeg((uint8_t)s_deg);
    s_on = 0u;
    s_dir = 1;
    s_last_ms = HAL_GetTick();
}

void Swing_SetOn(uint8_t on)
{
    if (on)
    {
        s_on = 1u;
        s_last_ms = HAL_GetTick();

        /* 범위 밖이면 클램프 */
        if (s_deg < s_left)  s_deg = s_left;
        if (s_deg > s_right) s_deg = s_right;

        /* 현재 각도 PWM 동기화 */
        Servo_WriteDeg((uint8_t)s_deg);
    }
    else
    {
        /* OFF: 각도 업데이트 중지, PWM은 유지 => 현재 위치 고정 */
        s_on = 0u;
    }
}

uint8_t Swing_IsOn(void)
{
    return s_on;
}

void Swing_Task(void)
{
    if (!s_on) return;

    uint32_t now = HAL_GetTick();
    if ((now - s_last_ms) < 30u) return;   /* 속도 고정 */
    s_last_ms = now;

    s_deg += (int16_t)(s_dir * 2);

    if (s_deg >= s_right)
    {
        s_deg = s_right;
        s_dir = -1;
    }
    else if (s_deg <= s_left)
    {
        s_deg = s_left;
        s_dir = 1;
    }

    Servo_WriteDeg((uint8_t)s_deg);
}
