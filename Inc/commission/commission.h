/*
MIT License

Copyright (c) 2022 Agvolution GmbH
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
#define PCB_REVISION_MAJOR	2
#define PCB_REVISION_MINOR	2
#define PCB_REVISION_PATCH	1

/**************************************************************
 * LPWAN Standard
 * 1: LoRa
 * 2: mioty
 * 3: Sigfox
 * 4: NB-IoT
 * 
 * Note: Only Sigfox MKRFox1200 boards supported so far.
 * LoRa / mioty / Nb-IoT require board integration.
 **************************************************************/
#define LPWAN_STD_LORA 1
#define LPWAN_STD_MIOTY 2
#define LPWAN_STD_SIGFOX 3
#define LPWAN_STD_NBIOT 4
#define LPWAN_STANDARD LPWAN_STD_SIGFOX

/**************************************************************
 * Transmission Interval between Data Packets (minutes)
 **************************************************************/
#define PACKET_INTERVAL_SECONDS	3600 // in seconds
#define WIND_SAMPLING_INTERVAL_SECONDS	900	// in seconds (every 15 min is a good energy compromise for battery stand-alone stations)
#define GPS_FIXTURE_TIMEOUT_SECONDS 85 // in seconds
#define GPS_LOCATION_PACKET_INTERVAL_HOURS 168 // once a week

/*****************************************************************
 * Debugging
 * ***************************************************************/
/* For debug purpose */
#define DEBUG_SERIAL            0 // printf debugging using Serial1 (RX/TX pins 13/14)
#define DEBUG_NO_LPMODE         0 // Use delay instead of low power mode -> Wake-up routing and wake-up interrupts won't work
#define DEBUG_SKIP_TX           0 // Skip data packet transmission
#define DEBUG_LED               0 // Use debug LED to display on / low power state

#endif // _commission_h_