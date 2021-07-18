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
#include "Inc/commission/commission.h"

#if DEBUG_SERIAL
#define DEBUG_INIT(speed)	Serial1.begin(speed)
#define DEBUG(txt)			Serial1.println(txt)
#define DEBUG_VAL(txt, val)	Serial1.print(txt); Serial1.print(": "); Serial1.println(val)
#define DEBUG_VAL_ARG(txt, val, prec)  Serial1.print(txt); Serial1.print(": "); Serial1.println(val, prec)
#else
#define DEBUG_INIT(speed)
#define DEBUG(txt)
#define DEBUG_VAL(txt, val)
#define DEBUG_VAL_ARG(txt, val, prec)
#endif
