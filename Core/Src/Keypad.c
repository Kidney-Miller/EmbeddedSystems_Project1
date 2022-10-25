/*
 * Keypad.c
 *
 *  Created on: Oct 11, 2022
 *      Author: cdgarcia20
 */

/*
 * From Left to Right on the Keypad
 * Row 1 = PB4
 * Row 3 = PB6
 * Column 1 = PB5
 * Column 3 = PB3
 */
#include "Keypad.h"
#include "pong_enums.h"
#include "main.h"
char get_keypd_key(void)
{
	char key_val = 'N';
	if((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET))
	{
		key_val = '1';
	}
	if((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET))
	{
		key_val = '3';
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); //Row 1 on standby
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // Row 3 on standby

	if((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET))
	{
		key_val = '7';
	}
	if((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET))
	{
		key_val = '9';
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // Row 1 Being Checked
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	return key_val;
}

enum keypad_buttonPress keypd_translate(char key)
{
	switch(key){
	case '1':
		return ONE_PRESSED;
		break;
	case '3':
		return THREE_PRESSED;
		break;
	case '7':
		return SEVEN_PRESSED;
		break;
	case '9':
		return NINE_PRESSED;
		break;
	default:
		return ZERO_PRESSED;
		break;
	}
}
