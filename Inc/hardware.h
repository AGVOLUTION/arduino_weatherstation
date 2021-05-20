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

#ifndef _HARDWARE_H
#define _HARDWARE_H

/* Specify hardware layout */
#define DAVIS_RAIN_Pin          0
#define DS18B20_Pin             7
#define WM_EXC2_Pin             5
#define WM_EXC1_Pin             2
#define DAVIS_WIND_SPD_Pin      1
#define WM_B_Pin                1
#define WM_A_Pin                A5
#define WM_RESISTANCE_Pin       A6
#define WM_SWITCH_SUP           A4
#define DAVIS_WIND_DIR_Pin      A5
#define DAVIS_WIND_SUP_Pin      A4
#define AGV_MOISTURE_EN_Pin     A3
#define GPS_SUP_Pin             A2
#define LEAFWET_SIG_Pin         A1
#define LEAFWET_SUP_Pin         A0
#define WEIGHT_SIG_POS_Pin		A1
#define WEIGHT_SIG_NEG_Pin		A3
#define WEIGHT_SUP_Pin			A0
#define SDLOG_CS_Pin            4

#endif // _HARDWARE_H