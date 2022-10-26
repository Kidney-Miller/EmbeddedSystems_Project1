/*
 * Pong_Gameplay.h
 *
 *  Created on: Sep 23, 2022
 *      Author: krmiller21
 */

#ifndef SRC_PONG_GAMEPLAY_H_
#define SRC_PONG_GAMEPLAY_H_
#define BALL_BOUNDS (((uint16_t)6))
//Code References Go Here:
#include <stdint.h>
#include <stdbool.h>
//#include "keypad.h"
#include "smc_queue.h"
#include "pong_main.h"

#include "pong_enums.h"


typedef struct {int16_t x; int16_t y;} XY_PT;

typedef struct {
	XY_PT player1;
	XY_PT player2;
	uint8_t player_length;
	XY_PT ball;
	XY_PT future_ball;
	bool ball_hit;
	enum player_directions p1_heading;
	enum player_directions p2_heading;
	enum ball_directions ball_heading;
} pong_game;

//UPDATED
void ball_opposite_direction(pong_game* p);
bool player1_plot(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
bool player2_plot(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
bool ball_plot(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
void detect_collision(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
void update_ball_XY(pong_game* p);
void update_P1_XY(pong_game* p);
void update_P2_XY(pong_game* p);
void pong_game_init(pong_game* p);
void pacify_compiler();
void player1_heading_update(pong_game* p, Smc_queue* q);
void player2_heading_update(pong_game* p, Smc_queue* q);
void ball_heading_update(pong_game* p);
bool isBallDead(pong_game* p);
void nextBallCords(pong_game* p);
void pong_periodic_play(pong_game* p);
void hit_player_one(pong_game* p);
void hit_player_two(pong_game* p);
void hit_upper_wall(pong_game* p);
void hit_lower_wall(pong_game* p);

#endif /* SRC_PONG_GAMEPLAY_H_ */
