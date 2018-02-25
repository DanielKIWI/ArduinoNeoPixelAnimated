// 
// 
// 

#include "LedPixels.h"

Color Color::RED = Color(255, 0, 0);
Color Color::GREEN = Color(0, 255, 0);
Color Color::BLUE = Color(0, 0, 255);
Color Color::YELLOW = Color(150, 150, 0);
Color Color::WHITE = Color(0, 0, 0, 255);
Color Color::BLACK = Color();


LedPixelsClass::LedPixelsClass()
{
	PIN = 0;
	NUMPIXELS = 0;
	pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);
	TimerQueue = TimerQueueClass<TaskParmeter, LedAnimationParam>::TimerQueueClass();
}
LedPixelsClass::LedPixelsClass(int pin, int numPixels)
{
	PIN = pin;
	NUMPIXELS = numPixels;
	pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

	pixels.begin(); // This initializes the NeoPixel library.
	pixels.show();
	TimerQueue = TimerQueueClass<TaskParmeter, LedAnimationParam>();
	//TimerQueue.init();
}
void LedPixelsClass::init(int pin, int numPixels)
{
	PIN = pin;
	NUMPIXELS = numPixels;
	pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

	pixels.begin(); // This initializes the NeoPixel library.
	pixels.show();
	TimerQueue = TimerQueueClass<TaskParmeter, LedAnimationParam>();
	//TimerQueue.init();
}


void LedPixelsClass::circleAround(int delayval, int length, int count, int colorCount, ...)
{
	int dist = (NUMPIXELS - (count * length)) / count;
	va_list argList;
	//const int constColCount = colorCount;
	uint32_t colors[colorCount];
	va_start(argList, colorCount);
	for (int i = 0; i < colorCount; i++) {
		colors[i] = (va_arg(argList, uint32_t));
	}
	for (int i = 0; i < NUMPIXELS; i++) {
		for (int num = 0; num < count; num++) {
			for (int j = 0; j < length; j++) {
				pixels.setPixelColor((i + j + num*(dist + length)) % NUMPIXELS, colors[num%colorCount]);
			}
			pixels.setPixelColor((i + num*(dist + length) - 1) % NUMPIXELS, pixels.Color(0, 0, 0));
		}

		pixels.show(); // This sends the updated pixel color to the hardware.

		delay(delayval); // Delay for a period of time (in milliseconds).
	}
}


/*
	speed: Pixel per second
*/
void LedPixelsClass::InitAnimatedcircleAround(float speed, int length, int start, Color color)
{
	int deltaMicros = (int)((1 / speed) * sTOmics);
	TaskParmeter param = TaskParmeter();
	param.setIntegers(3, start, length, deltaMicros);

	//TimerQueue.doTaskInSec
}

void LedPixelsClass::AnimatedcircleAround(TaskParmeter param)
{
	/*
	int dist = (NUMPIXELS - (count * length)) / count;
	va_list argList;
	const int constColCount = colorCount;
	uint32_t colors[constColCount];
	va_start(argList, colorCount);
	for (int i = 0; i < colorCount; i++) {
		colors[i] = (va_arg(argList, uint32_t));
	}
	for (int i = 0; i < NUMPIXELS; i++) {
		for (int num = 0; num < count; num++) {
			for (int j = 0; j < length; j++) {
				pixels.setPixelColor((i + j + num*(dist + length)) % NUMPIXELS, colors[num%colorCount]);
			}
			pixels.setPixelColor((i + num*(dist + length) - 1) % NUMPIXELS, pixels.Color(0, 0, 0));
		}

		pixels.show(); // This sends the updated pixel color to the hardware.

		delay(delayval); // Delay for a period of time (in milliseconds).
	}*/
}

//LedPixelsClass LedPixels;

