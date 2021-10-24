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
  csPin = cs;
  iniSuccessful = false;
}

bool SDLog::begin(String header, String units)
{
  if (iniSuccessful) // already initialized
  {
    return true;
  }
  
  iniSuccessful = true;
  DEBUG("Accessing SD Card");
  if (!SD.begin(csPin)) { // can not access SD card
    DEBUG("No access to SD Card");
    iniSuccessful = false;
    return false;
  }

  File headerfile = SD.open("header.txt", FILE_WRITE);
  if (! headerfile) {
    DEBUG("Cant open headerfile");
    iniSuccessful = false;
    return false;
  }

  headerfile.println(header);
  headerfile.println(units);
  headerfile.close();

  datafile = SD.open("log.txt", FILE_WRITE);
  if (! datafile) {
    DEBUG("Cant open datafile");
    iniSuccessful = false;
    return false;
  }
  else
  {
    datafile.close();
  }
  return true;
}

bool SDLog::write(String str)
{
  if(!iniSuccessful) // SD card not initialized yet
  {
    return false;
  }

  datafile = SD.open("log.txt", FILE_WRITE);
  datafile.println(str);
  datafile.close(); // ensures that data is physically written to file
}
