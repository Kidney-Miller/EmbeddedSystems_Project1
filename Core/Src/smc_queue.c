/*
 * smc_queue_struct.c
 *
 *  Created on: Jul 15, 2022
 *      Author: carrolls
 *
 *  Edited by: gleasonn
 *  Edited on: Oct 13, 2022
 */

// This is the edited version of Dr. Carrol's smc_queue.c to replace it with a circular queue


#include "smc_queue.h"
#include "snake_enums.h"

Smc_queue* smc_queue_init(Smc_queue* q){
	q->head = 0;
	q->tail= 0;
	q->cap = SMC_Q_BUFSIZE;
	q->items = 0;
	q->put = &(smc_queue_put);
	q->get = &(smc_queue_get);
	q->peek = &(smc_queue_peek);
	return q;
}


bool smc_queue_put(Smc_queue *q, const Q_data *msg){
	bool success = false;
	// FIRST check if there is room in the queue
	if (q->items >= q->cap) success = false;
	else {
		 q->buffer[q->tail] = *msg;

		 // Move tail
		 if ((q->tail + 1) >= q->cap){
			 q->tail = 0;
		 }
		 else {
			 q->tail += 1;
		 }

		 // Bookkeeping
		 q-> items += 1;
		 success = true;
	}
	return success;
}

bool smc_queue_get(Smc_queue *q, Q_data  *msg){
	bool success = false;
	// FIRST check if there is data in the queue
	if (q->items == 0) success= false;

	else {
	    // Get message from front
		*msg = q->buffer[q->head];

		// Move head
		if ((q->head + 1) >= q->cap){
			q->head = 0;
		}
		else {
			q->head += 1;
		}

		// Bookkeeping
		q->items -= 1;
		success = true;
	}
	return success;
}


bool smc_queue_peek(const Smc_queue *q, Q_data  *msg){
	bool success = false;
	// FIRST check if there is data in the queue
	if (q->items == 0) success = false;
	// If YES - copy data but do not modify anything.
	else {
		*msg = q->buffer[0];
		success = true;
	}
	return success;
}
