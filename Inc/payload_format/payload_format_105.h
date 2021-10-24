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

#ifndef _PAYLOAD_FORMAT_105_H
#define _PAYLOAD_FORMAT_105_H

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
 * 5		SHT3x I2C ADR 0x44 TEMP MSB		# Temperature (+15cm) in 0.01 °C, signed, 2-complement
 * 6		SHT3x I2C ADR 0x44 TEMP LSB
 * 7		SHT3x I2C ADR 0x44 HUM			# Relative Humidity (+15cm) in 1%
 * 8		SHT3x I2C ADR 0x45 TEMP MSB		# Temperature (+15cm) in 0.01 °C, signed, 2-complement
 * 9		SHT3x I2C ADR 0x45 TEMP LSB
 * 10		SHT3x I2C ADR 0x45 HUM			# Relative Humidity (+15cm) in 1%
 * 11		ERRORS WM0 | WM1 | WM2 | WM3 | SHT3x 0x44 TMP | SHT3x 0x44 HUM | SHT3x 0x45 TMP | SHT3x 045 HUM
 *
 * ERROR FLAGS
 */

// Payload frame format: Must not exceed 51 for LoRa (!)
#define PAYLOAD_LENGTH_105 11
#define PAYLOAD_VERSION_105 105

enum PLD_FORMAT_105 {
	PLD_VERSION_105	= 0,
	PLD_WM_0_105 	= 1,
	PLD_WM_1_105 	= 2,
	PLD_WM_2_105 	= 3,
	PLD_WM_3_105 	= 4,
	PLD_SHT3x_44_105 = 5,
	PLD_SHT3x_45_105 = 8
};

// Error flags bit positions
enum ERR_FLAGS_105 {
	ERR_WM_0_105 = 7,
	ERR_WM_1_105 = 6,
	ERR_WM_2_105 = 5,
	ERR_WM_3_105 = 4,
	ERR_SHT3x_44_TMP_105 = 3,
	ERR_SHT3x_44_HUM_105 = 2,
	ERR_SHT3x_45_TMP_105 = 1,
	ERR_SHT3x_45_HUM_105 = 0
};

// Structure typedef
/* Payload */
typedef struct __attribute__ ((packed)) __payload_105_t {
  uint8_t version = 105;
  uint8_t matrixPotential[4];
  int16_t sht3x_0x44_temperature;
  uint8_t sht3x_0x44_humidity;
  int16_t sht3x_0x45_temperature;
  uint8_t sht3x_0x45_humidity;
  uint8_t errors;
} payload_105_t;

#endif // _payload_format_h
