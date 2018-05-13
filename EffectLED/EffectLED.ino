/*
 Name:		EffectLED.ino
 Created:	21.02.2018 23:51:05
 Author:	Daniel
*/


#include "Math.h"
#include "LedPixels.h"
#include <pRNG.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

DispatchQueueClass<TaskParameter, TaskParameter> DispatchQueue = DispatchQueueClass<TaskParameter, TaskParameter>(10);
//LedPixelsClass LedPixels = LedPixelsClass(3, 108, &DispatchQueue);

void setup() {
	Serial.begin(9600);
	Serial.println("DipatchQueue Initialized");
	//LedPixels.init(3, 108);
	
	TaskParameter *serialParam = new TaskParameter();
	
	DispatchQueue.addRepeatingTask(&ListenOnSerial, serialParam, 0.5f);
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

bool ListenOnSerial(TaskParameter* param) {
	Serial.println("ListenOnSerial");
	if (Serial.available())
	{
		String stringData = Serial.readString();
		int intData = Serial.read();
		Serial.println(stringData);
		Serial.println(intData);
	}
	return true;
}
