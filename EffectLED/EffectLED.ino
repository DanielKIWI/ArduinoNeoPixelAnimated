/*
 Name:		EffectLED.ino
 Created:	21.02.2018 23:51:05
 Author:	Daniel
*/


#include "LedPixels.h"
#include <pRNG.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


LedPixelsClass LedPixels = LedPixelsClass();

void setup() {
	LedPixels.init(3, 108);

}



void loop() {
	LedPixels.circleAround(100, 10, 4, 1, Color::RED, Color::GREEN, Color::BLUE, Color::WHITE);

}
