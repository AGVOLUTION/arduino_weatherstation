#include "ArduinoLowPower.h"
#include <RTCZero.h>
#include <SigFox.h>
#include <SD.h>
#include <Wire.h>

#define SDLOG_CS_Pin            4
#define DAVIS_RAIN_Pin          0
#define DAVIS_WIND_SUP_Pin      A4
#define DAVIS_WIND_SPD_Pin      1

#define EQUIP_SD                0

#if EQUIP_SD
typedef struct __sdlog_t {
  File datafile;
  uint8_t cardPresent;
} sdlog_t;

sdlog_t sdlog;
#endif

RTCZero rtc;

void setup() {
  SigFox.begin();
  delay(1000);
  SigFox.end();
  Wire.begin();
  rtc.begin();
  Serial.begin(38400);

  pinMode(DAVIS_RAIN_Pin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(DAVIS_RAIN_Pin), davisRainCountIncrement, FALLING);
  pinMode(DAVIS_WIND_SPD_Pin, INPUT_PULLUP);
  pinMode(DAVIS_WIND_SUP_Pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // put your setup code here, to run once:
  // Initialize SD Card
  #if EQUIP_SD
  if (!SD.begin(SDLOG_CS_Pin)) {
    sdlog.cardPresent = 0;
  }
  else {
    sdlog.cardPresent = 1;
  }

  if(sdlog.cardPresent) {
    File headerfile = SD.open("header.txt", FILE_WRITE);
    String headerString = "T,RH,P,WIND_DIR,WIND_SPD_AVG,WIND_SPD_STD";
    headerfile.println(headerString);
    headerfile.close();
  
    sdlog.datafile = SD.open("log.txt", FILE_WRITE);
  }
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:
  LowPower.sleep();
}
