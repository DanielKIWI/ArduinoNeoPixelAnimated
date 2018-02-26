/*
 Name:		EffectLED.ino
 Created:	21.02.2018 23:51:05
 Author:	Daniel
*/


//#include "Math.h"
#include "LedPixels.h"
#include <pRNG.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


LedPixelsClass LedPixels = LedPixelsClass(3, 108);

void setup() {
	//LedPixels.init(3, 108);
	LedPixels.pixels.setPixelColor(0, Color::BLUE.rawValue());
	LedPixels.InitAnimatedcircleAround(2, 10, 0, Color::RED);
	LedPixels.Loop();
}

void loop() {
	//LedPixels.circleAround(100, 10, 4, 1, Color::RED, Color::GREEN, Color::BLUE, Color::WHITE);
	
}
