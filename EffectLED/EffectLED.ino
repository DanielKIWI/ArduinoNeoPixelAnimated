/*
 Name:		EffectLED.ino
 Created:	21.02.2018 23:51:05
 Author:	Daniel
*/


#include "Math.h"
#include "LedPixels.h"
#include "SerialListener.h"
#include <pRNG.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

DispatchQueueClass DispatchQueue = DispatchQueueClass(10);
//LedPixelsClass LedPixels = LedPixelsClass(3, 108, &DispatchQueue);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(108, 3, NEO_GRBW + NEO_KHZ800);

float volume = 0.0f;
Color ledColor = Color(100, 0, 0);
void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	Serial.println("DipatchQueue Initialized");
	SerialListener *serialListener = new SerialListener(5.0f, &volume);
	DispatchQueue.addTask(serialListener);

	NeoPixels::VisualizeFloat *VolumeVisualizer1 = new NeoPixels::VisualizeFloat(&pixels, &volume, 0, 100, &ledColor, true);
	DispatchQueue.addAnimationTask(VolumeVisualizer1);


	//DispatchQueue.addRepeatingTask(&ListenOnSerial, serialParam, 0.5f);
	/*
	LedPixels.pixels.setPixelColor(0, Color::BLUE.rawValue());
	LedPixels.InitAnimatedcircleAround(2, 10, 0, Color::RED);
	//LedPixels.Loop();
	Serial.write("Test2");*/
	DispatchQueue.Loop(); 
}

void loop() {
	//LedPixels.circleAround(100, 10, 4, 1, Color::RED, Color::GREEN, Color::BLUE, Color::WHITE);
	
}
