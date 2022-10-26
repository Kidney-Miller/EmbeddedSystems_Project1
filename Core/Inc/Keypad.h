/*
 * Keypad.h
 *
 *  Created on: Oct 12, 2022
 *      Author: cdgarcia20
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_
#include "pong_enums.h"

char get_keypd_key(void);
enum keypad_buttonPress keypd_translate(char key);

#endif /* KEYPAD_H_ */
