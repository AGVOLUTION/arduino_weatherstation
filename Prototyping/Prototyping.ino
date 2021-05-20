/*
  TimedWakeup

  This sketch demonstrates the usage of Internal Interrupts to wakeup a chip in sleep mode.
  Sleep modes allow a significant drop in the power usage of a board while it does nothing waiting for an event to happen. Battery powered application can take advantage of these modes to enhance battery life significantly.

  In this sketch, the internal RTC will wake up the processor every 2 seconds.
  Please note that, if the processor is sleeping, a new sketch can't be uploaded. To overcome this, manually reset the board (usually with a single or double tap to the RESET button)

  This example code is in the public domain.
*/

#include "ArduinoLowPower.h"
#include <RTCZero.h>

RTCZero rtc;

void setup() {
  Serial.begin(38400);
  Serial.println("Booting.");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  // Uncomment this function if you wish to attach function dummy when RTC wakes up the chip
  LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, lpmode_int_rtc, CHANGE);
  LowPower.attachInterruptWakeup(0, lpmode_int_rainDavis, FALLING);
  blinkLed(10);
  rtc.begin();
}

int cnt=0;

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  blinkLed(cnt);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
  // Triggers a 2000 ms sleep (the device will be woken up only by the registered wakeup sources and by internal RTC)
  // The power consumption of the chip will drop consistently
  Serial.println("Device entering lpmode.");
  Serial.print("System time: ");
  Serial.println(millis());
  /*
   * Different sleep modes:
   * idle(), I=, --> did not work
   * sleep(), I=, --> MCU disconnects virtual serial port after enerting lpmode and does not reconnect. Wakes up from RTC and external interrupt and executes callbacks, but only if they contain a minor amount of instructions (e.g. set a variable, but no LED switching / UART)... Important: Entprellen per SW! Entry point is after LowPower.sleep() instruction.
   * deepSleep(), I=, --> MCU disconnects virtual serial port after enerting lpmode and does not reconnect. Wakes up from RTC and external interrupt and executes callbacks, but only if they contain a minor amount of instructions (e.g. set a variable, but no LED switching / UART)... Important: Entprellen per SW! Entry point is after LowPower.sleep() instruction.
   */
  LowPower.deepSleep(5000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(5000);
  Serial.begin(38400);
  Serial.println("Waking up from lpmode.");
  Serial.print("System time: ");
  Serial.println(millis());
}

void lpmode_int_rtc() {
  Serial.println("Called LPMODE_INT_RTC.");
  blinkLed(5);
}

void lpmode_int_rainDavis() {
  Serial.println("Called LPMODE_INT_RAINDAVIS");
  //blinkLed(10);
  cnt++;
}

void blinkLed(uint16_t n) {
  for(uint16_t i=0;i<n;i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
  }
}
