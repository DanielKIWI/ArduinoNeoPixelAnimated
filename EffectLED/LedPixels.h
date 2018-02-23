// LedPixels.h

#ifndef _LEDPIXELS_h
#define _LEDPIXELS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>
#include "TimerQueue.h"

class Color {
public:

	static const uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
		return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
	}
	static const uint32_t rgbw(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
		return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
	}
public:
	static uint32_t RED;
	static uint32_t GREEN;
	static uint32_t BLUE;
	static uint32_t YELLOW;
	static uint32_t WHITE;
};

class LedPixelsClass
{
 protected:

	 int PIN;
	 int NUMPIXELS = 108;

	 Adafruit_NeoPixel pixels;
 public:
	void init(int pin, int numPixels);

	void circleAround(int delayval, int length, int count, int colorCount, ...);
};

extern LedPixelsClass LedPixels;

#endif

