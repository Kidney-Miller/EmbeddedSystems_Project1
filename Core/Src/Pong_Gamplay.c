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
#include "Keypad.h"

//#include "snake_gameplay.h"
//#include "snake_repair.h"
//#include "quadknob.h"
//#include "snake_enums.h"
#include "display_DOGS_102.h"

#define ERROR_DISPLAY_BAD_HEADING {{1,0},{1,1},{1,2}, {1,4},{1,5},{1,6}}
#define ERROR_DISPLAY_BLOCK_COUNT 6

//Pong Stuff:
const XY_PT error_bar[ERROR_DISPLAY_BLOCK_COUNT] = ERROR_DISPLAY_BAD_HEADING;

void ball_opposite_direction(pong_game* p) {
	switch(p->ball_heading) {
		//UP
		case BALL_UP: p->ball_heading = BALL_DOWN; break;
		case BALL_UPLEFT: p->ball_heading = BALL_UPRIGHT; break;
		case BALL_UPRIGHT: p->ball_heading = BALL_UPLEFT; break;
		//DOWN
		case BALL_DOWN: p->ball_heading = BALL_UP; break;
		case BALL_DOWNLEFT: p->ball_heading = BALL_UPLEFT; break;
		case BALL_DOWNRIGHT: p->ball_heading = BALL_DOWNLEFT; break;
		//LEFT
		case BALL_LEFT: p->ball_heading = BALL_RIGHT; break;
		//RIGHT
		case BALL_RIGHT: p->ball_heading = BALL_LEFT; break;
		default: break;
	}
}

bool player1_plot(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]) {
	bool good = true;
	int8_t x = p-> player1.x;
	int8_t y = p-> player1.y;
	b[x][y] = 1; // Player 1 starts here: post a 1!

	for (int i = 0; i < (p->player_length - 1); i++) {
		switch (p->p1_heading) {
			case PLYR_UP: y++; break;
			case PLYR_STAY: b[x][y+1] = 1; break;
			case PLYR_DOWN: y--; break;
			default:
				for (int bc = 0; bc < ERROR_DISPLAY_BLOCK_COUNT; bc++) {
					display_dark_square(error_bar[bc].x, error_bar[bc].y);
				} //Display Error.
		}
		//X VALUE DOESNT CHANGE
		if(y >= 7) {
			y = 7;
		} else if(y <= 0) {
			y = 0;
		}
		b[x][y] = 1;
	}

	return good;
}

bool player2_plot(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]) {
	bool good = true;
	int8_t x = p->player2.x;
	int8_t y = p->player2.y;
	b[x][y] = 2; // Player 2 starts here: post a 2!

	for (int i = 0; i < (p->player_length - 1); i++) {
			switch (p->p2_heading) {
				case PLYR_UP: y++; break;
				case PLYR_STAY: b[x][y+1] = 2; break;
				case PLYR_DOWN: y--; break;
				default:
					for (int bc = 0; bc < ERROR_DISPLAY_BLOCK_COUNT; bc++) {
						display_dark_square(error_bar[bc].x, error_bar[bc].y);
					} //Display Error.
			}
			//X VALUE DOESNT CHANGE
			if(y >= 7) {
				y = 7;
			} else if(y <= 0) {
				y = 0;
			}
			b[x][y] = 2;
		}

	return good;
}

