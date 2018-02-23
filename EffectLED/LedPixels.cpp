// 
// 
// 

#include "LedPixels.h"

uint32_t Color::RED = rgb(255, 0, 0);
uint32_t Color::GREEN = rgb(0, 255, 0);
uint32_t Color::BLUE = rgb(0, 0, 255);
uint32_t Color::YELLOW = rgb(150, 150, 0);
uint32_t Color::WHITE = rgbw(0, 0, 0, 255);


void LedPixelsClass::init(int pin, int numPixels)
{
	PIN = pin;
	NUMPIXELS = numPixels;
	pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

	pixels.begin(); // This initializes the NeoPixel library.
	pixels.show();

}

//LedSrip(NUMPIXELS, PIN) {
//	NEO_GRBW + NEO_KHZ800
//}

void LedPixelsClass::circleAround(int delayval, int length, int count, int colorCount, ...)
{
	int dist = (NUMPIXELS - (count * length)) / count;
	va_list argList;
	LinkedList<uint32_t> color;
	va_start(argList, colorCount);
	for (int i = 0; i < colorCount; i++) {
		color.add(va_arg(argList, uint32_t));
	}
	for (int i = 0; i < NUMPIXELS; i++) {
		for (int num = 0; num < count; num++) {
			for (int j = 0; j < length; j++) {

				pixels.setPixelColor((i + j + num*(dist + length)) % NUMPIXELS, color.get(num%colorCount));
			}
			pixels.setPixelColor((i + num*(dist + length) - 1) % NUMPIXELS, pixels.Color(0, 0, 0));
		}

		pixels.show(); // This sends the updated pixel color to the hardware.

		delay(delayval); // Delay for a period of time (in milliseconds).
	}
}

//LedPixelsClass LedPixels;

