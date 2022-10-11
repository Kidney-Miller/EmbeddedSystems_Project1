/*
 * pong_main.c
 *
 *  Created on: Oct 11, 2022
 *      Author: krmiller21
 */

//Pong Includes
#include "Pong_Gameplay.h"
#include "pong_enums.h"
//Snake Includes
#include <stdint.h>
#include <stdlib.h> // rand
#include <stdbool.h>
#include <cmsis_gcc.h>
#include "main.h"
#include "snake_main.h"
#include "snake_gameplay.h"
#include "display_DOGS_102.h"
#include "snake_enums.h"
#include "quadknob.h"
#include "smc_queue.h"
#include "show_snake.h"

///////////////////////////
// Test -without input, the expected output =?
// Without_Input - Testing
#define TEST_WITHOUT_INPUT



extern volatile int32_t timer_isr_countdown; // Required to control timing
const int snake_board_size = CHECKS_WIDE; // Provided for extern

void ram_health(uint16_t dummy_var, uint16_t pattern){
	// DEBUGGING PHASE: LOCK UP THE PROGRAM if RAM is corrupted.
	if (dummy_var != pattern){
		while(1);
	}
}


void snake_main(void){
	const int32_t timer_isr_500ms_restart = 500;
	const int32_t timer_isr_2000ms_restart = 2000;

	// INITIALIZE THE GAME
	// Construct the model "game" object:
	pong_game my_game;
	volatile uint16_t ram_dummy_1 = MEMORY_BARRIER_1;
	pong_game_init(&my_game);

	// Construct IPC
	Smc_queue keyboard_q;
	volatile uint16_t ram_dummy_2 = MEMORY_BARRIER_2;
	smc_queue_init(&keyboard_q);

	// Input object NEEDS REPLACED WITH KEYBOARD!!!
	QuadKnob user_knob_1;
	volatile uint16_t ram_dummy_3 = MEMORY_BARRIER_3;
	quadknob_init(&user_knob_1);

	// Output object
	// Block all interrupts while initializing - initial protocol timing is critical.
	__disable_irq();
	display_init();
	__enable_irq();

	// Welcome screen = checkerboard for 2 seconds.
	timer_isr_countdown = timer_isr_2000ms_restart;
	display_checkerboard();
	while (timer_isr_countdown > 0){}
	timer_isr_countdown = timer_isr_500ms_restart;
	// Confirm all the rules and paint the initial snake.
	display_blank();
	//snake_game_cleanup(&my_game);

	// OPERATE THE GAME
	int32_t prior_timer_countdown = timer_isr_countdown;

	while(1){
		ram_health(ram_dummy_1, MEMORY_BARRIER_1);
		ram_health(ram_dummy_2, MEMORY_BARRIER_2);
		ram_health(ram_dummy_3, MEMORY_BARRIER_3);

	// ASSERT TIMER COUNTDOWN IN RANGE
		if ((timer_isr_countdown > timer_isr_500ms_restart)||
				(timer_isr_countdown < 0)){
			display_checkerboard();
			while(1);
		}

#ifndef TEST_WITH_INPUT
		// Check for user input every 1 ms & paint one block of the display.
		if (prior_timer_countdown != timer_isr_countdown ){
			prior_timer_countdown = timer_isr_countdown;
			// If time changed, about 1 ms has elapsed.
			// Once each 1 ms, read input pins from user knob and then
			// update "knob" object (which debounces each input pin and
			// then calculates user command).

			bool user_knob_1_pin_A = (GPIO_PIN_SET == HAL_GPIO_ReadPin(QuadKnobA_GPIO_Port, QuadKnobA_Pin));
			bool user_knob_1_pin_B = (GPIO_PIN_SET == HAL_GPIO_ReadPin(QuadKnobB_GPIO_Port, QuadKnobB_Pin));
			user_knob_1.update(&user_knob_1, user_knob_1_pin_A, user_knob_1_pin_B);

			// Get user command from "knob" - if any action, make it a queue packet and then mail it.
			if (user_knob_1.get(&user_knob_1) != QUADKNOB_STILL){
				Q_data command_packet;
				command_packet.twist = user_knob_1.get(&user_knob_1);
				keyboard_q.put(&keyboard_q, &command_packet);
			}
			snake_heading_update(&my_game, &keyboard_q);
		// ASSERT HEADING IS VALID
			while ((my_game.heading != SNAKE_COMPASS_N)&&
					(my_game.heading != SNAKE_COMPASS_E)&&
					(my_game.heading != SNAKE_COMPASS_S)&&
					(my_game.heading != SNAKE_COMPASS_W));
			incremental_show_snake((const snake_game *)&my_game, false);
		}
		if (timer_isr_countdown <= 0) {
			// Move and animate every 500 ms
			timer_isr_countdown = timer_isr_500ms_restart;
			snake_periodic_play(&my_game);
			incremental_show_snake(&my_game, true);
		}
#endif
#ifdef TEST_WITHOUT_INPUT
		static int turns = 0;
		// Normally "check for user input every 1 ms & show" - here just update display
		if (prior_timer_countdown != timer_isr_countdown ){
			prior_timer_countdown = timer_isr_countdown;
			incremental_show_pong(&my_game, false);
		}
		if (timer_isr_countdown <= 0) {
			// Move and animate every 500 ms
			timer_isr_countdown = timer_isr_500ms_restart;
			if (turns < 3){
				turns ++;
				pong_periodic_play(&my_game);
			}
			else {
				turns = 0;
				Q_data command_packet_p1 = {.p1_buttonPressed = ONE_PRESSED};
				Q_data command_packet_p2 = {.p1_buttonPressed = THREE_PRESSED};
				keyboard_q.put(&keyboard_q, &command_packet_p1);
				keyboard_q.put(&keyboard_q, &command_packet_p2);
				player1_heading_update(&my_game, &keyboard_q);
				player2_heading_update(&my_game, &keyboard_q);
				pong_periodic_play(&my_game);
			}
			incremental_show_pong(&my_game, true);
		}
#endif
	}
}
