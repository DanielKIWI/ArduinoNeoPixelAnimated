#ifndef _SerialListener_h

class SerialListener : public Task {
	unsigned long dt_read;
	float *volume;
	bool simulateUp = true;
public:
	SerialListener(float frequence, float *volumePtr) {
		volume = volumePtr;
		dt_read = (unsigned long)((1 / frequence) * sTOmics);
		dueTime = 0; 
	}
	bool execute() {
		if (Serial.available())
		{
			Serial.print("ListenOnSerial: ");
			String stringData = Serial.readString(); Serial.println(stringData);
			String methodName = Serial.readStringUntil('(');
			String value = Serial.readStringUntil(')');
			int intValue = value.toInt();
				Serial.println(intValue);
			//if (intValue > 255) intValue = 255;
			//else if (intValue < 0) intData = 0;
			if (intValue > 128)
				digitalWrite(LED_BUILTIN, HIGH);
			else
				digitalWrite(LED_BUILTIN, LOW);
			*volume = (float)intValue / 255;
		}
		else {
			if (*volume > 1.0f) {
				*volume = 1.0f;
				simulateUp = false;
			}
			else if (*volume < 0) { 
				*volume = 0.0f; 
				simulateUp = true;
			}
			if (simulateUp) {
				*volume += 0.1f;
			}
			else {
				*volume -= 0.1f;
			}

			Serial.print("simulated volume: ");	Serial.println(*volume);
		}
		dueTime = micros() + dt_read;
		return true;
	}
};
#endif // !_SerialListener_h