bool ball_plot(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]) {
	bool good = true;
	int16_t x = p->ball.x;
	int16_t y = p->ball.y;

	switch (p->ball_heading) {
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

	if(y >= CHECKS_WIDE) {y = CHECKS_WIDE - 1;}
	else if(y < 0) {y = 0;}

	if(x >= CHECKS_WIDE){x = CHECKS_WIDE - 1;}
	else if(x < 0) {x = 0;}


	b[x][y] = -1; //Ball Value is -1.

	return good;
}

//Calculates the future ball's cords.
bool futureBall_cords(pong_game* p) {
	bool good = true;
	int16_t x = p->future_ball.x;
	int16_t y = p->future_ball.y;

	switch (p->ball_heading) {
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

	p->future_ball.x = x;
	p->future_ball.y = y;
	return good;
}

//Checks if ball hit player or wall based off next move.
void detect_collision(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]) {
	p->ball_hit = false;
	int8_t x = p->future_ball.x;
	int8_t y = p->future_ball.y;
	if(b[x][y] > 0) {
		p->ball_hit = true;
	}
}

//Gets the next X + Y value for player 1, based off heading.
void update_P1_XY(pong_game* p) {
	XY_PT next_move = p->player1;

	switch(p->p1_heading){
		case PLYR_UP: next_move.y++; break;
		case PLYR_STAY: break;
		case PLYR_DOWN: next_move.y--; break;
		default:
			for (int bc = 0; bc < ERROR_DISPLAY_BLOCK_COUNT; bc++) {
				display_dark_square(error_bar[bc].x, error_bar[bc].y);
			} //Display Error.
	}

	//X VALUE DOESNT CHANGE
	if(next_move.y >= 7) {
		next_move.y = 7;
	} else if(next_move.y <= 0) {
		next_move.y = 0;
	}

	p->player1.y = next_move.y;

}

//Gets the next X + Y value for player 2, based off heading.
void update_P2_XY(pong_game* p) {
	XY_PT next_move = p->player2;

	switch(p->p2_heading){
		case PLYR_UP: next_move.y++; break;
		case PLYR_STAY: break;
		case PLYR_DOWN: next_move.y--; break;
		default:
			for (int bc = 0; bc < ERROR_DISPLAY_BLOCK_COUNT; bc++) {
				display_dark_square(error_bar[bc].x, error_bar[bc].y);
			} //Display Error.
	}

	//X VALUE DOESNT CHANGE
	if(next_move.y >= 7) {
		next_move.y = 7;
	} else if(next_move.y <= 0) {
		next_move.y = 0;
	}

	p->player2.y = next_move.y;

}


//Gets the next X + Y value for the ball, based off heading.
void update_ball_XY(pong_game* p) {
	XY_PT next_move = p->ball;

	switch (p->ball_heading) {
			//LEFT
			case BALL_LEFT: next_move.x--; break;

			// RIGHT
			case BALL_RIGHT: next_move.x++; break;

			//UP
			case BALL_UP: next_move.y++; break;
			case BALL_UPLEFT: next_move.x--; next_move.y++; break;
			case BALL_UPRIGHT: next_move.x++; next_move.y++; break;

			//DOWN
			case BALL_DOWN: next_move.y--; break;
			case BALL_DOWNLEFT: next_move.x--;  next_move.y--; break;
			case BALL_DOWNRIGHT: next_move.x++;  next_move.y--; break;

			default:
				for (int bc = 0; bc < ERROR_DISPLAY_BLOCK_COUNT; bc++) {
					display_dark_square(error_bar[bc].x, error_bar[bc].y);
				} //Display Error.

		}
	//If y to big/small fix it.
	if(next_move.y >= CHECKS_WIDE) {next_move.y = CHECKS_WIDE - 1;}
	else if(next_move.y < 0) {next_move.y = 0;}
	//If x to big/small fix it.
	if(next_move.x >= CHECKS_WIDE){next_move.x = CHECKS_WIDE - 1;}
	else if(next_move.x < 0) {next_move.x = 0;}

	p->ball = next_move;

}

void pong_game_init(pong_game* p) {
	const XY_PT initial_player1 = { 0,3 };
	const XY_PT initial_player2 = { 7,4 };
	const XY_PT initial_ball = { 4,1 };
	const int8_t paddle_length = 2;

	p->player_length = paddle_length;

	//Player 1:
	p->player1.x = initial_player1.x;
	p->player1.y = initial_player1.y;
	p->p1_heading = PLYR_STAY;

	//Player 2:
	p->player2.x = initial_player2.x;
	p->player2.y = initial_player2.y;
	p->p2_heading = PLYR_STAY;

	//Ball
	p->ball.x = initial_ball.x;
	p->ball.y = initial_ball.y;
	p->future_ball.x = initial_ball.x + 1;
	p->future_ball.y = initial_ball.y + 1;
	p->ball_heading = BALL_UPRIGHT; // Start off towards the upper right.
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
			//p->p1_heading = (msg.p1_buttonPressed == ONE_PRESSED) ? PLYR_UP : PLYR_STAY;
			if(msg.p1_buttonPressed == ONE_PRESSED) {p->p1_heading = PLYR_UP;}
			else if(msg.p1_buttonPressed == SEVEN_PRESSED) {p->p1_heading = PLYR_DOWN;}
			else {p->p1_heading = PLYR_STAY;}
			break;
		case PLYR_DOWN:
			//p->p1_heading = (msg.p1_buttonPressed == SEVEN_PRESSED) ? PLYR_DOWN : PLYR_STAY;
			if(msg.p1_buttonPressed == SEVEN_PRESSED) {p->p1_heading = PLYR_DOWN;}
			else if(msg.p1_buttonPressed == ONE_PRESSED) {p->p1_heading = PLYR_UP;}
			else {p->p1_heading = PLYR_STAY;}
			break;
		case PLYR_STAY:
			//p->p1_heading = (msg.p1_buttonPressed == ONE_PRESSED) ? PLYR_UP : PLYR_DOWN;
			if(msg.p1_buttonPressed == ONE_PRESSED) {p->p1_heading = PLYR_UP;}
			else if(msg.p1_buttonPressed == SEVEN_PRESSED) {p->p1_heading = PLYR_DOWN;}
			else {p->p1_heading = PLYR_STAY;}
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
			//p->p2_heading = (msg.p2_buttonPressed == THREE_PRESSED) ? PLYR_UP : PLYR_STAY;
			if(msg.p2_buttonPressed == THREE_PRESSED) {p->p2_heading = PLYR_UP;}
			else if(msg.p2_buttonPressed == NINE_PRESSED) {p->p2_heading = PLYR_DOWN;}
			else {p->p2_heading = PLYR_STAY;}
			break;
		case PLYR_DOWN:
			//p->p2_heading = (msg.p2_buttonPressed == NINE_PRESSED) ?PLYR_DOWN : PLYR_STAY;
			if(msg.p2_buttonPressed == NINE_PRESSED) {p->p2_heading = PLYR_DOWN;}
			else if(msg.p2_buttonPressed == THREE_PRESSED) {p->p2_heading = PLYR_UP;}
			else {p->p2_heading = PLYR_STAY;}
			break;
		case PLYR_STAY:
			//p->p2_heading = (msg.p2_buttonPressed == NINE_PRESSED) ?PLYR_UP : PLYR_DOWN;
			if(msg.p2_buttonPressed == THREE_PRESSED) {p->p2_heading = PLYR_UP;}
			else if(msg.p2_buttonPressed == NINE_PRESSED) {p->p2_heading = PLYR_DOWN;}
			else {p->p2_heading = PLYR_STAY;}
			break;
		default: //p->heading remains unchanged. No good way to say this in C.
			pacify_compiler();
		}
	}
}
//Ball heading update. (OLD)
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
	//JPL
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

