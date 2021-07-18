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

#ifndef _PAYLOAD_FORMAT_103_H
#define _PAYLOAD_FORMAT_103_H

/**************************************************************
 * CONVERSION INFORMATION
 **************************************************************
 * TCN75A 12-bit code
 * Soil Temperature T (°C) = (code & 0x7ff) * 2**(-4); if code & 0x800: T *= -1 (sign bit).
 *
 **************************************************************/

/**************************************************************
 * Payload specification
 **************************************************************
 * Byte		Parameter
 * 0		VERSION								# number (0..255)
 * 1		FDC CH0		Normalized dielectric permittivity in %
 * 2		FDC CH1		Normalized dielectric permittivity in %
 * 3		FDC CH2		Normalized dielectric permittivity in %
 * 4		FDC CH3		Normalized dielectric permittivity in %
 * 5		TCN75A AA							# A 0x48: -15cm, B 0x49: -30cm, C 0x4a: -60cm, D 0x4b: -90cm
 * 6		TCN75A AB							# Ensure this frame format:
 * 7		TCN75A BB							# A & 0xff00, (A & 0x00f0) | (B & 0xf000) >> 12, (B & 0x0ff0) >> 4, ... where A = 16-bit code of TCN75A
 * 8		TCN75A CC							# Conversion, for example: 1. Take 16 bit 2-complement, 2. Shift right >> 4 bit, 3. * 0.0625
 * 9		TCN75A CD
 * 10		TCN75A DD
 * 11		ERROR
 */

// Payload frame format: Must not exceed 51 for LoRa (!)
#define PAYLOAD_LENGTH_103 11
#define PAYLOAD_VERSION_103 103

enum PLD_FORMAT_103 {
	PLD_VERSION_103	= 0,
	PLD_FDC_CH0_103	= 1,
	PLD_FDC_CH1_103	= 2,
	PLD_FDC_CH2_103	= 3,
	PLD_FDC_CH3_103	= 4,
	PLD_TCNs_103	= 5,
	PLD_ERROR_103	= 11
};

// Error flags bit positions
enum ERR_FLAGS_103 {
	ERR_TCN_48_103 = 0,	// ensure TCN channels are placed next to each other
	ERR_TCN_49_103 = 1,
	ERR_TCN_4a_103 = 2,
	ERR_TCN_4b_103 = 3,
	ERR_FDC_CH0_103 = 4,	// ensure FDC channels are placed next to each other
	ERR_FDC_CH1_103 = 5,
	ERR_FDC_CH2_103 = 6,
	ERR_FDC_CH3_103 = 7
};

// Structure typedef
/* Payload */
typedef struct __attribute__ ((packed)) __payload_103_t {
  uint8_t version = 103;
  uint8_t fdcCh[4];
  uint8_t tcns[6];
  uint8_t error;
} payload_103_t;

#endif // _payload_format_h
