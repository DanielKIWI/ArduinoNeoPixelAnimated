#ifndef _SerialListener_h

class SerialListener : public Task {
	unsigned long dt_read;
	float *volume;
public:
	SerialListener(float frequence, float *volumePtr) {
		dt_read = (unsigned long)(1 / frequence);
	}
	bool execute() {
		Serial.println("ListenOnSerial");
		if (Serial.available())
		{
			int intData = Serial.read();
			Serial.println(intData);
			if (intData > 255) intData = 255;
			else if (intData < 0) intData = 0;

			*volume = (float)intData / 255;
		}
		dueTime += dt_read;
		return true;
	}
};
#endif // !_SerialListener_h

