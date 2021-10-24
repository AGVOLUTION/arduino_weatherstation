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

#ifndef _PAYLOAD_FORMAT_107_H
#define _PAYLOAD_FORMAT_107_H

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
 * 1		BME280 I2C ADR 0x76 TEMP MSB		# Temperature (+40cm) in 0.01 °C, signed, 2-complement
 * 2		BME280 I2C ADR 0x76 TEMP LSB
 * 3		BME280 I2C ADR 0x76 HUM				# Relative Humidity (+40cm) in 1%
 * 4		BME280 I2C ADR 0x76 PRES MSB		# Pressure (+40cm) in 0.1 hPa
 * 5		BME280 I2C ADR 0x76 PRES LSB
 * 6		BME280 I2C ADR 0x77 TEMP MSB		# Temperature (+40cm) in 0.01 °C, signed, 2-complement
 * 7		BME280 I2C ADR 0x77 TEMP LSB
 * 8		BME280 I2C ADR 0x77 HUM				# Relative Humidity (+40cm) in 1%
 * 9		BME280 I2C ADR 0x77 PRES MSB		# Pressure (+40cm) in 0.1 hPa
 * 10		BME280 I2C ADR 0x77 PRES LSB
 * 11		ERRORS 0 | 0 | 0x76 TEMP | 0x76 HUM | 0x76 PRES | 0x77 TEMP | 0x77 HUM | 0x77 PRES
 */

// Payload frame format: Must not exceed 51 for LoRa (!)
#define PAYLOAD_LENGTH_107 12
#define PAYLOAD_VERSION_107 107

enum PLD_FORMAT_107 {
	PLD_VERSION_107	= 0,
	PLD_BME76_107	= 1,
	PLD_RAIN_107	= 6,
	PLD_RAIN_ACC_107 = 7,
	PLD_WIND_DIR_107 = 9,
	PLD_ERROR_BME76_107	= 9,
	PLD_ERROR_BME76_POS_107 = 0,
	PLD_WIND_SPD_AVG_107 = 10,
	PLD_WIND_SPD_STD_107 = 11
};

// Error flags bit positions
enum ERR_FLAGS_107 {
  ERR_BME76_TEMP_107 = 5,
  ERR_BME76_HUM_107 = 4,
  ERR_BME76_PRES_107 = 3,
  ERR_BME77_TEMP_107 = 2,
  ERR_BME77_HUM_107 = 1,
  ERR_BME77_PRES_107 = 0
};

// Structure typedef
/* Payload */
typedef struct __attribute__ ((packed)) __payload_107_t {
  uint8_t version = 107;
  int16_t bme76_temperature;
  uint8_t bme76_humidity;
  uint16_t bme76_pressure;
  int16_t bme77_temperature;
  uint8_t bme77_humidity;
  uint16_t bme77_pressure;
  uint8_t errors;
} payload_107_t;

#endif // _payload_format_h
