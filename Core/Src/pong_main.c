/*
 * pong_main.c
 *
 *  Created on: Oct 11, 2022
 *      Author: krmiller21
 */

//Pong Includes
#include "Pong_Gameplay.h"
#include "pong_enums.h"
#include "show_pong.h"
#include "smc_queue.h"
#include "Keypad.h"
#include "display_DOGS_102.h"
#include "SoundDriver.h"

#include <stdint.h>
#include <stdlib.h> // rand
#include <stdbool.h>
#include <cmsis_gcc.h>
#include "main.h"
//Snake Includes
//#include "snake_main.h"
//#include "snake_gameplay.h"
//#include "snake_enums.h"
//#include "quadknob.h"
//#include "show_snake.h"

///////////////////////////
// Test -without input, the expected output =?
// Without_Input - WORKING_PONG
//#define TEST_WITHOUT_INPUT
//#define TEST_WITH_INPUT
#define WORKING_PONG



extern volatile int32_t timer_isr_countdown; // Required to control timing
const int snake_board_size = CHECKS_WIDE; // Provided for extern

void ram_health(uint16_t dummy_var, uint16_t pattern){
	// DEBUGGING PHASE: LOCK UP THE PROGRAM if RAM is corrupted.
	if (dummy_var != pattern){
		while(1);
	}
}


void pong_main(void){
	const int32_t timer_isr_500ms_restart = 500;
	const int32_t timer_isr_2000ms_restart = 2000;

	// INITIALIZE THE GAME
	// Construct the model "game" object:
	pong_game my_game;
	volatile uint16_t ram_dummy_1 = MEMORY_BARRIER_1;
	pong_game_init(&my_game);

	// Construct IPC (QUEUE)
	Smc_queue keyboard_q;
	volatile uint16_t ram_dummy_2 = MEMORY_BARRIER_2;
	smc_queue_init(&keyboard_q);

	// Input object NEEDS REPLACED WITH KEYBOARD!!!
	SoundDriver buzzer;
	volatile uint16_t ram_dummy_3 = MEMORY_BARRIER_3;
	bool buzzer_hit = false;
	SoundDriver_init(&buzzer);
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);

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

#ifdef TEST_WITH_INPUT
		//Testing for Pong with Keypad.
		char key;
		enum keypad_buttonPress change;
				static int turns = 0;
				// Normally "check for user input every 1 ms & show" - here just update display
				if (prior_timer_countdown != timer_isr_countdown ){
					prior_timer_countdown = timer_isr_countdown;
					incremental_show_pong(&my_game, false);
					buzzer_hit = my_game.ball_hit;
					key = get_keypd_key();
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
						//key = get_keypd_key();
						change = keypd_translate(key);

						Q_data command_packet_p1 = {.p1_buttonPressed = change};
						Q_data command_packet_p2 = {.p2_buttonPressed = change};
						keyboard_q.put(&keyboard_q, &command_packet_p1);
						keyboard_q.put(&keyboard_q, &command_packet_p2);

						player1_heading_update(&my_game, &keyboard_q);
						player2_heading_update(&my_game, &keyboard_q);

						pong_periodic_play(&my_game);
					}
					SoundDriver_update (&buzzer, &buzzer_hit);
					incremental_show_pong(&my_game, true);
				}
#endif

#ifdef TEST_WITHOUT_INPUT
		//Testing without input.
		static int turns = 0;
		// Normally "check for user input every 1 ms & show" - here just update display
		if (prior_timer_countdown != timer_isr_countdown ){
			prior_timer_countdown = timer_isr_countdown;
			incremental_show_pong(&my_game, false);
			buzzer_hit = my_game.ball_hit;
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

				Q_data command_packet_p1 = {.p1_buttonPressed = ZERO_PRESSED};
				Q_data command_packet_p2 = {.p2_buttonPressed = ZERO_PRESSED};
				keyboard_q.put(&keyboard_q, &command_packet_p1);
				keyboard_q.put(&keyboard_q, &command_packet_p2);

				player1_heading_update(&my_game, &keyboard_q);
				player2_heading_update(&my_game, &keyboard_q);
				//ball_heading_update(&my_game);
				pong_periodic_play(&my_game);
			}
			//buzzer_hit = my_game.ball_hit;
			SoundDriver_update (&buzzer, &buzzer_hit);
			incremental_show_pong(&my_game, true);
		}
#endif

#ifdef WORKING_PONG
		//Normal Operation with keypad, circle queue, and buzzer.
		char key;
		enum keypad_buttonPress change;
		// Check for user input every 1 ms & paint one block of the display.
				if (prior_timer_countdown != timer_isr_countdown ){
					prior_timer_countdown = timer_isr_countdown;
					buzzer_hit = my_game.ball_hit;
					// If time changed, about 1 ms has elapsed.
					// Once each 1 ms, read input pins from user

					key = get_keypd_key();
					change = keypd_translate(key);


					// Get user command from "keyboard" - if any action, make it a queue packet and then mail it.
					if (change != ZERO_PRESSED){
						Q_data command_packet_POne, command_packet_PTwo;

						command_packet_POne.buttonPressed = change;
						command_packet_PTwo.buttonPressed = change;
						keyboard_q.put(&keyboard_q, &command_packet_POne);
						keyboard_q.put(&keyboard_q, &command_packet_PTwo);


					}

					//HEADING UPDATES!!
					player1_heading_update(&my_game, &keyboard_q);
					player2_heading_update(&my_game, &keyboard_q);

					incremental_show_pong(&my_game, false);
				}
				if (timer_isr_countdown <= 0) {
					// Move and animate every 500 ms
					timer_isr_countdown = timer_isr_500ms_restart;
					pong_periodic_play(&my_game);
					SoundDriver_update (&buzzer, &buzzer_hit);
					incremental_show_pong(&my_game, true);

				}
#endif



	}
}
