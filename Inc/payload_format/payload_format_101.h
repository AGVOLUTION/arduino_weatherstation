/*
MIT License

Copyright (c) 2020 Agvolution
Head Development: Lukas Kamm / l.kamm@agvolution.com

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
*/

/* This payload format is tailored for Sigfox communications.
   Hence, we limit ourselves to a message size of 12 bytes. */

#ifndef _PAYLOAD_FORMAT_101_H
#define _PAYLOAD_FORMAT_101_H

/**************************************************************
 * CONVERSION INFORMATION
 **************************************************************
 *
 **************************************************************/

/**************************************************************
 * Payload specification
 **************************************************************
 * Byte		Parameter
 * 0		VERSION								# number (0..255)
 * 1		GPS Latitude WGS-84		MSB
 * 2		GPS Latitude WGS-84		2
 * 3		GPS Latitude WGS-84		1
 * 4		GPS Latitude WGS-84		LSB
 * 5		GPS Longitude WGS-84	MSB
 * 6		GPS Longitude WGS-84	2
 * 7		GPS Longitude WGS-84	1
 * 8		GPS Longitude WGS-84	LSB
 * 9		DS18B20 TEMP MSB		# Temperature (external) in 0.01 °C, signed, 2-complement
 * 10		DS18B20 TEMP LSB
 */

// Payload frame format: Must not exceed 51 for LoRa (!)
#define PAYLOAD_LENGTH_101 11
#define PAYLOAD_VERSION_101 101

enum PLD_FORMAT_101 {
	PLD_VERSION_101	= 0,
	PLD_LAT_101    = 1,
	PLD_LONG_101   = 5,
	PLD_DS18B20_101 = 9
};

// Error flags bit positions
enum ERR_FLAGS_101 {
};

// Structure typedef
/* Payload */
typedef struct __attribute__ ((packed)) __payload_101_t {
  uint8_t version = 101;
  float latitude;
  float longitude;
  int16_t temperature;
} payload_101_t;

#endif // _payload_format_h
