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

#include "davisWind.h"
#include "Inc/commission/commission.h"
#include "Inc/hardware.h"

static uint16_t counterInternal;

void davisWindAddSpdSample(davisWind_t *davisWind) {
  if( (davisWind->sample < 0) || (davisWind->sample >= DAVIS_WIND_SPD_NUM)) {
    return;
  }

  /*
  // Millis counter is close (5s) to overflowing
  uint32_t tnow = millis();
  if(tnow > (0xffffffff - 5000)) {
    return;
  }
  */

  // Take measurement
  davisWind->counter = 0;
  counterInternal = 0;

  /*
  uint8_t oldState, newState;
  // Count falling edges in poll mode
  while(millis() < (tnow + 3000)) {
    oldState = digitalRead(DAVIS_WIND_SPD_Pin);
    delayMicroseconds(100);
    newState = digitalRead(DAVIS_WIND_SPD_Pin);
    if( (oldState == HIGH) && (newState == LOW)) {
      davisWind->counter++;
      Serial1.println(F("DAVIS_WIND_SPD COUNT EVENT"));
      delay(10); // dead time
    }
  }
  */
  //interrupts();
  attachInterrupt(DAVIS_WIND_SPD_Pin, davisWindIncrementSpdCnt, FALLING);
  delay(3000);
  detachInterrupt(DAVIS_WIND_SPD_Pin);

  /* Convert average pulse frequency to wind speed
   * v[km/h] = f[Hz] * 2.25 * 1.61
   * Explanation:
   * 2.25: Davis Anemometer pulse generator property
   * 1.61 mph -> km/h
   * Sampling time: 3 seconds
   */
  davisWind->spdSamples[davisWind->sample] = counterInternal / 3.0 * 2.25 * 1.61;
  davisWind->sample++;
}

void davisWindIncrementSpdCnt() {
  counterInternal++; 
}

float davisWindSpdAvg(davisWind_t *davisWind) {
  if(davisWind->sample == 0) { // no speed measurements recorded
    return 0.0f;
  }
  
  float sum = 0.0f;
  for(uint16_t i = 0; i < davisWind->sample; i++) {
    sum += davisWind->spdSamples[i];
  }
  return sum / davisWind->sample;
}

float davisWindSpdStd(davisWind_t *davisWind) {
  if(davisWind->sample == 0) { // no speed measurements recorded
    return 0.0f;
  }
  
  float sum = 0.0f;
  float avg = davisWindSpdAvg(davisWind);
  float diff;
  for(uint16_t i = 0; i < davisWind->sample; i++) {
    diff = davisWind->spdSamples[i] - avg;
    sum += diff * diff;
  }
  return sqrt(sum / davisWind->sample);
}
