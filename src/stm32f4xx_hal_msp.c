/**
  ******************************************************************************
  * @file   stm32fxx_hal_msp.c
  * @author Jordan Goulder
  * @brief  Implements MCU Support Package functions that perform low level
  *         peripheral initialization. This functions are called by the HAL.
  ******************************************************************************
*/

#include "stm32f4xx_hal.h"

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    // Timer 2 Channel 1 pin is on PA5
    // Enable GPIO Port A clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Enable Timer 2 clock
    __HAL_RCC_TIM2_CLK_ENABLE();

    // Configure GPIO PWM pin
    GPIO_InitTypeDef gpioInit;
    gpioInit.Pin = GPIO_PIN_5;
    gpioInit.Mode =  GPIO_MODE_AF_PP;
    gpioInit.Alternate = GPIO_AF1_TIM1;
    gpioInit.Pull = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIOA, &gpioInit);
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim)
{
    // Reset Timer 2
    __HAL_RCC_TIM2_FORCE_RESET();
    __HAL_RCC_TIM2_RELEASE_RESET();

    // Put GPIO pins back to default state
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
}
