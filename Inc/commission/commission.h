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

/* This source contains device-specific data (uuids, euis, encryption keys, ...) */

#ifndef _commission_h_
#define _commission_h_

/**************************************************************
 * PCB Revision
 **************************************************************/
#define PCB_REVISION_MAJOR	1
#define PCB_REVISION_MINOR	0
#define PCB_REVISION_PATCH	0

/**************************************************************
 * LPWAN Standard
 * 1: LoRa
 * 2: MIOTY
 * 3: Sigfox
 * 4: NB-IoT
 **************************************************************/
#define LPWAN_STD_LORA 1
#define LPWAN_STD_MIOTY 2
#define LPWAN_STD_SIGFOX 3
#define LPWAN_STD_NBIOT 4
#define LPWAN_STANDARD LPWAN_STD_SIGFOX

/**************************************************************
 * REGION
 * As preprocessor directive we set during compilation:
 * #define CFG_eu868 / CFG_us915
 **************************************************************/


/**************************************************************
 * RADIO MODULE
 * As preprocessor directive we set during compilation:
 * Semtech SX1276 radio chip on RFM95W module
 * #define CFG_sx1276_radio
 **************************************************************/


/**************************************************************
 * LORAWAN ACTIVATION
 **************************************************************/
// Define integer equivalents to perform '=='
#define ABP		0
#define OTAA	1

// specify activation method here (ABP / OTAA)
#define ACTIVATION ABP


/**************************************************************
 * LORAWAN Session Keys
 **************************************************************/
#if (LPWAN_STANDARD == LPWAN_STD_LORA)
#if (ACTIVATION == OTAA)
// network ID
static const u1_t NET_ID = 0x13;
// application router ID (LSByte first)
static const u1_t APPEUI[8]  = { [APPEUI] };
// unique device ID (LSByte first)
// static const u1_t DEVEUI[8]  = { 0x6B, 0xFA, 0x55, 0x56, 0xE5, 0x23, 0xE9, 0x00 };
// L1 Test Device
static const u1_t DEVEUI[8]  = { [DEVEUI] };
// device-specific AES key (MSByte first) (derived from device EUI) (= AppKey)
// static const u1_t DEVKEY[16] = { 0x71, 0x5C, 0x33, 0xF2, 0x1A, 0x74, 0x10, 0xF6, 0x7E, 0x2D, 0x7C, 0x3B, 0x4E, 0x2B, 0x30, 0x42 };
// L1 Test Device
static const u1_t DEVKEY[16] = { [DEVKEY] };

#elif (ACTIVATION == ABP)
static const u1_t DEVEUI[8]  = { 0x28, 0x8F, 0xA5, 0xA7, 0xF2, 0xF6, 0x51, 0xCE };

// network ID
static const u1_t NET_ID = 0x13;
// Network Session Key NwkSKey (MSByte first)
static const u1_t NWKSKEY[16] = { 0xFB, 0x9D, 0xA5, 0xD4, 0x2A, 0x03, 0xF4, 0x6A, 0x1A, 0xBC, 0x6D, 0x09, 0x15, 0x15, 0xE3, 0x84 };
// Application Session Key AppSKey (MSByte first)
static const u1_t APPSKEY[16] = { 0x1D, 0x3C, 0xF9, 0xD6, 0x99, 0x12, 0x4C, 0x3E, 0x8D, 0x78, 0xE7, 0xE4, 0x7B, 0xAC, 0x0E, 0x94 };
// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR = 0x2601158D;

#define LORA_ABP_TX_DR_SF DR_SF8

#endif // ACTIVATION

#define LORA_REMOVE_RX_WND

/**************************************************************
 * MIOTY Personalisation
 **************************************************************/
#elif (LPWAN_STANDARD == LPWAN_STD_MIOTY)

#endif

/**************************************************************
 * Transmission Interval between Data Packets (minutes)
 **************************************************************/
#define PACKET_INTERVAL_INITIAL	60
// #define PACKET_INTERVAL_ENERGY_LOW	60	// low = 15% until power supply shutdown
// #define PACKET_INTERVAL_ENERGY_CRIT	60	// critical = 10% until power supply shutdown

/**************************************************************
 * Module Activation (Which modules are equipped on the PCB)
 **************************************************************/
 /* Possible equipment */
#define EQUIP_SDLOG 1

#define EQUIP_BME76 1
#define EQUIP_DAVIS_RAIN 1
#define EQUIP_DAVIS_WIND 1
#define WIND_SAMPLING_RATE	180	// in seconds

#define EQUIP_GPS 0

#define EQUIP_WM 0
#define EQUIP_DS18B20 0
#define EQUIP_LEAFWET 0
#define EQUIP_FDC 0

#define EQUIP_BME77 0
#define EQUIP_TCN 0

#endif // _commission_h_

/*****************************************************************
 * Debugging
 * ***************************************************************/
/* For debug purpose */
#define DEBUG_SERIAL            1 // printf debugging using Serial1 (RX/TX pins 13/14)
#define DEBUG_NO_LPMODE         0 // use delay instead of low power mode -> routing and wakeup interrupts wont work
#define DEBUG_SKIP_TX           0 // skip data packet transmission
#define DEBUG_LED               0 // use debug led to display on / low power state