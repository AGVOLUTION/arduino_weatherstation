/*
MIT License

Copyright (c) 2021 Agvolution
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

#ifndef _PAYLOAD_FORMAT_104_H
#define _PAYLOAD_FORMAT_104_H

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
 * 1		Weight (float, IEE754, 4 byte, byte order LSB first) in grams
 * 2		"
 * 3		"
 * 4		"
 * 5		DS18B20 TEMP LSB		# Temperature (external) in 0.01 °C, signed, 2-complement
 * 6		DS18B20 TEMP MSB
 * 7		
 * 8		
 * 9		
 * 10		
 * 11		
 *
 * ERROR FLAGS
 *	15		14		13		12		11		10		9		8		7		6		5		4		3		2		1		0
 *	
 */

// Payload frame format: Must not exceed 51 for LoRa (!)
#define PAYLOAD_LENGTH_104 7
#define PAYLOAD_VERSION_104 104

enum PLD_FORMAT_104 {
	PLD_VERSION_104	= 0,
	PLD_WEIGHT_104 = 1,
	PLD_DS18B20_104	= 5,
};

// Error flags bit positions
enum ERR_FLAGS_104 {
};

// Structure typedef
/* Payload */
typedef struct __attribute__ ((packed)) __payload_104_t {
  uint8_t version = 104;
  float weight;
  int16_t temperature;
} payload_104_t;

#endif // _payload_format_104_h