void nextBallCords(pong_game* p) {
	int8_t currentx = p->ball.x;
	int8_t currenty = p->ball.y;
	XY_PT nextBall;
	nextBall.x = 0;
	nextBall.y = 0;

	switch (p->ball_heading) {
		//LEFT
		case BALL_LEFT: nextBall.x--; break;
		// RIGHT
		case BALL_RIGHT: nextBall.x++; break;
		//UP
		case BALL_UP: nextBall.y++; break;
		case BALL_UPLEFT: nextBall.x--; nextBall.y++; break;
		case BALL_UPRIGHT: nextBall.x++; nextBall.y++; break;
		//DOWN
		case BALL_DOWN: nextBall.y--; break;
		case BALL_DOWNLEFT: nextBall.x--;  nextBall.y--; break;
		case BALL_DOWNRIGHT: nextBall.x++;  nextBall.y--; break;
		default: break;

	}

	nextBall.x += currentx;
	nextBall.y += currenty;
	//If nextBall.x Out of Bounds.
	if(nextBall.x >= CHECKS_WIDE) {
		nextBall.x = CHECKS_WIDE - 1;
	} else if(nextBall.x <= 0) {
		nextBall.x = 0;
	}
	//If nextBall.y Out of Bounds.
	if(nextBall.y >= CHECKS_WIDE) {
		nextBall.y = CHECKS_WIDE - 1;
	} else if(nextBall.y <= 0) {
		nextBall.y = 0;

	}

	p->future_ball.x = nextBall.x;
	p->future_ball.y = nextBall.y;
}

