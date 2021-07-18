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

#include "weight.h"

Weight::Weight(uint8_t sup, uint8_t sig1, uint8_t sig2) {
  supPin = sup;
  sigPins[0] = sig1;
  sigPins[1] = sig2;

  for(int ch=0; ch<2; ch++) {
    amplifierGain[ch] = 538.33f;
    Cn[ch] = 2.0f; // in mV/V (Kennwert)
    nominalLoad[ch] = 50.0f; // in kg (Nennlast)
    taraGrams[ch] = 0.0f; // tara load = 0 grams
  }
}

void Weight::init() {
  pinMode(supPin, OUTPUT);
  digitalWrite(supPin, LOW);
}

void Weight::enable() {
  digitalWrite(supPin, HIGH);
  delay(50);
}

void Weight::disable() {
  digitalWrite(supPin, LOW);
}

void Weight::tara() {
  enable();
  for(uint8_t ch = 0; ch < 2; ch++) {
    taraGrams[ch] = getGrams(ch) + taraGrams[ch]; // getGrams adds the previous taraGrams weight
  }
  disable();
}

float Weight::getGrams(uint8_t channel) {
  if(channel > 1) return 0.0f;
  // debugln(F("Read WEIGHT_SIG"));
  analogReadResolution(12); // MKR series has up to 12 bits ADC resolution -> 4096 divisions
  uint16_t adc = analogRead(sigPins[channel]);
  /* The weight is derived from the weight cell properties */
  return adc / 4095.0f * nominalLoad[channel] / amplifierGain[channel] / Cn[channel] * 1000.0f - taraGrams[channel]; // subtract the tara weight
}
