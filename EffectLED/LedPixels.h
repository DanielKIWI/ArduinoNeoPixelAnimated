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
/*
class LedPixelsClass
{
 protected:
	 //int FPS = 60;
	 int PIN;
	 int NUMPIXELS = 108;

	 DispatchQueueClass *DispatchQueue;
	  
 public:
	Adafruit_NeoPixel pixels;
	LedPixelsClass(); 
	LedPixelsClass(int pin, int numPixels, int fps = 60);
	LedPixelsClass(int pin, int numPixels, DispatchQueueClass *dq = NULL);
	/*
	void circleAround(int delayval, int length, int count, int colorCount, ...);

	void InitAnimatedcircleAround(float speed, int length, int start, Color color);

	static void setPixelBrightnessTo(float value, TaskParameter *param);

	static bool AnimatedcircleAround(TaskParameter *param);
	*


	void showPixels();
};*/

//extern LedPixelsClass dLedPixels;
namespace NeoPixels {
	class FadeColor: AnimationTask {

		FadeColor(Adafruit_NeoPixel *pixels, int pixelIndex, Color targetColor, float duration) {

		}
	};

	class VisualizeFloat: public AnimationTask {
		float *targetValue;
		bool isInversed;
		int maxPixelIndex;
		int minPixelIndex;
		Adafruit_NeoPixel *pixels;
		Color *color;
	public:
		VisualizeFloat(Adafruit_NeoPixel *_pixels, float *value, int _minPixelIndex, int _maxPixelIndex, Color *_color, bool inversed = false) {
			pixels = _pixels;
			color = _color;
			targetValue = value;
			isInversed = inversed;
			minPixelIndex = _minPixelIndex;
			maxPixelIndex = _maxPixelIndex;
		}
		bool execute(float deltaTime) override {
			Serial.println("test");
			float actValue = *targetValue;
			float pixelsValue = actValue * (maxPixelIndex - minPixelIndex);
			if (isInversed) {

			}
			else {
				float x = 0;
				for (uint16_t i = minPixelIndex; i < maxPixelIndex; i++, x++) {
					if (pixelsValue > x) {
						if (pixelsValue > x + 1) {
							pixels->setPixelColor(i, color->rawValue());
						}
						else {
							Color c = *color * (pixelsValue - x);
							pixels->setPixelColor(i, c.rawValue());
						}
					}
				}
			}
		}
	};
}



#endif

