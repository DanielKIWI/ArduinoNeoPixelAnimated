// 
// 
// 
/*
#include "LedPixels.h"



LedPixelsClass::LedPixelsClass()
{
	PIN = 0;
	NUMPIXELS = 0;
	pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);
	//DispatchQueue = DispatchQueueClass<TaskParameter, TaskParameter>::DispatchQueueClass(FPS);
}
LedPixelsClass::LedPixelsClass(int pin, int numPixels, int fps = 60)
{
	DispatchQueue = new DispatchQueueClass(fps);
	//FPS = fps;
	PIN = pin;
	NUMPIXELS = numPixels;
	pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

	pixels.begin(); // This initializes the NeoPixel library.
	pixels.show();
	//DispatchQueue = DispatchQueueClass<TaskParameter, TaskParameter>(FPS);
}

LedPixelsClass::LedPixelsClass(int pin, int numPixels, DispatchQueueClass *dq)
{
	DispatchQueue = dq;
	//FPS = fps;
	PIN = pin;
	NUMPIXELS = numPixels;
	pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

	pixels.begin(); // This initializes the NeoPixel library.
	pixels.setPixelColor(4, 100, 0, 0, 0);
	pixels.show();
	//DispatchQueue = DispatchQueueClass<TaskParameter, TaskParameter>(FPS);
}
/*
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
*
void LedPixelsClass::InitAnimatedcircleAround(float speed, int length, int startIndex, Color color)
{
	unsigned long deltaMicros = (unsigned long)((1 / speed) * sTOmics);
	TaskParameter *param = new TaskParameter();
	param->setIntegers(3, startIndex, length, NUMPIXELS, (int)Color::RED.rawValue());
	param->setPointers(2, DispatchQueue, &pixels);
	DispatchQueue->addRepeatingTask(&AnimatedcircleAround, param, deltaMicros);
	//TimerQueue.doTaskInSec
}

void LedPixelsClass::setPixelBrightnessTo(float value, TaskParameter *param)
{
	float factor = (value > 1) ? 1 : ((value < 0) ? 0 : value);
	int index = param->integers[0];
	Color color = Color((uint32_t)param->integers[1]);
	void *pixelsVoidP = param->pointers[0];
	Adafruit_NeoPixel *pixels = static_cast<Adafruit_NeoPixel*>(pixelsVoidP);
	pixels->setPixelColor(index, (color * factor).rawValue());
	pixels->show();
}


bool LedPixelsClass::AnimatedcircleAround(TaskParameter *param)
{
	int index = param->integers[0];
	int length = param->integers[1];
	param->changeInt(1, length + 1);
	int NumPixels = param->integers[2];
	DispatchQueueClass<TaskParameter, TaskParameter> *dispatchQueue = static_cast<DispatchQueueClass<TaskParameter, TaskParameter>*>(param->pointers[0]);
	void *pixelsVoidP = param->pointers[1];
	Adafruit_NeoPixel *pixels = static_cast<Adafruit_NeoPixel*>(pixelsVoidP);

	int color = param->integers[3];
	int nextIndexOn = (index + length - 1) % NumPixels;
	int nextIndexOff = (index - 1) % NumPixels;
	//LedAnimationParam *AnimParam = new LedAnimationParam(nextIndexOn, &Color::BLACK, new Color(color), pixels);
	TaskParameter *AnimParamOn = new TaskParameter();
	AnimParamOn->setIntegers(2, nextIndexOn, color);
	AnimParamOn->setPointers(1, pixels);
	dispatchQueue->addAnimationTask(&setPixelBrightnessTo, AnimParamOn, 0, 1, 1.0);

	TaskParameter *AnimParamOff = new TaskParameter();
	AnimParamOff->setIntegers(2, nextIndexOff, color);
	AnimParamOff->setPointers(1, pixels);
	dispatchQueue->addAnimationTask(&setPixelBrightnessTo, AnimParamOff, 1, 0, 1.0);
	//pixels.setPixelColor((i + length - 1) % NumPixels, color);
	//pixels.setPixelColor((i - 1) % NumPixels, pixels.Color(0, 0, 0));

return true;
}

/*void LedPixelsClass::showPixels(int ) {
	pixels.show();
}*/