void pong_periodic_play(pong_game* p) {
	// Get a fresh plot of the board to check for legal & ball moves:
	static int8_t board[CHECKS_WIDE][CHECKS_WIDE];
	// Always clear the board and redraw it.
	for (int x = 0; x < CHECKS_WIDE; x++) {
		for (int y = 0; y < CHECKS_WIDE; y++) {
			//Set Lower Boundary for Board.
			if ((x == 1 && y == 0) || (x == 2 && y == 0)
			  || (x == 3 && y == 0) || (x == 4 && y == 0) ||
			  (x == 5 && y == 0) || (x == 6 && y == 0)) {
				board[x][y] = 3;
			}//Set Upper Boundary for Board.
			else if ((x == 1 && y == 7) || (x == 2 && y == 7)
			  || (x == 3 && y == 7) || (x == 4 && y == 7) ||
			  (x == 5 && y == 7) || (x == 6 && y == 7)) {
				board[x][y] = 4;
			} else{
				board[x][y] = 0;
			}

		}
	}
	bool ok;
	detect_collision(p, board);
	ok = player1_plot(p, board) && player2_plot(p, board) && ball_plot(p, board); //NEEDS JPL FIXIN'
	if (!ok) {
		display_checkerboard();
		for (volatile int32_t n = 0; n < BIG_DELAY_COUNT; n++);
		pong_game_init(p);
		player1_plot(p, board);
		player2_plot(p, board);
		ball_plot(p, board);
	}

	//Get ball's next move?
	update_P1_XY(p);
	update_P2_XY(p);
	update_ball_XY(p);


	nextBallCords(p);
	bool deadBall = isBallDead(p);

	// Check to see if ball is in a dead position.
	if ((deadBall == true) && p->ball_hit == false) {//(board[p->future_ball.x][p->future_ball.y] == 0)) {
		// CRASH!
		while (1);
	}

	//Collision
	//PLAYER ONE:
	if(p->future_ball.x == 0 && board[p->future_ball.x][p->future_ball.y] == 1) {
		hit_player_one(p);
		nextBallCords(p);
		p->ball.x = p->future_ball.x;
		p->ball.y = p->future_ball.y;

	//PLAYER TWO:
	} else if(p->future_ball.x == 7 && board[p->future_ball.x][p->future_ball.y] == 2){
		hit_player_two(p);
		nextBallCords(p);
		p->ball.x = p->future_ball.x;
		p->ball.y = p->future_ball.y;

	//LOWER WALL:
	} else if(p->future_ball.y == 0 && board[p->future_ball.x][p->future_ball.y] == 3){
		hit_lower_wall(p);
		nextBallCords(p);
		p->ball.x = p->future_ball.x;
		p->ball.y = p->future_ball.y;

	//UPPER WALL:
	} else if(p->future_ball.y == 7 && board[p->future_ball.x][p->future_ball.y] == 4){
		hit_upper_wall(p);
		nextBallCords(p);
		p->ball.x = p->future_ball.x;
		p->ball.y = p->future_ball.y;
	}
}

void hit_player_one(pong_game* p) {
	switch(p->ball_heading) {
				case BALL_UPLEFT: p->ball_heading = BALL_UPRIGHT; break;
				case BALL_DOWNLEFT: p->ball_heading = BALL_DOWNRIGHT; break;
				default: break;
			}
}

void hit_player_two(pong_game* p) {
	switch(p->ball_heading) {
				case BALL_UPRIGHT: p->ball_heading = BALL_UPLEFT; break;
				case BALL_DOWNRIGHT: p->ball_heading = BALL_DOWNLEFT; break;
				default: break;
			}
}

void hit_upper_wall(pong_game* p) {
	switch(p->ball_heading) {
		case BALL_UPLEFT: p->ball_heading = BALL_DOWNLEFT; break;
		case BALL_UPRIGHT: p->ball_heading = BALL_DOWNRIGHT; break;
		default: break;
	}
}

void hit_lower_wall(pong_game* p) {
	switch(p->ball_heading) {
		case BALL_DOWNLEFT: p->ball_heading = BALL_UPLEFT; break;
		case BALL_DOWNRIGHT: p->ball_heading = BALL_UPRIGHT; break;
		default: break;
	}
}

