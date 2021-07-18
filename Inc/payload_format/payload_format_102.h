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

#ifndef _PAYLOAD_FORMAT_102_H
#define _PAYLOAD_FORMAT_102_H

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
 * 1		Watermark 0 Matrix potential in centibars
 * 2		Watermark 1 Matrix potential in centibars
 * 3		Watermark 2 Matrix potential in centibars
 * 4		Watermark 3 Matrix potential in centibars
 * 5		Leaf wetness
 * 6		BME280 I2C ADR 0x77 TEMP MSB		# Temperature (+15cm) in 0.01 °C, signed, 2-complement
 * 7		BME280 I2C ADR 0x77 TEMP LSB
 * 8		BME280 I2C ADR 0x77 HUM				# Relative Humidity (+15cm) in 1%
 * 9		BME280 I2C ADR 0x77 PRES MSB		# Pressure (+15cm) in 0.1 hPa
 * 10		BME280 I2C ADR 0x77 PRES LSB

 *
 * ERROR FLAGS
 */

// Payload frame format: Must not exceed 51 for LoRa (!)
#define PAYLOAD_LENGTH_102 11
#define PAYLOAD_VERSION_102 102

enum PLD_FORMAT_102 {
	PLD_VERSION_102	= 0,
	PLD_WM_0_102 	= 1,
	PLD_WM_1_102 	= 2,
	PLD_WM_2_102 	= 3,
	PLD_WM_3_102 	= 4,
	PLD_LEAF_WET_102 = 5,
	PLD_BME77_102 	= 6
};

// Error flags bit positions
enum ERR_FLAGS_102 {
};

// Structure typedef
/* Payload */
typedef struct __attribute__ ((packed)) __payload_102_t {
  uint8_t version = 102;
  uint8_t matrixPotential[4];
  uint8_t leafWetness;
  int16_t temperature;
  uint8_t humidity;
  uint16_t pressure;
} payload_102_t;

#endif // _payload_format_h
