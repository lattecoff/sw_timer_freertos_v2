/* Includes ------------------------------------------------------------------*/
#include "FreeRTOSConfig.h"

#include "stm32l4xx_hal.h"

#include "main.h"

#include "FreeRTOS.h"

#include "timers.h"

#include "stdbool.h"

#include "led_control.h"
/* Private define ------------------------------------------------------------*/


/* Extern variables ---------------------------------------------------------*/

/* Private variables --------------------------------------------------------*/
led_t Led;
/* Public Functions ---------------------------------------------------------*/

void led_on(void)
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}


void led_off(void)
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void led_toggle(void)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

/**
 * @brief  Led flash series: 3 + 1 (3 - flash, 1 - pause).
 */
void my_led_task(void)
{
	static uint8_t flashSeries = 0;


	if (Led.xSwTimerLed == NULL)
	{
		Led.xSwTimerLed = xTimerCreate ("SwTimerLed", pdMS_TO_TICKS(300), pdTRUE, 0, my_led_task);
		led_on();
	}
	else
	{
		flashSeries++;
		if ((flashSeries & 1) == 0)
		{
			led_on();
		}
		else
		{
			if (flashSeries <= 5)
			{
				xTimerChangePeriod( Led.xSwTimerLed, pdMS_TO_TICKS(400), 100 );
			}
			else
			{
				flashSeries = 0;
				xTimerChangePeriod( Led.xSwTimerLed, pdMS_TO_TICKS(3000), 100 );
			}

			led_off();
		}
	}
}
