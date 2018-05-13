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
#include "DispatchQueue.h"

#define FPS  60

struct TaskParameter {
	int intCount, floatCount;
	int *integers;
	float *floats;	
	Color *colors;
	void **pointers;
	TaskParameter() {
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
	void setPointers(int count, ...) {
		intCount = count;
		*pointers = malloc(sizeof(void *) * count);;
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
	bool changePointer(int index, void *newValue) {
		if (index < intCount) {
			pointers[index] = newValue;
		}
	}
};
/*
struct LedAnimationParam {
public:
	int pixelIndex;
	void *Info
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
};*/

class LedPixelsClass
{
 protected:
	 //int FPS = 60;
	 int PIN;
	 int NUMPIXELS = 108;

	 DispatchQueueClass<TaskParameter, TaskParameter> *DispatchQueue;
	  
 public:
	 Adafruit_NeoPixel pixels;
	LedPixelsClass();
	LedPixelsClass(int pin, int numPixels, int fps = 60);
	LedPixelsClass(int pin, int numPixels, DispatchQueueClass<TaskParameter, TaskParameter> *dq = NULL);

	void circleAround(int delayval, int length, int count, int colorCount, ...);

	void InitAnimatedcircleAround(float speed, int length, int start, Color color);

	static void setPixelBrightnessTo(float value, TaskParameter *param);

	static bool AnimatedcircleAround(TaskParameter *param);

	void Loop() {
		DispatchQueue->Loop();
	}

	void showPixels();
};

//extern LedPixelsClass LedPixels;





#endif

