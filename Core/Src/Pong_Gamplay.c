/*
 * Pong_Gamplay.c
 *
 *  Created on: Sep 23, 2022
 *  Author: krmiller21
 *  Using GPIO Port B
 */


//Status - Still Snake, Pong Not Working.

//Includes:
#include "Pong_Gameplay.h"

//Pong Stuff:

enum ball_directions ball_opposite_direction(enum ball_directions d) {
	switch(d) {
		//UP
		case BALL_UP: return BALL_DOWN; break;
		case BALL_UPLEFT: return BALL_UPRIGHT; break;
		case BALL_UPRIGHT: return BALL_DOWNRIGHT; break;
		//DOWN
		case BALL_DOWN: return BALL_UP; break;
		case BALL_DOWNLEFT: return BALL_UPLEFT; break;
		case BALL_DOWNRIGHT: return BALL_DOWNLEFT; break;
		//LEFT
		case BALL_LEFT: return BALL_RIGHT; break;
		//RIGHT
		case BALL_RIGHT: return BALL_LEFT; break;
		default: return d;
	}
}

bool player1_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]) {
	bool good = true;
	int8_t x = s-> player1.x;
	int8_t y = s-> player1.y;
	b[x][y] = 1; // Player 1 starts here: post a 1!

	for (int i = 0; i < (p->length - 1); i++) {
		switch (p->p1_heading) {
			case PLYR_UP: y++; break;
			case PLYR_STAY: break;
			case PLYR_DOWN: y--; break;
			default:
				for (int bc = 0; bc < ERROR_DISPLAY_BLOCK_COUNT; bc++) {
					display_dark_square(error_bar[bc].x, error_bar[bc].y);
				} //Display Error.
		}
		//X VALUE DOESNT CHANGE
		//y = (y >= 0) ? (y % CHECKS_WIDE) : (CHECKS_WIDE - 1);
		//if (b[x][y] > 0) ok = false; // overlapping vertebrae NOT ok
		//b[x][y] = i + 2;
		b[x][y] = 1;
	}

	return good;
}

bool player2_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]) {
	bool good = true;
	int8_t x = s->player2.x;
	int8_t y = s->player2.y;
	b[x][y] = 1; // Player 1 starts here: post a 1!

	for (int i = 0; i < (p->length - 1); i++) {
		switch (p->p2_heading) {
			case PLYR_UP: y++; break;
			case PLYR_STAY: break;
			case PLYR_DOWN: y--; break;
			default:
				for (int bc = 0; bc < ERROR_DISPLAY_BLOCK_COUNT; bc++) {
					display_dark_square(error_bar[bc].x, error_bar[bc].y);
				} //Display Error.
		}
		//X VALUE DOESNT CHANGE
		//y = (y >= 0) ? (y % CHECKS_WIDE) : (CHECKS_WIDE - 1);
		//if (b[x][y] > 0) ok = false; // overlapping vertebrae NOT ok
		//b[x][y] = i + 2;
		b[x][y] = 1;
	}

	return good;
}

bool ball_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]) {
	bool good = true;
	int8_t x = p->ball.x;
	int8_t y = p->ball.y;

	switch (p->ball _heading) {
		//LEFT 
		case BALL_LEFT: x--; break;
		// RIGHT
		case BALL_RIGHT: x++; break;
		//UP
		case BALL_UP: y++; break;
		case BALL_UPLEFT: x--; y++; break;
		case BALL_UPRIGHT: x++; y++; break;
		//DOWN
		case BALL_DOWN: y--; break;
		case BALL_DOWNLEFT: x--;  y--; break;
		case BALL_DOWNRIGHT: x++;  y--; break;
		default:
			for (int bc = 0; bc < ERROR_DISPLAY_BLOCK_COUNT; bc++) {
				display_dark_square(error_bar[bc].x, error_bar[bc].y);
			} //Display Error.
	
	}
	
	b[x][y] = -1; //Ball Value is -1.


	return good;
}

//Checks if ball hit player or wall
bool detect_collision(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]) {
	p->ball_hit = false;
	int8_t x = p->ball.x;
	int8_t y = p->ball.y;
	if(b[x][y] == 1) {
		p->ball_hit = true;
	}
	bool done = true;
	return done;
}

