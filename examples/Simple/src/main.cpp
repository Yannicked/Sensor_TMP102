#include <Arduino.h>
#include <Wire.h>
#include <Sensor_TMP102.h>

Sensor_TMP102 tmp;

void setup() {
    Serial.begin(9600);
    Serial.println("TMP102 test");

    // default settings
    tmp.begin(0x48);

    Serial.println();
}


void loop() {
    Serial.print(tmp.readTemperature());
	Serial.println("*C");
	delay(1000);
}
