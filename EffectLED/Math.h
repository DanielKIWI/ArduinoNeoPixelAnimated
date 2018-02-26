// Math.h

#ifndef _MATH_h
#define _MATH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Math
{
public:
	static float modf(float f1, float f2) {
		if (f2 < 0) {
			return 0;
		}
		float result = f1;
		while (result > f2) {
			result -= f2;
		}
		while (result < 0) {
			result += f2;
		}
		return result;
	}
	static void interpolateLinear(float* start, float* end, float part, float* target) {
		//ToBeImplemented
	}
};

#endif