bool plot_collision(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]) {
	bool good = false;
	int8_t x = p->ball.x;
	int8_t y = p->ball.y;
	enum ball_directions newHeading = p->ball_heading;

	//Returns the bounce heading.
	newHeading = ball_opposite_direction(newHeading);

	switch (newHeading) {
	//LEFT 
	case BALL_LEFT: x--; break;
	// RIGHT
	case BALL_RIGHT: x++; break;
	//UP
	case BALL_UP: y++; break;
	case BALL_UPLEFT: x--; y++; break;
	case BALL_UPRIGHT: x++; y++; break;
	//DOWN
	case BALL_DOWN: y--; break;
	case BALL_DOWNLEFT: x--;  y--; break;
	case BALL_DOWNRIGHT: x++;  y--; break;
	default:
		for (int bc = 0; bc < ERROR_DISPLAY_BLOCK_COUNT; bc++) {
			display_dark_square(error_bar[bc].x, error_bar[bc].y);
		} //Display Error.
	}
	b[x][y] = -1; //Ball Value is -1.
	good = true;

	return good;
}

void pong_game_init(pong_game* p) {
	const XY_PT initial_player1 = { 0,3 };
	const XY_PT initial_player2 = { 7,3 };
	const XY_PT initial_ball = { 4,4 };
	const int8_t paddle_length = 3;
	
	p->player_length = paddle_length;

	//Player 1:
	p->player1.x = initial_head.x;
	p->player1.y = initial_head.y;
	p1_heading = PLYR_STAY;
	
	//Player 2:
	p->player2.x = initial_player2.x;
	p->player2.y = initial_player2.y;
	p2_heading = PLYR_STAY;

	//Ball
	p->ball.x = initial_ball.x;
	p->ball.y = initial_ball.y;
	p->heading = BALL_UPRIGHT; // Start off towards the upper right.
	p->ball_hit = false;
}

void pacify_compiler() {

}

//PLAYER HEADING UPDATES NEED MORE KEYPAD DATA BEFORE FUNCTIONAL
void player1_heading_update(pong_game* p, Smc_queue* q) {
	Q_data msg;
	bool data_available;
	data_available = q->get(q, &msg);
	if (!data_available) return;
	else {
		switch (p->p1_heading) {
		case PLYR_UP:
			p->p1_heading = (msg.buttonPressed == ONE_PRESSED) ? //NEED KEYPAD ENUM FOR THIS!
				PLYR_UP : PLYR_STAY;
			break;
		case PLYR_DOWN:
			p->p1_heading = (msg.buttonPressed == SEVEN_PRESSED) ?
				PLYR_DOWN : PLYR_STAY;
			break;
		case PLYR_STAY:
			p->p1_heading = (msg.buttonPressed == ONE_PRESSED) ?
				PLYR_UP : PLYR_DOWN;
			break;
		default: //p->heading remains unchanged. No good way to say this in C.
			pacify_compiler();
		}
	}
}

//Update Player 2 heading 
void player2_heading_update(pong_game* p, Smc_queue* q) {
	Q_data msg;
	bool data_available;
	data_available = q->get(q, &msg);
	if (!data_available) return;
	else {
		switch (p->p2_heading) {
		case PLYR_UP:
			p->p2_heading = (msg.buttonPressed == THREE_PRESSED) ? //NEED KEYPAD ENUM FOR THIS!
				PLYR_UP : PLYR_STAY;
			break;
		case PLYR_DOWN:
			p->p2_heading = (msg.buttonPressed == NINE_PRESSED) ?
				PLYR_DOWN : PLYR_STAY;
			break;
		case PLYR_STAY:
			p->p2_heading = (msg.buttonPressed == NINE_PRESSED) ?
				PLYR_UP : PLYR_DOWN;
			break;
		default: //p->heading remains unchanged. No good way to say this in C.
			pacify_compiler();
		}
	}
}

