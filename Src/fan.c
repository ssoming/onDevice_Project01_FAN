

#include "fan.h"
#include "tim.h"
#include "led.h"

/* ===== 내부 상태 ===== */
static uint8_t s_step = 0u;  /* 0/1/4/8 */

/* ===== 단계별 듀티(%) ===== */
#define DUTY_1   30u
#define DUTY_4   60u
#define DUTY_8   100u

static void Fan_UpdatePwmDuty(uint8_t duty_percent)
{
    /* ARR+1 기준으로 CCR 계산 */
    uint32_t arrp1 = (uint32_t)__HAL_TIM_GET_AUTORELOAD(&htim2) + 1u;
    uint32_t ccr   = (arrp1 * (uint32_t)duty_percent) / 100u;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, ccr);
}

void Fan_Init(void)
{
    s_step = 0u;
    Fan_ApplyStep(0u);
}

void Fan_ApplyStep(uint8_t step)
{
    uint8_t duty = 0u;
    uint8_t led_n = 0u;

    if      (step == 1u) { duty = DUTY_1; led_n = 1u; }
    else if (step == 4u) { duty = DUTY_4; led_n = 4u; }
    else if (step == 8u) { duty = DUTY_8; led_n = 8u; }
    else                 { duty = 0u;     led_n = 0u; }

    s_step = step;

    Fan_UpdatePwmDuty(duty);

    /* LED BAR 표시(잔상 방지: 전체 OFF 후 필요한 개수 ON) */
    ledOff(8);
    if (led_n > 0u)
        ledOn(led_n);
}

uint8_t Fan_GetStep(void)
{
    return s_step;
}

uint8_t Fan_NextUp(uint8_t cur)
{
    if (cur == 0u) return 1u;
    if (cur <= 1u) return 4u;
    if (cur <= 4u) return 8u;
    return 8u;
}

uint8_t Fan_NextDown(uint8_t cur)
{
    if (cur == 0u) return 8u;
    if (cur >= 8u) return 4u;
    if (cur >= 4u) return 1u;
    return 1u;
}
