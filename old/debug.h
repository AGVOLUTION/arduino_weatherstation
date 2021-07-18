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

#pragma once

#include "Arduino.h"

/* Debug output */
void debugInit();

void debugDeInit();

void debug(const arduino::__FlashStringHelper *);
void debug(const String &);
void debug(const char[]);
void debug(char);
void debug(unsigned char, int = DEC);
void debug(int, int = DEC);
void debug(unsigned int, int = DEC);
void debug(long, int = DEC);
void debug(unsigned long, int = DEC);
void debug(long long, int = DEC);
void debug(unsigned long long, int = DEC);
void debug(double, int = 2);
void debug(const Printable&);

void debugln(const arduino::__FlashStringHelper *);
void debugln(const String &s);
void debugln(const char[]);
void debugln(char);
void debugln(unsigned char, int = DEC);
void debugln(int, int = DEC);
void debugln(unsigned int, int = DEC);
void debugln(long, int = DEC);
void debugln(unsigned long, int = DEC);
void debugln(long long, int = DEC);
void debugln(unsigned long long, int = DEC);
void debugln(double, int = 2);
void debugln(const Printable&);
void debugln(void);
