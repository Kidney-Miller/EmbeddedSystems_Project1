/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Keypad.c
  * @brief          : Keypad Control File
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "Keypad.h"

char get_keypd_key(void)
{
    char key_val = 'N';
    GPIOB->ODR |= (1<<6);//Change for GPIOB = ROW 1
    if(GPIOB->ODR == 0x0030){
        key_val = '1';
    }
    if(GPIOB->ODR == 0x0018){
        key_val = '3';
    }
    GPIOB->ODR &= ~(1<<6);
    GPIOB->ODR |= (1<<4); //Change for GPIOB = ROW 3
    if(GPIOB->ODR == 0x0060){ //Column 1
        key_val = '7';
    }
    if(GPIOB->ODR == 0x0049){ // Column 3
        key_val = '9';
    }
    GPIOB->ODR &= ~(1<<4);
    return key_val;
}
