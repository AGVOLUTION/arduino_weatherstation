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

#include "sdlog.h"

SDLog::SDLog(uint8_t cs) {
  iniSuccessful = true;

  if (!SD.begin(cs)) { // can not access SD card
    iniSuccessful = false;
    return;
  }

  File headerfile = SD.open("header.txt", FILE_WRITE);
  if (! headerfile) {
    // debugln(F("Error opening SD headerfile."));
    iniSuccessful = false;
    return;
  }

  String headerString = "T,RH,P,RAIN/LATEST,RAIN/ACC,WIND/DIR,WIND/SPD/AVG,WIND/SPD/STD";
  headerfile.println(headerString);
  String unitString = "°C,%,hPa,COUNTS,COUNTS,°,km h-1,km h-1";
  headerfile.println(unitString);
  headerfile.close();

  datafile = SD.open("log.txt", FILE_WRITE);
  if (! datafile) {
    // debugln(F("Error opening SD logfile."));
    iniSuccessful = false;
    return;
  }
    // debugln(F("Completed SD setup."));
}