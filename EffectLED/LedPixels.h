// LedPixels.h

#ifndef _LEDPIXELS_h
#define _LEDPIXELS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>
//#include <map>
#include "TimerQueue.h"

class Color {
private:
	uint32_t rawValue;
	uint8_t red, green, blue, white;
public:
	static const uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
		return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
	}
	static const uint32_t rgbw(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
		return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
	}
	Color() {
		red = 0; green = 0; blue = 0; white = 0;
		rawValue = 0;
	}
	Color(int r, int g, int b) {
		red = r; green = g; blue = b; white = 0;
		rawValue = rgb(r, g, b);
	}
	Color(int r, int g, int b, int w) {
		red = r; green = g; blue = b; white = w;
		rawValue = rgbw(r, g, b, w);
	}
	Color(uint32_t raw) {
		rawValue = raw;
		//TODO
	}
	static Color RED;
	static Color GREEN;
	static Color BLUE;
	static Color YELLOW;
	static Color WHITE;
	static Color BLACK;
};

struct TaskParmeter {
	int intCount, floatCount;
	int *integers;
	float *floats;	
	TaskParmeter() {
		intCount = 0;
		floatCount = 0;
		integers = NULL;
		floats = NULL;
	}
	void setFloats(int count, ...) {
		floatCount = count;
		floats = new float[count];
		va_list argList;
		va_start(argList, count);
		for (int i = 0; i < count; i++) {
			floats[i] = va_arg(argList, float);
		}
	}
	void setIntegers(int count, ...) {
		intCount = count;
		integers = new int[count];
		va_list argList;
		va_start(argList, count);
		for (int i = 0; i < count; i++) {
			integers[i] = va_arg(argList, int);
		}
	}
	bool changeFloat(int index, float newValue) {
		if (index < floatCount) {
			floats[index] = newValue;
		}
	}
	bool changeInt(int index, int newValue) {
		if (index < intCount) {
			integers[index] = newValue;
		}
	}
};

struct LedAnimationParam {
public:
	int pixelIndex;
	void *startState;
	void *endState;
	Adafruit_NeoPixel *LedSegment;
	LedAnimationParam() {
		pixelIndex = 0;
		startState = NULL;
		endState = NULL;
		LedSegment = NULL;
	}
	LedAnimationParam(int index, void *start, void *end) {
		pixelIndex = index;
		startState = start;
		endState = end;
		LedSegment = NULL;
	}
	LedAnimationParam(int index, void *start, void *end, Adafruit_NeoPixel *segment) {
		pixelIndex = index;
		startState = start;
		endState = end;
		LedSegment = segment;
	}
};

class LedPixelsClass
{
 protected:

	 int PIN;
	 int NUMPIXELS = 108;

	 Adafruit_NeoPixel pixels;

	 TimerQueueClass<TaskParmeter, LedAnimationParam> TimerQueue;
	  
 public:
	LedPixelsClass();
	LedPixelsClass(int pin, int numPixels);
	void init(int pin, int numPixels);

	void circleAround(int delayval, int length, int count, int colorCount, ...);

	void LedPixelsClass::InitAnimatedcircleAround(float speed, int length, int start, Color color);

	void LedPixelsClass::AnimatedcircleAround(TaskParmeter param);

	//void Fade
};

extern LedPixelsClass LedPixels;

#endif

