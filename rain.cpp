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

#include "rain.h"

Rain::Rain(FlashStorageClass<uint16_t> *rainAccFlash, FlashStorageClass<uint16_t> *rainAccFlashSeal) {
  accFlash = rainAccFlash;
  accFlashSeal = rainAccFlashSeal;
  lastCountOstime = 0;
  current = 0;
  // Check if Flash area which stores the rain accumulation is properly initialized
  if(accFlashSeal->read() != 0xaffe) {
    // Initialize -> set acc counter to zero and destroy seal
    accFlash->write(0);
    accFlashSeal->write(0xaffe);
  }
}

/* Returns true if rain count has been counted */
bool Rain::increment(uint32_t nowTimeSeconds) {
  // count only if there is at  least one second dead time
  if(nowTimeSeconds > lastCountOstime) {
    current++;  // accumulate non-persistent counter
    uint16_t rainAccTmp = accFlash->read();
    accFlash->write(rainAccTmp+1);
    lastCountOstime = nowTimeSeconds;
    return true;
  }
  return false;
}

uint8_t Rain::getCounts() {
  return current;
}

uint16_t Rain::getAccCounts() {
  return accFlash->read();
}

void Rain::resetCounts() {
  current = 0;
}
