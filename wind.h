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

// Maximum number of samples during one packet cycle
#define WIND_SPD_NUM  120

class Wind {
  public:
    uint16_t sample;
    float spdSamples[WIND_SPD_NUM];

    Wind(uint8_t spdPin, uint8_t supPin, uint8_t dirPin) {
      sample = 0;
      spdCounter = 0;
      spdPin = spdPin;
      supPin = supPin;
      dirPin = dirPin;
    }

    bool init();            // Initializes GPIOs
    void addSpdSample(float val); // Add a speed measurement
    float spdAvg();         // Average of all speed samples in km/h
    float spdStd();         // Std. Deviation of all speed samples in km/h
    float direction();      // Wind direction in °

  private:
    uint16_t spdCounter;
    uint8_t spdPin;
    uint8_t supPin;
    uint8_t dirPin;
    bool isInit;  // dedicated "Class initialized" flag
};


