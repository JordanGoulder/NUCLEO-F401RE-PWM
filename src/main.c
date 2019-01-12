/**
  ******************************************************************************
  * @file   main.c
  * @author Jordan Goulder
  * @brief  Fade LED2 in and out with PWM
  ******************************************************************************
*/

#include <string.h>

#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"

void error_handler();

int main(void)
{
    // Initialize the Hardware Abstraction Library
    HAL_Init();

    // Set Timer 2 to a frequency of 1 MHz
    uint32_t timerFreq = 1000000;

    // Use a PWM frequency of 20 KHz
    uint32_t pwmFreq = 20000;

    // Start with a 50% duty cycle
    float pwmDutyCycle = 0.5;

    // Initialize Timer 2
    TIM_HandleTypeDef hTimer;
    memset(&hTimer, 0, sizeof(hTimer));
    hTimer.Instance = TIM2;
    hTimer.Init.Prescaler = (SystemCoreClock / timerFreq) - 1;
    hTimer.Init.Period =  (timerFreq / pwmFreq) - 1;
    hTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
    hTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    if (HAL_TIM_PWM_Init(&hTimer) != HAL_OK)
    {
        error_handler();
    }

    // Initialize Timer 2 Channel 1 as PWM
    TIM_OC_InitTypeDef hPwm;
    memset(&hPwm, 0, sizeof(hPwm));
    hPwm.OCMode = TIM_OCMODE_PWM1;
    hPwm.OCIdleState = TIM_OCIDLESTATE_RESET;
    hPwm.Pulse = (timerFreq / pwmFreq) * pwmDutyCycle;

    if (HAL_TIM_PWM_ConfigChannel(&hTimer, &hPwm, TIM_CHANNEL_1) != HAL_OK)
    {
        error_handler();
    }

    // Start Timer 2
    if (HAL_TIM_PWM_Start(&hTimer, TIM_CHANNEL_1) != HAL_OK)
    {
        error_handler();
    }

    // Amount to change the duty cycle on each iteration
    float step = 0.01;

    // Loop forever, fading LED2 in and out by altering the PWM duty cycle
    for(;;)
    {
        // Delay to control fade speed
        HAL_Delay(10);

        // Update the duty cycle
        pwmDutyCycle += step;

        // if the duty cycle is more than 100%
        if (pwmDutyCycle > 1.0)
        {
            // Clamp duty cycle to 100%
            pwmDutyCycle = 1.0;

            // Change the fade direction
            step *= -1;
        }
        // else if the duty cycle is less than 0%
        else if (pwmDutyCycle < 0.0)
        {
            // Clamp duty cycle to 0%
            pwmDutyCycle = 0.0;

            // Change the fade direction
            step *= -1;
        }

        // Set the duty cycle
        __HAL_TIM_SET_COMPARE(&hTimer, TIM_CHANNEL_1, (1000000 / pwmFreq) * pwmDutyCycle);
    }
}

void error_handler()
{
    // Initialize the LED
    BSP_LED_Init(LED2);

    // loop forever flashing LED2
    while(1)
    {
        BSP_LED_Toggle(LED2);
        HAL_Delay(250);
    }
}
