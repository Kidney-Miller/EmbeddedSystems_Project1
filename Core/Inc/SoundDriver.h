/*
 * SoundDriver.h
 *
 *  Created on: Oct 13, 2022
 *      Author: gleasonn
 *
 *   Contains SoundDriver
 */

#ifndef INC_SOUNDDRIVER_H_
#define INC_SOUNDDRIVER_H_

#include "main.h"
#include "stdbool.h"


// "Class" SoundDriver///////////////////////////////////////////////
// Attributes & method pointers for DebouncedEdgeDetector
typedef
		struct SoundDriver_struct {
		// Attribute
			volatile uint32_t beepTime;
			// Method pointer
			void (*update) (struct SoundDriver_struct* self, bool *beepFlag);
} SoundDriver;

// Methods for SoundDriver
void SoundDriver_init(SoundDriver *self);
void SoundDriver_update(SoundDriver *self, bool *beepFlag);
///////////////////////////////////////////////////////////////////////////////


#endif /* INC_SOUNDDRIVER_H_ */
