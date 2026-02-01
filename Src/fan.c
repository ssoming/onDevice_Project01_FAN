
#include "fan.h"
#include "led.h"   /* 기존 led.c 사용 */

#define DUTY_1   30u
#define DUTY_4   60u
#define DUTY_8   100u

static TIM_HandleTypeDef *s_htim = NULL;
static uint32_t s_ch = 0;
static FanStep_t s_step = FAN_STEP_0;

/* ---------------------------------------------------------
 * 내부 유틸: LED Bar를 "n개" 상태로 맞추기
 * - 기존 ledOn(num)은 앞에서부터 num개만 ON시키고,
 *   나머지 OFF는 보장하지 않으므로 먼저 8개 OFF 후 ON.
 * --------------------------------------------------------- */
static void LedBar_SetCount(uint8_t n)
{
    if (n > 8u) n = 8u;
    ledOff(8);     /* 8개 전부 OFF */
    ledOn(n);      /* 0~n-1 ON */
}

void Fan_Init(TIM_HandleTypeDef *htim, uint32_t channel)
{
    s_htim = htim;
    s_ch = channel;
    s_step = FAN_STEP_0;

    /* PWM은 main에서 Start해도 되고 여기서 해도 됨 */
    /* HAL_TIM_PWM_Start(s_htim, s_ch); */

    Fan_ApplyStep(FAN_STEP_0);
}

void Fan_ApplyStep(FanStep_t step)
{
    uint8_t duty = 0u;
    uint8_t led_n = 0u;

    switch (step)
    {
    case FAN_STEP_1: duty = DUTY_1;  led_n = 1u; break;
    case FAN_STEP_4: duty = DUTY_4;  led_n = 4u; break;
    case FAN_STEP_8: duty = DUTY_8;  led_n = 8u; break;
    case FAN_STEP_0:
    default:         duty = 0u;      led_n = 0u; break;
    }

    s_step = step;

    /* ARR 기반 듀티 계산 */
    uint32_t arrp1 = (uint32_t)__HAL_TIM_GET_AUTORELOAD(s_htim) + 1u;
    uint32_t ccr   = (arrp1 * (uint32_t)duty) / 100u;
    __HAL_TIM_SET_COMPARE(s_htim, s_ch, ccr);

    LedBar_SetCount(led_n);
}

FanStep_t Fan_GetStep(void)
{
    return s_step;
}

FanStep_t Fan_NextUp(FanStep_t cur)
{
    if (cur == FAN_STEP_0) return FAN_STEP_1;
    if (cur <= FAN_STEP_1) return FAN_STEP_4;
    if (cur <= FAN_STEP_4) return FAN_STEP_8;
    return FAN_STEP_8;
}

FanStep_t Fan_NextDown(FanStep_t cur)
{
    if (cur == FAN_STEP_0) return FAN_STEP_8;
    if (cur >= FAN_STEP_8) return FAN_STEP_4;
    if (cur >= FAN_STEP_4) return FAN_STEP_1;
    return FAN_STEP_1;
}
