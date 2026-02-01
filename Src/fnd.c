
#include "fnd.h"

/* ====== Pin map: 기존 main.c 값 그대로 ====== */
#define SEG_A_PORT GPIOC
#define SEG_A_PIN  GPIO_PIN_3
#define SEG_B_PORT GPIOC
#define SEG_B_PIN  GPIO_PIN_2
#define SEG_C_PORT GPIOC
#define SEG_C_PIN  GPIO_PIN_1
#define SEG_D_PORT GPIOC
#define SEG_D_PIN  GPIO_PIN_0
#define SEG_E_PORT GPIOC
#define SEG_E_PIN  GPIO_PIN_13
#define SEG_F_PORT GPIOB
#define SEG_F_PIN  GPIO_PIN_7
#define SEG_G_PORT GPIOA
#define SEG_G_PIN  GPIO_PIN_15
#define SEG_COM_PORT GPIOC
#define SEG_COM_PIN  GPIO_PIN_15

/* 논리 정의(사용 보드에 맞춰 유지) */
#define SEG_ON   GPIO_PIN_SET
#define SEG_OFF  GPIO_PIN_RESET
#define COM_ON   GPIO_PIN_RESET
#define COM_OFF  GPIO_PIN_SET

static const uint8_t s_fnd_mask[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

/* ---------------------------------------------------------
 * 숫자가 좌우 반전(거울)처럼 보일 때만 1로 두세요.
 * - B <-> F
 * - C <-> E
 * --------------------------------------------------------- */
#define FND_CORR_MIRROR_LR   1

static uint8_t FND_MirrorLR(uint8_t m)
{
    uint8_t r = 0;

    /* A(0), D(3), G(6) 유지 */
    if (m & (1u<<0)) r |= (1u<<0);
    if (m & (1u<<3)) r |= (1u<<3);
    if (m & (1u<<6)) r |= (1u<<6);

    /* B(1) <-> F(5) */
    if (m & (1u<<1)) r |= (1u<<5);
    if (m & (1u<<5)) r |= (1u<<1);

    /* C(2) <-> E(4) */
    if (m & (1u<<2)) r |= (1u<<4);
    if (m & (1u<<4)) r |= (1u<<2);

    return r;
}

static uint8_t FND_CorrectMask(uint8_t m)
{
#if FND_CORR_MIRROR_LR
    m = FND_MirrorLR(m);
#endif
    return m;
}

void FND_InitPins(void)
{
    GPIO_InitTypeDef g = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    g.Mode  = GPIO_MODE_OUTPUT_PP;
    g.Pull  = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;

    /* PC: A,B,C,D,E,COM */
    g.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_13 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOC, &g);

    /* PB: F */
    g.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &g);

    /* PA: G */
    g.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOA, &g);

    FND_AllOff();
}

void FND_AllOff(void)
{
    HAL_GPIO_WritePin(SEG_COM_PORT, SEG_COM_PIN, COM_OFF);

    HAL_GPIO_WritePin(SEG_A_PORT, SEG_A_PIN, SEG_OFF);
    HAL_GPIO_WritePin(SEG_B_PORT, SEG_B_PIN, SEG_OFF);
    HAL_GPIO_WritePin(SEG_C_PORT, SEG_C_PIN, SEG_OFF);
    HAL_GPIO_WritePin(SEG_D_PORT, SEG_D_PIN, SEG_OFF);
    HAL_GPIO_WritePin(SEG_E_PORT, SEG_E_PIN, SEG_OFF);
    HAL_GPIO_WritePin(SEG_F_PORT, SEG_F_PIN, SEG_OFF);
    HAL_GPIO_WritePin(SEG_G_PORT, SEG_G_PIN, SEG_OFF);
}

void FND_Display(uint8_t digit)
{
    if (digit > 9u) return;

    uint8_t m = FND_CorrectMask(s_fnd_mask[digit]);

    /* Ghost 방지: COM 끄고 -> 세그 갱신 -> COM 켜기 */
    HAL_GPIO_WritePin(SEG_COM_PORT, SEG_COM_PIN, COM_OFF);

    HAL_GPIO_WritePin(SEG_A_PORT, SEG_A_PIN, (m & (1u<<0)) ? SEG_ON : SEG_OFF);
    HAL_GPIO_WritePin(SEG_B_PORT, SEG_B_PIN, (m & (1u<<1)) ? SEG_ON : SEG_OFF);
    HAL_GPIO_WritePin(SEG_C_PORT, SEG_C_PIN, (m & (1u<<2)) ? SEG_ON : SEG_OFF);
    HAL_GPIO_WritePin(SEG_D_PORT, SEG_D_PIN, (m & (1u<<3)) ? SEG_ON : SEG_OFF);
    HAL_GPIO_WritePin(SEG_E_PORT, SEG_E_PIN, (m & (1u<<4)) ? SEG_ON : SEG_OFF);
    HAL_GPIO_WritePin(SEG_F_PORT, SEG_F_PIN, (m & (1u<<5)) ? SEG_ON : SEG_OFF);
    HAL_GPIO_WritePin(SEG_G_PORT, SEG_G_PIN, (m & (1u<<6)) ? SEG_ON : SEG_OFF);

    HAL_GPIO_WritePin(SEG_COM_PORT, SEG_COM_PIN, COM_ON);
}
