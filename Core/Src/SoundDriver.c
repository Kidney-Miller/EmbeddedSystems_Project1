/*
 * SoundDriver.c
 *
 * C-language class-like definition

 */

#include <SoundDriver.h>
#include "main.h"


void
		SoundDriver_init(SoundDriver *self){
		self->beepTime = 10000;
	    self->update = &SoundDriver_update;
}

void SoundDriver_update (SoundDriver *self, bool *beepFlag){
	if (*beepFlag) {
	      for (volatile uint32_t n = 0; n < self->beepTime; n++);
	      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	      for (volatile uint32_t n = 0; n < self->beepTime; n++);

	      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	      *beepFlag = false;
	}
	else {
		return;
	}
}

/*
 * To get this to work, several things must be added to snake_main.c,
 * in several places
 *
 * This in #includes V
 * #include "SoundDriver.h"
 *
 * This in Initialization V
 *Initialize pin for sound
 *	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
 *
 * This in Construct IPC section V
 * IPC flag for the sound driver
 * bool beepFlag;
	beepFlag = true;
	bool *beepPtr;
	beepPtr = &beepFlag;
 *
 * When the pong game logic detects that the ball has made contact with a paddle
 * it must somehow send a message to main, where beepFlag must be set to true
 *
 * Finally, this as the last update in the main loop
 * beeper.update(&beeper, beepPtr);
 *
 *
 */
