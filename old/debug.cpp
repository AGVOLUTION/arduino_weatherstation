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

#include "debug.h"
#include "Inc/commission/commission.h"

/* Debug output */
void debugInit() {
  #if DEBUG_SERIAL
  Serial1.begin(9600);
  #endif
  return;
}

void debugDeInit() {
  #if DEBUG_SERIAL
  Serial1.end();
  #endif
  return;
}

void debug(const arduino::__FlashStringHelper *str) {
  #if DEBUG_SERIAL
  debug(str);
  #endif
  return;
}
void debug(const String &str) {
  #if DEBUG_SERIAL
  Serial1.print(str);
  #endif
  return;
}

void debug(const char str[]) {
  #if DEBUG_SERIAL
  Serial1.print(str);
  #endif
  return;
}

void debug(char str) {
  #if DEBUG_SERIAL
  Serial1.print(str);
  #endif
  return;
}

void debug(unsigned char str, int base) {
  #if DEBUG_SERIAL
  Serial1.print(str, base);
  #endif
  return;
}

void debug(int str, int base) {
  #if DEBUG_SERIAL
  Serial1.print(str, base);
  #endif
  return;
}

void debug(unsigned int n, int base) {
  #if DEBUG_SERIAL
  Serial1.print(n, base);
  #endif
  return;
}

void debug(long n, int base) {
  #if DEBUG_SERIAL
  Serial1.print(n, base);
  #endif
  return;
}

void debug(unsigned long n, int base) {
  #if DEBUG_SERIAL
  Serial1.print(n, base);
  #endif
  return;
}

void debug(long long n, int base) {
  #if DEBUG_SERIAL
  Serial1.print(n, base);
  #endif
  return;
}

void debug(unsigned long long n, int base) {
  #if DEBUG_SERIAL
  Serial1.print(n, base);
  #endif
  return;
}

void debug(double n, int digits) {
  #if DEBUG_SERIAL
  Serial1.print(n, digits);
  #endif
  return;
}

void debug(const Printable& x) {
  #if DEBUG_SERIAL
  Serial1.print(x);
  #endif
  return;
}


void debugln(const arduino::__FlashStringHelper *ifsh) {
  #if DEBUG_SERIAL
  Serial1.println(ifsh);
  #endif
  return;
}

void debugln(const String &s) {
  #if DEBUG_SERIAL
  Serial1.println(s);
  #endif
  return;
}

void debugln(const char c[]) {
  #if DEBUG_SERIAL
  Serial1.println(c);
  #endif
  return;
}

void debugln(char c) {
  #if DEBUG_SERIAL
  Serial1.println(c);
  #endif
  return;
}

void debugln(unsigned char b, int base) {
  #if DEBUG_SERIAL
  Serial1.println(b, base);
  #endif
  return;
}

void debugln(int num, int base) {
  #if DEBUG_SERIAL
  Serial1.println(num, base);
  #endif
  return;
}

void debugln(unsigned int num, int base) {
  #if DEBUG_SERIAL
  Serial1.println(num, base);
  #endif
  return;
}

void debugln(long num, int base) {
  #if DEBUG_SERIAL
  Serial1.println(num, base);
  #endif
  return;
}

void debugln(unsigned long num, int base) {
  #if DEBUG_SERIAL
  Serial1.println(num, base);
  #endif
  return;
}

void debugln(long long num, int base) {
  #if DEBUG_SERIAL
  Serial1.println(num, base);
  #endif
  return;
}

void debugln(unsigned long long num, int base) {
  #if DEBUG_SERIAL
  Serial1.println(num, base);
  #endif
  return;
}

void debugln(double num, int digits) {
  #if DEBUG_SERIAL
  Serial1.println(num, digits);
  #endif
  return;
}

void debugln(const Printable& x) {
  #if DEBUG_SERIAL
  Serial1.println(x);
  #endif
  return;
}

void debugln(void) {
  #if DEBUG_SERIAL
  Serial1.println();
  #endif
  return;
}
