
#include "button.h"

BUTTON_CONTROL button[4] =
		{
				{GPIOC, GPIO_PIN_9, 0, 0},
				{GPIOB, GPIO_PIN_8, 0, 0},
				{GPIOB, GPIO_PIN_9, 0, 0},
				{GPIOC, GPIO_PIN_7, 0, 0}
		};

bool buttonGetPressed(uint8_t num)
{
    bool ret = false;
    uint32_t currTime = HAL_GetTick();

    // 눌려있는지 확인
    if (HAL_GPIO_ReadPin(button[num].port, button[num].number) == button[num].onState)
    {
        // 해당 버튼의 개별 prevTime과 비교
        if(currTime - button[num].prevTime > 50) // 디바운스 시간을 약간 늘리는 것이 안정적입니다.
        {
            ret = true;
            button[num].prevTime = currTime; // 해당 버튼의 시간만 갱신
        }
    }
    return ret;
}

/**
 * Blocking Code
 */
//
//bool buttonGetPressed(uint8_t num)
//{
//	bool ret = false;
//
//	if(HAL_GPIO_ReadPin(button[num].port, button[num].number) == button[num].onState)
//	{
//		HAL_Delay(10);	// 디바운스 코드
//		if(HAL_GPIO_ReadPin(button[num].port, button[num].number) == button[num].onState)
//		{
//			ret = true;
//		}
//	}
//	return ret;
//}
