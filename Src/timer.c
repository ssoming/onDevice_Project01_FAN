
#include "fnd.h"        /* FND_Display(), FND_AllOff() */
#include "timer.h"       /* HAL_GetTick() */

/* 내부 상태 */
static uint32_t s_remain_sec = 0;
static uint32_t s_last_1s_ms = 0;
static TimerExpiredCallback_t s_on_expired = 0;

void Timer_Init(TimerExpiredCallback_t cb)
{
    s_on_expired = cb;
    s_remain_sec = 0u;
    s_last_1s_ms = HAL_GetTick();
    FND_AllOff();
}

void Timer_Cancel(void)
{
    s_remain_sec = 0u;
    FND_AllOff();
}

uint8_t Timer_IsRunning(void)
{
    return (s_remain_sec != 0u) ? 1u : 0u;
}

uint32_t Timer_GetRemainSec(void)
{
    return s_remain_sec;
}

void Timer_SetMinutes(uint8_t min)
{
    if (min < 1u || min > 9u) return;

    s_remain_sec = (uint32_t)min * 60u;
    s_last_1s_ms = HAL_GetTick();

    /* 시작 시 입력 분을 즉시 표시 */
    FND_Display(min);
}

void Timer_Task1s_AndUpdateFND(void)
{
    if (s_remain_sec == 0u) return;

    uint32_t now = HAL_GetTick();
    if ((now - s_last_1s_ms) < 1000u) return;

    /* 누적 드리프트 방지: 정확히 1초 단위로 누적 */
    s_last_1s_ms += 1000u;

    if (s_remain_sec > 0u) s_remain_sec--;

    /* 만료 처리 */
    if (s_remain_sec == 0u)
    {
        FND_AllOff();

        if (s_on_expired)
        {
            s_on_expired();   /* main.c의 All_StopEverything() 등 */
        }
        return;
    }

    /* 남은 '분' 표시(9~1)
       예) 60~1초 남으면 1 표시, 120~61초 남으면 2 표시 */
    uint32_t disp = ((s_remain_sec - 1u) / 60u) + 1u;
    if (disp > 9u) disp = 9u;

    FND_Display((uint8_t)disp);
}
