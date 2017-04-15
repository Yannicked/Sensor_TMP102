#include <Arduino.h>
#include <Wire.h>
#include <Sensor_TMP102.h>

Sensor_TMP102 tmp;

void setup() {
    Serial.begin(9600);
    Serial.println("TMP102 Advanced Test");

    tmp.begin(0x48);
	
	/* Can be: 
	RATE_SINGLE_SHOT (reads temperature only when requested) takes about 50 ms
	RATE_025HZ (reads temperature at 0.25Hz intervals) faster than single shot
	RATE_1HZ (reads temperature at 1Hz intervals)
	etc.
	*/
	tmp.setConversionRate(Sensor_TMP102::RATE_SINGLE_SHOT);
	
	// Set the TMP102 to extended mode (temperatures above 128*C)
	tmp.setExtended(true);
	
    Serial.println();
}


void loop() {
	float temperature = tmp.readTemperature();
	
    Serial.print(temperature);
	Serial.println("*C");
	delay(1250);
}
