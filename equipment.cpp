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

#include "equipment.h"
#include "debug.h"

Equipment::Equipment() {
	  bme76     = false;
    bme77     = false;
    weight1   = false;
    weight2   = false;
    watermark = false; // at least one Watermark tensiometer
    gps       = false;
    leafWet   = false;
    wind      = false;
    rain      = false;
    climavi   = false;
    ds18b20   = false;
}

void Equipment::mutuallyExclude() {
	// Mutual equipment exclusion
	// 1. GPS <-> LEAF_WET -> GPS overrides Leaf wetness
	if(gps) {
		leafWet = 0;
	}

	// 2. Climavi overrides Weight 1
	if(climavi) {
		weight1 = 0;
	}

	// 3. Watermark overrides Wind
	if(watermark) {
		wind = 0;
	}

	// 4. Watermark overrides Weight 1+2
	if(watermark) {
		weight1 = 0;
		weight2 = 0;
	}
}

// Print the whole equipment list on Serial1
void Equipment::print() {
  DEBUG("Equipment list:");
  DEBUG_VAL("    BME76", bme76);
  DEBUG_VAL("    BME77", bme77);
  DEBUG_VAL("  WEIGHT1", weight1);
  DEBUG_VAL("  WEIGHT2", weight2);
  DEBUG_VAL("WATERMARK", watermark);
  DEBUG_VAL("      GPS", gps);
  DEBUG_VAL("  LEAFWET", leafWet);
  DEBUG_VAL("     WIND", wind);
  DEBUG_VAL("     RAIN", rain);
  DEBUG_VAL("  CLIMAVI", climavi);
  DEBUG_VAL("  DS18B20", ds18b20);
  DEBUG();
}
