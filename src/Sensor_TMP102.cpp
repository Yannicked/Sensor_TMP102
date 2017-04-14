/*
	MIT License

	Copyright (c) 2017 Yannick de Jong

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	Datasheet: http://www.ti.com/lit/ds/symlink/tmp102.pdf
*/

#include <Arduino.h>
#include <Wire.h>

#include "Sensor_TMP102.h"

Sensor_TMP102::Sensor_TMP102() : _single_shot(false)
{

}

Sensor_TMP102::Sensor_TMP102(bool single_shot) : _single_shot(single_shot)
{

}

bool Sensor_TMP102::begin(uint8_t addr)
{
	_i2caddr = addr;

	Wire.begin();
	
	if (_single_shot) {
		uint16_t config = getConfig();
		config |= (1 << 8);
		setConfig(config);
	} 
	return true;
}

uint16_t Sensor_TMP102::getConfig() {
	Wire.beginTransmission((uint8_t) _i2caddr);
	Wire.write((uint8_t) CONF_REG_ADDR);
	Wire.endTransmission();

	Wire.requestFrom((uint8_t) _i2caddr, (uint8_t) 2);

	return (Wire.read() << 8) | Wire.read();
}

void Sensor_TMP102::setConfig(uint16_t config) {
	Wire.beginTransmission((uint8_t) _i2caddr);
	Wire.write((uint8_t) CONF_REG_ADDR);
	Wire.write((uint8_t) config >> 8);
	Wire.write((uint8_t) config & 255);
	Wire.endTransmission();
}

void Sensor_TMP102::setConversionRate(conversion_rate rate)
{
	uint16_t config = getConfig();

	switch (rate) {
		case 0:
			config &= ~CONFIG_CR0;
			config &= ~CONFIG_CR1;
			config &= ~CONFIG_SD;
			_single_shot = false;
			break;
		case 1:
			config |=  CONFIG_CR0;
			config &= ~CONFIG_CR1;
			config &= ~CONFIG_SD;
			_single_shot = false;
			break;
		case 4:
			config &= ~CONFIG_CR0;
			config |=  CONFIG_CR1;
			config &= ~CONFIG_SD;
			_single_shot = false;
			break;
		case 8:
			config |=  CONFIG_CR0;
			config |=  CONFIG_CR1;
			config &= ~CONFIG_SD;
			_single_shot = false;
			break;
		case 99:
			config |= CONFIG_SD;
			_single_shot = true;
			break;
	}

	setConfig(config);
}

void Sensor_TMP102::setExtended(bool extended)
{
	uint16_t config = getConfig();

	if (extended) {
		config |= (1 << 4);
	} else {
		config &= ~(1 << 4);
	}

	setConfig(config);
}

float Sensor_TMP102::readTemperature()
{
	if (_single_shot) {
		uint16_t config = getConfig();
		config |= CONFIG_OS;
		setConfig(config);
		delay(35);
	}
	Wire.beginTransmission((uint8_t) _i2caddr);
	Wire.write((uint8_t) TEMP_REG_ADDR);
	Wire.endTransmission();

	Wire.requestFrom((uint8_t) _i2caddr, (uint8_t) 2);

	int16_t temp_raw;

	byte registerByte[2];
	registerByte[0] = Wire.read();
	registerByte[1] = Wire.read();

	if (registerByte[1]&0x01) {
		temp_raw = (registerByte[0] << 5) | (registerByte[1] >> 3);
		if (temp_raw > 0xFFF) {
			temp_raw |= 0xE000;
		}
	} else {
		temp_raw = (registerByte[0] << 4) | (registerByte[1] >> 4);
		if (temp_raw > 0x7FF) {
			temp_raw |= 0xF000;
		}
	}

	float temp = (float) temp_raw * 0.0625;

	return temp;
}
