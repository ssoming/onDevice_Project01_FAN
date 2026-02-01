#include "timer_app.h"

static uint32_t s_remain_sec = 0u;
static uint32_t s_last_1s_ms = 0u;

void Timer_Cancel(void)
{
    s_remain_sec = 0u;
}

void Timer_SetMinutes(uint8_t min)
{
    if (min < 1u || min > 9u) return;

    s_remain_sec = (uint32_t)min * 60u;
    s_last_1s_ms = HAL_GetTick();
}

uint32_t Timer_GetRemainSec(void)
{
    return s_remain_sec;
}

/* ---------------------------------------------------------
 * out_disp_digit:
 *  - 동작 중이면 9~1 표시값을 갱신해서 전달
 *  - 타이머가 꺼져 있으면 out_disp_digit 변경하지 않음
 * return:
 *  - 만료(0초 도달)면 1 반환
 * --------------------------------------------------------- */
uint8_t Timer_Task1s(uint8_t *out_disp_digit)
{
    if (s_remain_sec == 0u) return 0u;

    uint32_t now = HAL_GetTick();
    if ((now - s_last_1s_ms) < 1000u) return 0u;
    s_last_1s_ms += 1000u;

    if (s_remain_sec > 0u) s_remain_sec--;

    if (s_remain_sec == 0u)
    {
        return 1u; /* 만료 */
    }

    /* 남은 분 표시(9~1) */
    uint32_t disp = ((s_remain_sec - 1u) / 60u) + 1u;
    if (disp > 9u) disp = 9u;

    if (out_disp_digit) *out_disp_digit = (uint8_t)disp;
    return 0u;
}