void ball_heading_update(pong_game* p) {
	
		switch (p->ball_heading) {
		case BALL_UP: 
			p->ball_heading = (p->ball_hit == false) ? BALL_UP : BALL_DOWN;
			break;
		case BALL_UPLEFT:
			p->ball_heading = (p->ball_hit == false) ? BALL_UPLEFT : BALL_UPRIGHT;
			break;
		case BALL_UPRIGHT:
			p->ball_heading = (p->ball_hit == false) ? BALL_DOWNRIGHT : BALL_UPRIGHT;
			break;
		case BALL_DOWN:
			p->ball_heading = (p->ball_hit == false) ? BALL_DOWN : BALL_UP;
			break;
		case BALL_DOWNLEFT:
			p->ball_heading = (p->ball_hit == false) ? BALL_DOWNLEFT : BALL_UPLEFT;
			break;
		case BALL_DOWNRIGHT:
			p->ball_heading = (p->ball_hit == false) ? BALL_DOWNRIGHT : BALL_DOWNLEFT;
			break;
		case BALL_DOWNRIGHT:
			p->ball_heading = (p->ball_hit == false) ? BALL_DOWNRIGHT : BALL_DOWNLEFT;
			break;
		case BALL_LEFT:
			p->ball_heading = (p->ball_hit == false) ? BALL_LEFT : BALL_RIGHT;
			break;
		case BALL_RIGHT:
			p->ball_heading = (p->ball_hit == false) ? BALL_RIGHT : BALL_LEFT;
			break;
		default: //s->heading remains unchanged. No good way to say this in C.
			pacify_compiler();
	}
}



void pong_periodic_play(pong_game* p) {
	// Get a fresh plot of the board to check for legal & fruit moves:
	static int8_t board[CHECKS_WIDE][CHECKS_WIDE];
	// Always clear the board and redraw it.
	for (int x = 0; x < CHECKS_WIDE; x++) {
		for (int y = 0; y < CHECKS_WIDE; y++) {
			if (x == 1 && y == 0 || x == 2 && y == 0
			  || x == 3 && y == 0 || x == 4 && y == 0 ||
			  x == 5 && y == 0 || x == 6 && y == 0) {
				board[x][y] = 1;
			}
			else if (x == 1 && y == 7 || x == 2 && y == 7
			  || x == 3 && y == 7 || x == 4 && y == 7 ||
			  x == 5 && y == 7 || x == 6 && y == 7) {
				board[x][y] = 1;
			} else{
				board[x][y] = 0;
			}

		}
	}
	bool ok;
	ok = player1_plot(p, board) && player2_plot(p, board) && ball_plot(p, board); // Will happen l-to-r.
	if (!ok) {
		display_checkerboard();
		for (volatile int32_t n = 0; n < BIG_DELAY_COUNT; n++);
		pong_game_init(p);
		snake_plot(p, board);
	}
	//Check for game loss here.
	XY_PT next_head = find_next_head(s);

	// Check for snake self-bite
	if (board[next_head.x][next_head.y] >= 1) {
		// CRASH!
		while (1);
	}

	// Is the heading a normal move into an empty cell?
	else if (board[next_head.x][next_head.y] == 0) {
		s->head.x = next_head.x;
		s->head.y = next_head.y;
		// Start at the last element in the spine.
		// Vertebra[n] moves forward into the cell
		// previously occuppied by Vertebra[n-1], and
		// it inherits not only the cell, but also the
		// heading of Vertebra[n-1] - so copy the heading
		// from V[n-1] to V[n], and then let the head, AKA
		// Vertebra[0], take the user-controlled Heading as
		// its direction.
		// There are length-1 Vertebrae, but only length-2
		// connections between them.
		for (int n = (s->length - 2); n > 0; n--) {
			s->vertebra[n] = s->vertebra[n - 1];
		}
		s->vertebra[0] = snake_opposite_direction(s->heading);
	}

	// Is this a move into fruit?
	else if (board[next_head.x][next_head.y] == -1)
	{
		s->length++;
		snake_place_fruit(s, (const int8_t(*)[CHECKS_WIDE]) board);
		s->head.x = next_head.x;
		s->head.y = next_head.y;
		// Slither all vertebrae fwd with a for-loop and add a new
		// head - and keep the old tail.
		for (int n = (s->length - 2); n > 0; n--) {
			s->vertebra[n] = s->vertebra[n - 1];
		}
		s->vertebra[0] = snake_opposite_direction(s->heading);
	}
}

