
#include "debounce.h"

extern uint32_t millis(void);

uint8_t debouncePressed(GPIO_PinState currentState,
                         GPIO_PinState *lastState,
                         uint32_t *lastTime,
                         uint32_t delay)
{
    if (currentState != *lastState)
    {
        if (millis() - *lastTime > delay)
        {
            *lastTime = millis();
            *lastState = currentState;

            if (currentState == GPIO_PIN_RESET)
            {
                return 1; // 유효한 눌림 이벤트
            }
        }
        else
        {
            *lastState = currentState;
        }
    }
    return 0;
}

