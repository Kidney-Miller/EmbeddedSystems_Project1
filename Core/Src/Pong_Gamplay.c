/*
 * Pong_Gamplay.c
 *
 *  Created on: Sep 23, 2022
 *  Author: krmiller21
 *  Using GPIO Port B
 */


//Status - Awaiting Hardware Testing.

//Includes:
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Pong_Gameplay.h"
#include "pong_enums.h"
//ADD KEYPAD .H HERE!!

#include "snake_gameplay.h"
#include "snake_repair.h"
#include "quadknob.h"
#include "snake_enums.h"
#include "display_DOGS_102.h"

#define ERROR_DISPLAY_BAD_HEADING {{1,0},{1,1},{1,2}, {1,4},{1,5},{1,6}}
#define ERROR_DISPLAY_BLOCK_COUNT 6

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
//
void get_collision_cords(pong_game* p) {
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

//Checks to see if ball is in a dead position.
bool isBallDead(pong_game* p) {
	int8_t x = p->ball.x;
	int8_t y = p->ball.y;
	bool dead = false;
	//JPL?
	switch (x) {
		case 0: switch (y) {
			case 0: dead = true; break;
			case 1: dead = true; break;
			case 2: dead = true; break;
			case 3: dead = true; break;
			case 4: dead = true; break;
			case 5: dead = true; break;
			case 6: dead = true; break;
			case 7: dead = true; break;
			default: break;
		}
		case 7: switch (y) {
			case 0: dead = true; break;
			case 1: dead = true; break;
			case 2: dead = true; break;
			case 3: dead = true; break;
			case 4: dead = true; break;
			case 5: dead = true; break;
			case 6: dead = true; break;
			case 7: dead = true; break;
			default: break;
		}
		default: break;
	}

	return dead;
}

XY_PT nextBallCords(const pong_game* p) {
	int8_t currentx = p->ball.x;
	int8_t currenty = p->ball.y;
	XY_PT nextBall;

	switch (p->ball _heading) {
		//LEFT 
		case BALL_LEFT: nextBall.x--; break;
		// RIGHT
		case BALL_RIGHT: nextBall.x++; break;
		//UP
		case BALL_UP: nextBall.y++; break;
		case BALL_UPLEFT: nextBall.x--; nextBall.y++; break;
		case BALL_UPRIGHT: x++; y++; break;
		//DOWN
		case BALL_DOWN: nextBall.y--; break;
		case BALL_DOWNLEFT: nextBall.x--;  nextBall.y--; break;
		case BALL_DOWNRIGHT: nextBall.x++;  nextBall.y--; break;
		default: 
		
	}

	nextBall.x += x;
	nextBall.y += y;

	return nextBall;
}

void pong_periodic_play(pong_game* p) {
	// Get a fresh plot of the board to check for legal & fruit moves:
	static int8_t board[CHECKS_WIDE][CHECKS_WIDE];
	// Always clear the board and redraw it.
	for (int x = 0; x < CHECKS_WIDE; x++) {
		for (int y = 0; y < CHECKS_WIDE; y++) {
			//Set Upper Boundary for Board.
			if (x == 1 && y == 0 || x == 2 && y == 0
			  || x == 3 && y == 0 || x == 4 && y == 0 ||
			  x == 5 && y == 0 || x == 6 && y == 0) {
				board[x][y] = 1;
			}//Set Lower Boundary for Board.
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
		player1_plot(p, board);
		player2_plot(p, board);
		ball_plot(p, board);
	}
	//Check for ball collision:
	detect_collision(p, board[p->ball.x][p->ball.y]);
	//Get ball's next move?
	XY_PT next_head = nextBallCords(p);

	// Check for snake self-bite (Check to see if ball is in a dead position)
	if (isBallDead(p) && board[next_head.x][next_head.y] == 0) {
		// CRASH!
		while (1);
	}

	// Is the heading a normal move into an empty cell?
	//If Yes, just update ball cords.
	else if (board[next_head.x][next_head.y] == 0) {
		p->ball.x = next_head.x;
		p->ball.y = next_head.y;
		
	}

	//Collision
	//Flip heading and update cords with function call.
	else if (board[next_head.x][next_head.y] == 1 || p->ball_hit == ture)
	{
		p->ball.x = next_head.x;
		p->ball.y = next_head.y; //Set new cords before calculating bounce direction
		get_collision_cords(p);
	}
}