//SNAKE CODE

// MIT License - Sean Carroll - (c) 2022
// carrolls@trine.edu 2022.7.28 - merged into project
// carrolls@trine.edu 2022.07.04
// Game "snake - Eight-by-eight board
// Board encoding: -1 = fruit. 0 = empty. 1 = head;  2 = neck, ...
// so the snake's body comprises consecutive integers.

//// STATUS - Works.

//#define UNIT_TEST_SNAKE
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "snake_gameplay.h"
#include "snake_repair.h"
#include "quadknob.h"
#include "snake_enums.h"
#include "display_DOGS_102.h"

#define ERROR_DISPLAY_BAD_HEADING {{1,0},{1,1},{1,2}, {1,4},{1,5},{1,6}}
#define ERROR_DISPLAY_BLOCK_COUNT 6

// snake_opposite_direction(dir) is a utility N<->S, E<->W. It is used
// to convert head-to-tail guidance into tail-to-head direction-of-
// movement. The snake is stored h-to-t (because a player will be
// focused on the head). But when the snake moves, each vertebra
// will move toward the head - the reverse of directions here FROM
// the head
enum snake_compass_dir snake_opposite_direction(enum snake_compass_dir d){
	switch(d){
	case SNAKE_COMPASS_N: return SNAKE_COMPASS_S; break;
	case SNAKE_COMPASS_S: return SNAKE_COMPASS_N; break;
	case SNAKE_COMPASS_W: return SNAKE_COMPASS_E; break;
	case SNAKE_COMPASS_E: return SNAKE_COMPASS_W; break;
	default: return d;
	}
}

// snake_plot (snake_game, 2-d matrix of snake_compass dirs)
// Converts the snake from a list of vertebrae into a
// matrix of directions. 0 = empty, F = fruit, and the
// compass directions = a snake segment's direction of travel.
const XY_PT error_bar[ERROR_DISPLAY_BLOCK_COUNT] = ERROR_DISPLAY_BAD_HEADING;
bool snake_plot(const snake_game *s, int8_t b[CHECKS_WIDE][CHECKS_WIDE]){
	bool ok = true;
	int8_t x = s->head.x;
	int8_t y = s->head.y;
	b[x][y] = 1; // snake starts here: post a 1!

	// Plot each vertebra.
	for (int n = 0; n < (s->length - 1); n++){
		switch(s->vertebra[n]){
			case SNAKE_COMPASS_N: y--; break;
			case SNAKE_COMPASS_S: y++; break;
			case SNAKE_COMPASS_E: x++; break;
			case SNAKE_COMPASS_W: x--; break;
			default:
				for (int bc = 0; bc < ERROR_DISPLAY_BLOCK_COUNT; bc++){
				    display_dark_square(error_bar[bc].x, error_bar[bc].y);
				}
		}
		x = (x >= 0)?(x % CHECKS_WIDE):(CHECKS_WIDE-1);
		y = (y >= 0)?(y % CHECKS_WIDE):(CHECKS_WIDE-1);
		if (b[x][y] > 0) ok = false; // overlapping vertebrae NOT ok
		b[x][y] = n+2;
	}
	return ok;
}

bool fruit_plot(const snake_game *s, int8_t b[CHECKS_WIDE][CHECKS_WIDE]){
	// If the fruit already plotted, or the ground is clear = ok!
	bool ok = true;
	if (b[s->fruit.x][s->fruit.y] == -1) ok = true;
	else if (b[s->fruit.x][s->fruit.y] == 0){
		ok = true;
		b[s->fruit.x][s->fruit.y] = -1;
	}
	else{
		ok = false;
	}
	return ok;
}


