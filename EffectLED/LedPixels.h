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

