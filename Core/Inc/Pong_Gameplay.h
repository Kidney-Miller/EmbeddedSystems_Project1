/*
 * Pong_Gameplay.h
 *
 *  Created on: Sep 23, 2022
 *      Author: krmiller21
 */

#ifndef SRC_PONG_GAMEPLAY_H_
#define SRC_PONG_GAMEPLAY_H_
#define BALL_BOUNDS (((uint16_t)6))
//Code Refrences Go Here:
#include "pong_enums.h"

typedef struct {int16_t x; int16_t y;} XY_PT;

typedef struct {
	XY_PT player1;
	XY_PT player2;
	uint8_t player_length;
	XY_PT ball;
	bool ball_hit;
	enum player_directions p1_heading;
	enum player_directions p2_heading;
	enum ball_directions ball_heading;
} pong_game;

//UPDATED
enum ball_directions ball_opposite_direction(enum ball_directions d);
bool player1_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
bool player2_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
bool ball_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
bool detect_collision(pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
void get_collision_cords(pong_game* p);
void pong_game_init(pong_game* p);
void pacify_compiler();
void player1_heading_update(pong_game* p, Smc_queue* q);
void player2_heading_update(pong_game* p, Smc_queue* q);
void ball_heading_update(pong_game* p);
bool isBallDead(pong_game* p);
XY_PT nextBallCords(const pong_game* p);
void pong_periodic_play(pong_game* p);

#endif /* SRC_PONG_GAMEPLAY_H_ */
