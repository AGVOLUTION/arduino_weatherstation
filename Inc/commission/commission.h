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
#define PCB_REVISION_MAJOR	2
#define PCB_REVISION_MINOR	0
#define PCB_REVISION_PATCH	0

/**************************************************************
 * LPWAN Standard
 * 3: Sigfox
 * 4: NB-IoT
 **************************************************************/
#define LPWAN_STD_SIGFOX 3
#define LPWAN_STD_NBIOT 4
#define LPWAN_STANDARD LPWAN_STD_SIGFOX

/**************************************************************
 * Transmission Interval between Data Packets (minutes)
 **************************************************************/
#define PACKET_INTERVAL_INITIAL	60
#define WIND_SAMPLING_RATE	180	// in seconds
#define GPS_FIXTURE_TIMEOUT 90 // in seconds

/*****************************************************************
 * Debugging
 * ***************************************************************/
/* For debug purpose */
#define DEBUG_SERIAL            1 // printf debugging using Serial1 (RX/TX pins 13/14)
#define DEBUG_NO_LPMODE         1 // use delay instead of low power mode -> routing and wakeup interrupts wont work
#define DEBUG_SKIP_TX           0 // skip data packet transmission
#define DEBUG_LED               0 // use debug led to display on / low power state

#endif // _commission_h_