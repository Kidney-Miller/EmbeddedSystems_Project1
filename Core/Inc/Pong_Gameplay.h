/*
 * Pong_Gameplay.h
 *
 *  Created on: Sep 23, 2022
 *      Author: krmiller21
 */

#ifndef SRC_PONG_GAMEPLAY_H_
#define SRC_PONG_GAMEPLAY_H_

//Code Refrences Go Here:
#include "pong_enums.h"

typedef struct {int16_t x; int16_t y;} XY_PT;

typedef struct {
	XY_PT player1;
	XY_PT player2;
	XY_PT ball;
	enum  player_directions p1_heading;
	enum  player_directions p2_heading;
	enum  ball_directions ball_heading;
} pong_game;

enum ball_directions ball_opposite_direction(enum ball_directions d);
void pong_game_init(pong_game* p);
void pong_game_cleanup(pong_game* p);
void pong_game_periodic_play(pong_game* p);
void pong_place_ball(pong_game * p, const int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
bool p1_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
bool p2_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);
bool ball_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);

#endif /* SRC_PONG_GAMEPLAY_H_ */
