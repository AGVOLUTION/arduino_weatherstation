/*
MIT License

Copyright (c) 2021 Agvolution GmbH
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

#include "wind.h"

bool Wind::init() {
  pinMode(spdPin, INPUT_PULLUP);
  pinMode(supPin, OUTPUT);
  digitalWrite(supPin, LOW);
  isInit = true;
}

// Add a wind speed measurement in km/h to storage array -> average / std dev is taken later on
void Wind::addSpdSample(float val) {
  if(!isInit) return;

  if(sample >= WIND_SPD_NUM) {
    return;
  }

  spdSamples[sample] = val;
  sample++;
}

// Average of all speed samples in km/h
float Wind::spdAvg() {
  if(sample == 0) { // no speed measurements recorded
    return 0.0f;
  }
  
  float sum = 0.0f;
  for(uint16_t i = 0; i < sample; i++) {
    sum += spdSamples[i];
  }
  return sum / sample;
}

// Standard deviation of all speed samples in km/h
float Wind::spdStd() {
  if(sample == 0) { // no speed measurements recorded
    return 0.0f;
  }
  
  float sum = 0.0f;
  float avg = spdAvg();
  float diff;
  for(uint16_t i = 0; i < sample; i++) {
    diff = spdSamples[i] - avg;
    sum += diff * diff;
  }
  // A bias-free variance estimator is normalized by n-1 -> only if sample >= 2 to avoid division by zero!
  uint16_t n = 1;
  if(sample > 2)
  {
    n = sample - 1;
  }
  return sqrt(sum / n);
}

float Wind::direction() {
  // debugln(F("Turn on DAVIS_WIND_SUP"));
  digitalWrite(supPin, HIGH); // Turn on voltage supply for wind poti
  delay(100); // Wait until stable voltage supply operation
  
  double windPoti;
  analogReadResolution(10);
  windPoti = analogRead(dirPin);
  digitalWrite(supPin, LOW); // Turn off voltage supply of wind poti
  // ADC resolution is set to 10 bit -> 0 = 0°, 1023 = 360°
  return windPoti / 1023.0f * 360.0f;
}
