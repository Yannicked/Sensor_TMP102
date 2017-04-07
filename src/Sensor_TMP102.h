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

#ifndef TMP102_Sensor_H
#define TMP102_Sensor_H

#include <Arduino.h>
#include <Wire.h>

enum
{
	TEMP_REG_ADDR = 0b00,
	CONF_REG_ADDR = 0b01,
	TLOW_REG_ADDR = 0b10,
	THIG_REG_ADDR = 0b11
}

class Sensor_TMP102
{
	public:
		enum conversion_rate
		{
			RATE_025HZ = 0,
			RATE_1HZ = 1,
			RATE_4HZ = 4,
			RATE_8HZ = 8
		}

		Sensor_TMP102(void);

		bool begin(uint8_t addr);

		void setConversionRate(uint8_t rate = RATE_4HZ);

		void setExtended(bool extended);

		float readTemperature(void);

	private:
		uint8_t _i2caddr;
		bool _ext_mode;
		uint16_t getConfig(void);
		void setConfig(uint16_t config);
};

#endif