// snake_board_init (obvious - sets head, tail, vertebra directions & length, first fruit)
void snake_game_init(snake_game* s){
	const XY_PT initial_head = {3,3};
	const XY_PT initial_fruit = {3,6};
	const int8_t initial_length = 3;
	// JPL DEVIATION - ACCEPT WARNING: "Avoid constant literals"
	// Excuse -- The snake's initial position is (1) initialization,
	// not operation; and (2) it is arbitrary, not critical -- so defining
	// named constants for this is silly. Just be sure that each
	// coordinate is within {0 ... CHECKS_WIDE-1}.
	s->head.x = initial_head.x;
	s->head.y = initial_head.y;
	s->vertebra[0] = SNAKE_COMPASS_N; // This is the head. The neck is NORTH
	s->vertebra[1] = SNAKE_COMPASS_W; // The body @ 1. The tail is WEST.
	s->length = initial_length;
	s->fruit.x = initial_fruit.x;
	s->fruit.y = initial_fruit.y;
	s->heading = SNAKE_COMPASS_S; // Start off toward the bottom.
}


// snake_heading_update(game-pointer, queue-pointer) will adjust the
// (.heading) field of a snake in response to a message from the user
// as conveyed on the queue.
void snake_heading_update(snake_game* s, Smc_queue* q){
	Q_data msg;
	bool data_available;
	data_available = q->get(q, &msg);
    if (!data_available) return;
    else{
    	switch(s->heading){
    	case SNAKE_COMPASS_N:
    		s->heading = (msg.twist == QUADKNOB_CW)?
    					 SNAKE_COMPASS_E:SNAKE_COMPASS_W;
    		break;
    	case SNAKE_COMPASS_E:
    		s->heading = (msg.twist == QUADKNOB_CW)?
    					 SNAKE_COMPASS_S:SNAKE_COMPASS_N;
    		break;
    	case SNAKE_COMPASS_S:
    		s->heading = (msg.twist == QUADKNOB_CW)?
    					 SNAKE_COMPASS_W:SNAKE_COMPASS_E;
    		break;
    	case SNAKE_COMPASS_W:
    		s->heading = (msg.twist == QUADKNOB_CW)?
    					 SNAKE_COMPASS_N:SNAKE_COMPASS_S;
    		break;
    	default: //s->heading remains unchanged. No good way to say this in C.
    		pacify_compiler();
    	}
    }
}



void snake_periodic_play(snake_game* s){
	// Get a fresh plot of the board to check for legal & fruit moves:
	static int8_t board[CHECKS_WIDE][CHECKS_WIDE];
	// Always clear the board and redraw it.
	for (int x = 0; x < CHECKS_WIDE; x++){
		for (int y = 0; y < CHECKS_WIDE; y++){
			board[x][y] = 0;
		}
	}
	bool ok;
	ok = snake_plot(s, board) && fruit_plot(s, board); // Will happen l-to-r.
	if (!ok) {
		display_checkerboard();
		for (volatile int32_t n = 0 ; n< BIG_DELAY_COUNT; n++);
		snake_game_init(s);
		snake_plot(s, board);
	}

	XY_PT next_head = find_next_head(s);

	// Check for snake self-bite
	if (board[next_head.x][next_head.y] >= 1){
		// CRASH!
		while(1);
	}

	// Is the heading a normal move into an empty cell?
	else if (board[next_head.x][next_head.y] == 0){
		s->head.x = next_head.x;
		s->head.y = next_head.y;
		// Start at the last element in the spine.
		// Vertebra[n] moves forward into the cell
		// previously occuppied by Vertebra[n-1], and
		// it inherits not only the cell, but also the
		// heading of Vertebra[n-1] - so copy the heading
		// from V[n-1] to V[n], and then let the head, AKA
		// Vertebra[0], take the user-controlled Heading as
		// its direction.
		// There are length-1 Vertebrae, but only length-2
		// connections between them.
		for (int n = (s->length - 2); n>0; n--){
			s->vertebra[n] = s->vertebra[n-1];
		}
		s->vertebra[0] = snake_opposite_direction(s->heading);
	}

	// Is this a move into fruit?
	else if (board[next_head.x][next_head.y] == -1)
	{
		s->length++;
		snake_place_fruit(s, (const int8_t(*)[CHECKS_WIDE]) board);
		s->head.x = next_head.x;
		s->head.y = next_head.y;
		// Slither all vertebrae fwd with a for-loop and add a new
		// head - and keep the old tail.
		for (int n = (s->length - 2); n>0; n--){
			s->vertebra[n] = s->vertebra[n-1];
		}
		s->vertebra[0] = snake_opposite_direction(s->heading);
	}
}




