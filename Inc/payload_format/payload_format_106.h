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

#ifndef _PAYLOAD_FORMAT_106_H
#define _PAYLOAD_FORMAT_106_H

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
 * 1		DS18B20 TEMP MSB		  # Temperature (+40cm) in 0.01 °C, signed, 2-complement
 * 2		DS18B20 TEMP LSB
 * 3		RAIN COUNT PRONAMIC							# Rain counts since last data packet (in 0.581 mm/m^2)
 * 4		RAIN COUNT ACC MSB PRONAMIC			# Rain counts accumulated since sensor activation (16 bit overflow), (in 0.581 mm/m^2)
 * 5		RAIN COUNT ACC LSB PRONAMIC
 * 6		RAIN COUNT DAVIS						    # Rain counts since last data packet (in 0.581 mm/m^2)
 * 7		RAIN COUNT ACC MSB DAVIS				# Rain counts accumulated since sensor activation (16 bit overflow), (in 0.581 mm/m^2)
 * 8		RAIN COUNT ACC LSB DAVIS
 * 9		(WIND_DIR in 4° & 0xfe) | (0x01 ERR_DS18B20)
 * 10		WIND_SPD_AVG						# in 1 km/h
 * 11		WIND_SPD_STD						# in 1 km/h
 */

// Payload frame format: Must not exceed 51 for LoRa (!)
#define PAYLOAD_LENGTH_106 12
#define PAYLOAD_VERSION_106 106

enum PLD_FORMAT_106 {
	PLD_VERSION_106	= 0,
	PLD_DS18B20_106	= 1,
	PLD_RAIN_PRONAMIC_106	= 3,
	PLD_RAIN_ACC_PRONAMIC_106 = 4,
	PLD_RAIN_DAVIS_106	= 6,
	PLD_RAIN_ACC_DAVIS_106 = 7,
	PLD_WIND_DIR_DAVIS_106 = 9,
	PLD_ERROR_DS18B20_106	= 9,
	PLD_WIND_SPD_AVG_106 = 10,
	PLD_WIND_SPD_STD_106 = 11
};

// Error flags bit positions
enum ERR_FLAGS_106 {
  ERR_DS18B20_106 = 0
};

// Structure typedef
/* Payload */
typedef struct __attribute__ ((packed)) __payload_106_t {
  uint8_t version = 106;
  int16_t temperature;
  uint8_t rainCountPronamic;
  uint16_t rainCountPronamicAcc;
  uint8_t rainCountDavis;
  uint16_t rainCountDavisAcc;
  uint8_t windDirAndError;
  uint8_t windSpdAvg;
  uint8_t windSpdStd;
} payload_106_t;

#endif // _payload_format_h
