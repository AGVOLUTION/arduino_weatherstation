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

#include "watermark.h"
#include "debug.h"

Watermark::Watermark(uint8_t exc1Pin, uint8_t exc2Pin, uint8_t resPin, uint8_t supPin, uint8_t aPin, uint8_t bPin) {
  exc1 = exc1Pin;
  exc2 = exc2Pin;
  res = resPin;
  sup = supPin;
  a = aPin;
  b = bPin;

  pinMode(exc1, OUTPUT);
  pinMode(exc2, OUTPUT);
  pinMode(sup, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);

  digitalWrite(exc1, LOW);
  digitalWrite(exc2, LOW);
  digitalWrite(sup, LOW);
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
}

void Watermark::activate() {
  /* Power analog multiplexer */
  PORT->Group[g_APinDescription[sup].ulPort].PINCFG[g_APinDescription[sup].ulPin].reg |= 0x40 ; // modify output drive current on sup -> select higher drive strength
  digitalWrite(sup, HIGH);
  digitalWrite(exc1, LOW); // shorten signals for safety
  digitalWrite(exc2, LOW);
  delay(50); // voltage supply stabilization
}

void Watermark::deactivate() {
  digitalWrite(sup, LOW);
  digitalWrite(exc1, LOW); // shorten signals for safety
  digitalWrite(exc2, LOW);
  digitalWrite(a, LOW); // prevent leakage currents through switch select A/B
  digitalWrite(b, LOW);
  PORT->Group[g_APinDescription[sup].ulPort].PINCFG[g_APinDescription[sup].ulPin].reg &= ~(0x40) ; // reset output drive current on sup -> select normal drive strength
}

uint8_t Watermark::matrixPotentialCbars(uint8_t channel) {
  /* Connect channel */
  digitalWrite(a, channel & 0x01); // Multiplexer with binary input line
  digitalWrite(b, (channel & 0x02) >> 1);

  analogReadResolution(12);

  /* Measure current direction 1, tDelay=10ms */
  // DEBUG("DIR1 CH" + String(channel));
  digitalWrite(exc1, HIGH);
  digitalWrite(exc2, LOW);
  delay(10);
  float dir1 = analogRead(res);

  /* Measure current direction 2, tDelay=10ms */
  // DEBUG("DIR2 CH" + String(channel));
  digitalWrite(exc1, LOW);
  digitalWrite(exc2, HIGH);
  delay(10);
  float dir2 = analogRead(res);

  /* Shorten the watermark analog lines to clear any charges */
  digitalWrite(exc2, LOW);
  // DEBUG_VAL(String("ADC dir1 Ch")+String(channel), dir1);
  // DEBUG_VAL(String("ADC dir2 Ch")+String(channel), dir2);
  /* Compute resistance values measured. Using the formulas for series resistors (voltage divider) */
  dir1 = 7870.0f / (4095.0f / dir1 - 1.0f); // 4095 = ADC maximum, 7870 = R3 in ohms
  dir2 = (4095.0f / dir2 - 1.0f) * 7870.0f; // Rsense = (3.3V / U(7.87k) - 1) * 7.87k

  // The internal resistance of the analog multiplexer is 97 ohm @ room temperature
  dir1 -= 2*97;
  dir2 -= 2*97;
  
  float Rsense = (dir1 + dir2) / 2.0f;
  // Crop if maximum R (>255 cbars) exceeded
  if(Rsense >= 39300) Rsense = 39300;
  // DEBUG_VAL(String("dir1 Ch")+String(channel), dir1);
  // DEBUG_VAL(String("dir2 Ch")+String(channel), dir2);
  // DEBUG_VAL(String("Rsense Ch")+String(channel), Rsense);

  /* Compute matrix potential in cbar */
  /* Reference: https://www.irrometer.com/200ss.html */
  float cbars;
  float TempC = 16; // assume soil temperature
  if(Rsense <= 550) { // Resistor value interval bounds
    cbars = 0;
  }
  else if((Rsense > 550) && (Rsense <= 1000)) {
    cbars = ( 23.156f * ( Rsense / 1000.0f ) - 12.736f ) * ( 1.0f+ 0.018f * ( TempC - 24.0f ) ); // note- updated May 2021
  }
  else if((Rsense > 1000) && (Rsense <= 8000)) {
    cbars = ( -3.213f * ( Rsense / 1000.0f ) - 4.093f ) / ( 1.0f- 0.009733f * ( Rsense / 1000.0f ) - 0.01205f * TempC);
  }
  else { // > 8kOhms
    cbars = -2.246f - 5.239f * ( Rsense / 1000.0f ) * ( 1.0f+ 0.018f * ( TempC - 24.0f ) ) - 0.06756f * ( Rsense / 1000.0f ) * ( Rsense / 1000.0f ) * ( ( 1.0f + 0.018f * (TempC-24.0f)) * (1.0f + 0.018f * (TempC - 24.0f)));
  }
  // DEBUG_VAL(String("cbars Ch")+String(channel), cbars);
  // DEBUG("");
  int cbarsInt = round(cbars);
  cbarsInt = abs(cbarsInt); // Schönheitsfehler: Manche Formeln benutzen Konvention cbars < 0, in anderen Intervallen gilt cbars > 0. 
  // Crop if cbars wont fit the 8bit payload
  if(cbarsInt > 0xff) cbarsInt = 0xff;
  return (uint8_t)(cbarsInt);
}
