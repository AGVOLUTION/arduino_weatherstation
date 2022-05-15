/*
MIT License

Copyright (c) 2022 Agvolution GmbH
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

/* Pin definitions and equipment */
#include "Inc/hardware.h"     // Contains Pinout
#include "equipment.h"        // Stores the modules equipped
#include "Inc/commission/commission.h"  // Configuration of packet interval, ...
#include "Inc/buildInfo.h"

/* Build info segment */
buildInfo_t const buildInfo __attribute__((used)) = {
    .identifier = BUILDINFO_IDENTIFIER,
    .naming     = "MKR Weatherstation",
    .vendor     = "Agvolution GmbH",
    .v_major    = 2,
    .v_minor    = 1,
    .v_patch    = 2,
    .buildDate  = __DATE__,
    .buildTime  = __TIME__
};

/* Payload definition */
#include "Inc/payload_format/payload_format_101.h"
#include "Inc/payload_format/payload_format_103.h"
#include "Inc/payload_format/payload_format_104.h"
#include "Inc/payload_format/payload_format_105.h"
#include "Inc/payload_format/payload_format_106.h"
#include "Inc/payload_format/payload_format_107.h"

/* Modules */
#include "wind.h"             // Wind sensor
#include "rain.h"             // Rain counter
#include "bme280.h"           //
#include "SHT31.h"            // SHT31 T/RH sensor
#include "watermark.h"        // Watermark Soil Matrix Potential
#include <DS18B20.h>          //
#include "weight.h"           // Weight cells
#include "debug.h"            // 
#include "ArduinoLowPower.h"  //
#include "rtc.h"              //
#include <SigFox.h>           //
#include <Wire.h>             //
#include "sdlog.h"            //

// The modules equipped
Equipment equip;

// Declare new payload buffer
payload_101_t payload_101;
payload_103_t payload_103;
payload_104_t payload_104;
payload_105_t payload_105;
payload_106_t payload_106;
payload_107_t payload_107;

// The payload formats to be sent
typedef struct __txPld_t {
  uint8_t p101 : 1;
  uint8_t p103 : 1;
  uint8_t p104 : 1;
  uint8_t p105 : 1;
  uint8_t p106 : 1;
  uint8_t p107 : 1;
} txPld_t;
txPld_t txPld;

// Wake-Up routing
#define lpmodeWkupReasonSize 4
typedef enum __lpmodeWkupReason_t {
  lpmodeWkupReasonReport,
  lpmodeWkupReasonWind,
  lpmodeWkupReasonRainDavis,
  lpmodeWkupReasonRainPronamic
} lpmodeWkupReason_t;

// Global flag which is set by interrupt callbacks causing the wake up
lpmodeWkupReason_t wkupReason = lpmodeWkupReasonReport;
// Global flag which stores RTC wakeup reasons specifically
lpmodeWkupReason_t rtcAlarmEvent = lpmodeWkupReasonReport;
// Last transmit time
static uint32_t ostimeLastPacketTx = 0;
// Packet counter between location packets
static uint32_t pcktsSinceLastLoc = 999999; // init with large value to transmit position after reset

Wind wind(WIND_SPD_Pin, WIND_SUP_Pin, WIND_DIR_Pin); // Wind sensor handle
static uint16_t windSpdCounter; // Global speed counter for wind speed measurement
void windIncrementSpdCounter(); // Interrupt callback to increment windSpdCounter

FlashStorage(rainDavisAccFlash, uint16_t);     // Persistent memory area for rain counts
FlashStorage(rainDavisAccFlashSeal, uint16_t); // and rain accumulator
FlashStorage(rainPronamicAccFlash, uint16_t);     // Persistent memory area for rain counts
FlashStorage(rainPronamicAccFlashSeal, uint16_t); // and rain accumulator
Rain rainDavis(&rainDavisAccFlash, &rainDavisAccFlashSeal); // Rain Davis handle
Rain rainPronamic(&rainPronamicAccFlash, &rainPronamicAccFlashSeal); // Rain Pronamic handle

Bme280 bme76(0x76); // BME 0x76 handle
Bme280 bme77(0x77); // BME 0x77 handle
SHT31 sht30_44;
SHT31 sht30_45;
Watermark watermark(WM_EXC1_Pin, WM_EXC2_Pin, WM_RESISTANCE_Pin, WM_SWITCH_SUP, WM_A_Pin, WM_B_Pin);
DS18B20 ds(7); // DS18B20 handle
static uint8_t ds18b20Address[8];
Weight weight(WEIGHT_EN_Pin, WEIGHT_1_Pin, WEIGHT_2_Pin);
RtcUser rtc;
SDLog sdlog(4);

void setup() {
  // Setup UART
  DEBUG_INIT(9600);
  DEBUG(F("*************************************************"));
  DEBUG(F("*A G V O L U T I O N    M I C R O C L I M A T E *"));
  DEBUG(F("*************************************************"));

  uint8_t sigfoxState = SigFox.begin();

  DEBUG_VAL(F("SigFox State: "), sigfoxState);
  if (!sigfoxState) {
    reboot();
  }

  String version = SigFox.SigVersion();
  String ID = SigFox.ID();
  String PAC = SigFox.PAC();
  DEBUG("SigFox FW version " + version);
  DEBUG("ID  = " + ID);
  DEBUG("PAC = " + PAC);

  SigFox.end(); //Send module to standby until we need to send a message
  
  // Check Equipment
  // 1. Assume Wind by default (there is no check)
  equip.wind = 1;

  // 2. Assume Rain by default (there is no check)
  equip.rainDavis = 1;
  equip.rainPronamic = 1;

  // 3. Try to switch on GPS and see if characters are received from the GPS module
  #if !DEBUG_SERIAL // init Serial1 if not already done by debug library
  Serial1.begin(9600);
  #endif
  Serial1.read(); // clear receive buffer
  digitalWrite(GPS_SUP_Pin, HIGH);
  delay(1000); // Wait for module power up
  equip.gps = 0; // assume GPS not equipped
  if(Serial1.available() > 3) { // receive at least four characters from GPS module
    equip.gps = 1;
  }
  while(Serial1.available()) {
    Serial1.read(); // clear RX buffer
  }
  #if !DEBUG_SERIAL // de-init again to save power
  // Serial1.end(); // in fact, de-initializing the Serial1 module after initialization makes the current consumption worse --> takes 50uA more @3V3 compared to leaving the module initialized!
  #endif
  digitalWrite(GPS_SUP_Pin, LOW);

  // 4. Both BME280's -> Try to initialize
  equip.bme76 = bme76.setup();
  equip.bme77 = bme77.setup();

  // 4.2 Search for SHT30 at I2C address 0x44 and 0x45
  sht30_44.begin(0x44);
  equip.sht30_44 = sht30_44.isConnected();
  sht30_45.begin(0x45);
  equip.sht30_45 = sht30_45.isConnected();

  // 5. WaterMark equipment is decided by detecting if resistor R3 is equipped
  // WM_EXC2 floating, WM_EXC1 HIGH/LOW, WM_RESISTANCE ADC
  // Turn on WM Switch Sup, otherwise WM_RESISTANCE wont be floating towards the MUX
  pinMode(WM_EXC2_Pin, INPUT);
  pinMode(WM_EXC1_Pin, OUTPUT);
  digitalWrite(WM_EXC1_Pin, HIGH);
  pinMode(WM_SWITCH_SUP, OUTPUT); // turn on Analog Switch
  digitalWrite(WM_SWITCH_SUP, HIGH);
  analogReadResolution(12);
  delay(20); // wait until voltage level stabilization
  int wmEquipTestValHigh = analogRead(WM_RESISTANCE_Pin);
  DEBUG_VAL("val High", wmEquipTestValHigh);
  digitalWrite(WM_EXC1_Pin, LOW);
  delay(20); // wait until voltage level stabilization
  int wmEquipTestValLow = analogRead(WM_RESISTANCE_Pin);
  DEBUG_VAL("val Low", wmEquipTestValLow);
  if((wmEquipTestValHigh > 3950) && (wmEquipTestValLow < 150)) { // require high and low levels
    equip.watermark = 1;
  }
  pinMode(WM_EXC2_Pin, OUTPUT);
  digitalWrite(WM_EXC2_Pin, LOW);
  digitalWrite(WM_SWITCH_SUP, LOW); // Analog switch off

  // 6. Scan for DS18B20 and obtain address
  equip.ds18b20 = (ds.getNumberOfDevices() > 0);
  if(equip.ds18b20) {
    while(ds.selectNext()) {
      ds.getAddress(ds18b20Address);
    }
  }

  // 7. Do not Assume Weight Cells by default
  equip.weight1 = 0;
  equip.weight2 = 0;

  // TODO DETECT CLIMAVI

  // 8. Detect SD card
  String headerString = "T,RH,P,RAIN/LATEST,RAIN/ACC,WIND/DIR,WIND/SPD/AVG,WIND/SPD/STD";
  String unitString = "°C,%,hPa,COUNTS,COUNTS,°,km h-1,km h-1";
  equip.sd = sdlog.begin(headerString, headerString);

  equip.print();
  equip.mutuallyExclude();
  equip.print();

  // Determine which payload formats shall be transmitted
  txPld.p101 = equip.gps;
  txPld.p103 = equip.climavi;
  txPld.p104 = equip.weight1 | equip.weight2;
  txPld.p105 = equip.watermark | equip.sht30_44 | equip.sht30_45;
  txPld.p106 = equip.ds18b20 | equip.rainDavis | equip.rainPronamic | equip.wind;
  txPld.p107 = equip.bme76 | equip.bme77;

  // Setup GPIOs
  setupGpios();

  // Attach wakeup interrupts
  attachWakeupInterrupts();

  // Reset RTC
  rtc.reset();
}

void setupGpios() {
  // Rain Interrupt pin as input
  if(equip.rainDavis)
  {
    pinMode(RAIN_DAVIS_Pin, INPUT_PULLUP);
  }
  
  if(equip.rainPronamic)
  {
    pinMode(RAIN_PRONAMIC_Pin, INPUT_PULLUP);
  }

  // Wind Poti Supply as output, velocity counter as input
  if(equip.wind)
  {
    wind.init();
  }

  // GPS enable output and disable GPS module
  if(equip.gps)
  {
    pinMode(GPS_SUP_Pin, OUTPUT);
    digitalWrite(GPS_SUP_Pin, LOW);
  }

  if(equip.weight1 || equip.weight2)
  {
    weight.init();
  }

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void attachWakeupInterrupts() {
  // Attach wake-up interrupts during LPMODE
  if(equip.rainDavis)
  {
    LowPower.attachInterruptWakeup(RAIN_DAVIS_Pin, lpmodeInterruptRainDavis, FALLING);
  }
  
  if(equip.rainPronamic)
  {
    LowPower.attachInterruptWakeup(RAIN_PRONAMIC_Pin, lpmodeInterruptRainPronamic, FALLING);
  }
  
  LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, lpmodeInterruptRTC, CHANGE);
}

void loop() {
  // Enter low power mode by default
  lpmodeEnterStop();

  // After wakeup, act based on return value
  DEBUG_VAL(F("wkupReason"), wkupReason);
  uint32_t nowTimeSeconds;
  rtc.getTimeSeconds(&nowTimeSeconds);
  
  switch(wkupReason) {
    case lpmodeWkupReasonReport:
      if(equip.rainDavis)
      {
        attachInterrupt(RAIN_DAVIS_Pin, isrRainDavisCountNoLpmode, FALLING);  // do not ignore Rain Count Interrupts during report
      }
      
      if(equip.rainPronamic)
      {
        attachInterrupt(RAIN_PRONAMIC_Pin, isrRainPronamicCountNoLpmode, FALLING);  // do not ignore Rain Count Interrupts during report
      }
      
      DEBUG(F("REPORT"));
      report();

      if(equip.rainDavis)
      {
        detachInterrupt(RAIN_DAVIS_Pin); // The attachInterrupt is resetting the LowPower wakeupInterrupt functionality
        LowPower.attachInterruptWakeup(RAIN_DAVIS_Pin, lpmodeInterruptRainDavis, FALLING);
      }

      if(equip.rainPronamic)
      {
        detachInterrupt(RAIN_PRONAMIC_Pin); // The attachInterrupt is resetting the LowPower wakeupInterrupt functionality
        LowPower.attachInterruptWakeup(RAIN_PRONAMIC_Pin, lpmodeInterruptRainPronamic, FALLING);
      }
    break;
    
    case lpmodeWkupReasonWind:
      if(equip.rainDavis)
      {
        attachInterrupt(RAIN_DAVIS_Pin, isrRainDavisCountNoLpmode, FALLING);  // do not ignore Rain Count Interrupts during wind sampling
      }
      
      if(equip.rainPronamic)
      {
        attachInterrupt(RAIN_PRONAMIC_Pin, isrRainPronamicCountNoLpmode, FALLING);  // do not ignore Rain Count Interrupts during wind sampling
      }
    
      if(equip.wind)
      {
        DEBUG(F("WIND"));
        // Take measurement
        windSpdCounter = 0;
        attachInterrupt(WIND_SPD_Pin, windIncrementSpdCount, FALLING);
        delay(3000); // active wait
        detachInterrupt(WIND_SPD_Pin);
      
        /* Convert average pulse frequency to wind speed
         * v[km/h] = f[Hz] * 2.25 * 1.61
         * Explanation:
         * 2.25: Davis Anemometer pulse generator property
         * 1.61 mph -> km/h
         * Sampling time: 3 seconds
         */
        float windSpdSmpl = windSpdCounter / 3.0 * 2.25 * 1.61;
        wind.addSpdSample(windSpdSmpl);
        DEBUG_VAL(F("       wind.sample"), wind.sample-1);
        DEBUG_VAL(F("wind.spdSamples[i]"), wind.spdSamples[wind.sample-1]);
        DEBUG("");
      }

      if(equip.rainDavis)
      {
        detachInterrupt(RAIN_DAVIS_Pin); // The attachInterrupt is resetting the LowPower wakeupInterrupt functionality
        LowPower.attachInterruptWakeup(RAIN_DAVIS_Pin, lpmodeInterruptRainDavis, FALLING);
      }
      
      if(equip.rainPronamic)
      {
        detachInterrupt(RAIN_PRONAMIC_Pin); // The attachInterrupt is resetting the LowPower wakeupInterrupt functionality
        LowPower.attachInterruptWakeup(RAIN_PRONAMIC_Pin, lpmodeInterruptRainPronamic, FALLING);
      }
    break;
    
    case lpmodeWkupReasonRainDavis:
      {
        if(equip.rainDavis)
        {
          DEBUG(F("RAIN DAVIS"));
          // Increment rain counter by one
          rainDavis.increment(nowTimeSeconds);
          
          DEBUG_VAL(F("    RAIN DAVIS current"), rainDavis.getCounts());
          DEBUG_VAL(F("RAIN DAVIS accumulated"), rainDavis.getAccCounts());
        }
      }
    break;
    case lpmodeWkupReasonRainPronamic:
      {
        if(equip.rainPronamic)
        {
          DEBUG(F("RAIN PRONAMIC"));
          // Increment rain counter by one
          rainPronamic.increment(nowTimeSeconds);
          
          DEBUG_VAL(F("    RAIN PRONAMIC current"), rainPronamic.getCounts());
          DEBUG_VAL(F("RAIN PRONAMIC accumulated"), rainPronamic.getAccCounts());
        }
      }
    break;

    default:
    // do nothing
    break;
  }
}

/* Transmit a data packet.
 *
 */
void report() {
  // Checking whether we are supposed to transmit a location package
  // Send packet once a day
  txPld.p101 = 0; // let's assume not to transmit GPS data, because at least something will go wrong (GPS Fixture, Module comms, etc.). Set txPld.p101 = 1 only if position data is available.
  #if (GPS_LOCATION_PACKET_INTERVAL_HOURS < 12) || (GPS_LOCATION_PACKET_INTERVAL_HOURS > 720)
    #error "GPS Location Packet Interval must be within [12,720] hours"
  #endif
  if(equip.gps && ((PACKET_INTERVAL_SECONDS * pcktsSinceLastLoc) > GPS_LOCATION_PACKET_INTERVAL_HOURS*3600)) { // transmit a position packet?
    pcktsSinceLastLoc = 0;
    uint32_t tBeg, tNow;
    rtc.getTimeSeconds(&tBeg);
    rtc.getTimeSeconds(&tNow);

    // Enable GPS
    digitalWrite(GPS_SUP_Pin, HIGH);
    #if !DEBUG_SERIAL // Init Serial1 module if not otherwize initialized by the Debug Library
    Serial1.begin(9600);
    #endif
    String line;
    float lat, lon;
    uint8_t hh, mm, ss; // UTC time via GPS module
    String latOrient, lonOrient, latStr, lonStr;
    DEBUG(F("Waiting for GPS Fixture."));
    #if (GPS_FIXTURE_TIMEOUT_SECONDS < 1) || (GPS_FIXTURE_TIMEOUT_SECONDS > 180)
    #error "GPS Fixture Timeout must be within [1,180] seconds."
    #endif
    while((tNow - tBeg) < GPS_FIXTURE_TIMEOUT_SECONDS) { // wait for a fixture
      rtc.getTimeSeconds(&tNow);
      line = Serial1.readStringUntil('\n'); // read a single line
      /* The NMEA Global Positioning System Fix Data (GGA) format:
       * $GPGGA,HHMMSS.ss,BBBB.BBBB,b,LLLLL.LLLL,l,Q,NN,D.D,H.H,h,G.G,g,A.A,RRRR*PP or
       * $GPRMC,HHMMSS,A,BBBB.BBBB,b,LLLLL.LLLL,l,GG.G,RR.R,DDMMYY,M.M,m,F*PP           --> used on our u.blox module
       * $GNRMC,HHMMSS.000,A,BBBB.BBBBB,N,LLLLL.LLLLL,E,G.GG,R.RR,DDMMYY,,,A,V*05       --> used on our SIM28 module
       * $GNRMC,225445.000,A,4934.44872,N,01059.09229,E,0.00,0.00,051221,,,A,V*05       --> used on our SIM28 module
       * -> Use substrings due to fixed-width string format
       */

      // Search for $GPRMC and $GNRMC command
      bool indicateGPRMC = line.substring(0, 6).equals("$GPRMC");
      bool indicateGNRMC = line.substring(0, 6).equals("$GNRMC");
      if((!indicateGPRMC) && (!indicateGNRMC) ) continue; // skip
      if(!line.substring(18, 19).equals("A")) continue; // character at pos. 18 has to be A=OK (V=WARNING). Otherwise -> no position fix
      DEBUG(line); // forward line to Debug console

      // Extract UTC time
      hh = line.substring(7, 9).toInt();
      mm = line.substring(9, 11).toInt();
      ss = line.substring(11, 13).toInt();
      #if DEBUG_SERIAL
      Serial1.print("UTC Time: "); Serial1.print(hh, DEC); Serial1.print(":"); Serial1.print(mm, DEC); Serial1.print(":"); Serial1.println(ss, DEC);
      #endif
      
      // Extract position information
      if(indicateGPRMC) // used on u.blox module
      {
        latStr = line.substring(20,29);
        latOrient = line.substring(30, 31);
      }
      else // SIM28 module
      {
        latStr = line.substring(20,30);
        latOrient = line.substring(31, 32);
      }
      /* Convert the latitude ddmm.mmmm format to decimal degrees. 60 angular minutes = 1 degree */
      lat = latStr.substring(0,2).toFloat() + latStr.substring(2).toFloat() / 60.0f;
      if(indicateGPRMC) // used on u.blox module
      {
        lonStr = line.substring(32, 42);
        lonOrient = line.substring(43, 44);
      }
      else // SIM28 module
      {
        lonStr = line.substring(33, 44);
        lonOrient = line.substring(45, 46);
      }
      /* Convert the longitude dddmm.mmmm format to decimal degrees. 60 angular minutes = 1 degree */
      lon = lonStr.substring(0,3).toFloat() + lonStr.substring(3).toFloat() / 60.0f;

      // Check boundaries
      if((lat >= 0.0f) && (lat <= 90.0f) && (lon >= 0.0f) && (lon <= 180.0f)) {
        // Convert orientation characters to sign (Lat: N+, S-, Lon: E+, W-)
        if(latOrient.equals("S")) lat *= (-1.0f);
        if(lonOrient.equals("W")) lon *= (-1.0f);
        payload_101.latitude = lat;
        payload_101.longitude = lon;
        txPld.p101 = 1; // Transmit Position Payload!
        DEBUG_VAL_ARG("Latitude", lat, 6);
        DEBUG_VAL_ARG("Longitude", lon, 6);
        break; // Leave after first position fix
      }
    }
    if((tNow - tBeg) >= 90) {
      DEBUG(F("GPS FIXTURE TIMEOUT"));
    }
    #if !DEBUG_SERIAL
    // Serial1.end(); // in fact, de-initializing the Serial1 module after initialization makes the current consumption worse --> takes 50uA more @3V3 compared to leaving the module initialized!
    #endif
    digitalWrite(GPS_SUP_Pin, LOW); // Disable module
  }

  DEBUG(F("SigFox Module Boot"));

  // Start the module
  SigFox.begin();
  //#if debug_LED // SigFox Module not really working without Debug LED ?
  SigFox.debug();
  //#endif
  
  // Submit sensor read tasks
  if(equip.bme76)
  {
    DEBUG(F("Trigger BME280 0x76"));
    bme76.handle.writeCTRLMeas(); // Trigger BME280 measurement by writing mode parameter (set to force) again
  }

  if(equip.bme77)
  {
    DEBUG(F("Trigger BME280 0x77"));
    bme77.handle.writeCTRLMeas(); // Trigger BME280 measurement by writing mode parameter (set to force) again
  }

  // Turn on voltage supply for weight cell
  if(equip.weight1 || equip.weight2)
  {
    weight.enable();
  }

  // Take soil matrix potential measurements
  if(equip.watermark)
  {
    watermark.activate();
    for(uint8_t ch = 0; ch < 4; ch++) {
      payload_105.matrixPotential[ch] = watermark.matrixPotentialCbars(ch);
    }
    watermark.deactivate();
  }

  // Read DS18B20 temperature sensor
  if(equip.ds18b20)
  {
    if(ds.select(ds18b20Address)) { //check if device can be found on 1-Wire bus
      int16_t temperature = round(ds.getTempC() * 100);
      payload_106.temperature = temperature;
    }
  }

  // Wait (BME280 measurement, WEIGHT supply stabilization, SigFox boot (at least 100ms))
  delay(100);

  // Read wind poti voltage + sum up speed measurements
  if(equip.wind)
  {
    DEBUG(F("Read WIND_DIR"));
    payload_106.windDirAndError = (uint8_t)(round(wind.direction() / 4.0)) << 1; // in 4° steps
    payload_106.windSpdAvg = round(wind.spdAvg());
    payload_106.windSpdStd = round(wind.spdStd());
    // Reset wind sample counter
    wind.sample = 0;
  }

  // Read weight cell voltage
  if(equip.weight1)
  {
    // save to payload
    payload_104.weight1 = weight.getGrams(0);
  }

  if(equip.weight2)
  {
    // save to payload
    payload_104.weight2 = weight.getGrams(1);
  }

  if(equip.weight1 || equip.weight2)
  {
    weight.disable();
  }

  if(equip.bme76)
  {
    payload_107.bme76_temperature = round(bme76.handle.readTempC() * 100); // in 0.01 °C
    payload_107.bme76_humidity = round(bme76.handle.readHumidity()); // in percent
    payload_107.bme76_pressure = round(bme76.handle.readPressure() * 10); // in 0.1 hPa
    payload_107.errors &= ~0b00111000; // clear BME 0x76 T/RH/P error flags
  }
  else { // set error flag
    payload_107.errors |= 0b00111000; // set BME 0x76 T/RH/P error flags
  }

  if(equip.bme77) {
    payload_107.bme77_temperature = round(bme77.handle.readTempC() * 100); // in 0.01 °C
    payload_107.bme77_humidity = round(bme77.handle.readHumidity()); // in percent
    payload_107.bme77_pressure = round(bme77.handle.readPressure() * 10); // in 0.1 hPa
    payload_107.errors &= ~0b00000111; // clear BME 0x77 T/RH/P error flags
  }
  else { // set error flag
    payload_107.errors |= 0b00000111; // set BME 0x77 T/RH/P error flags
  }

  if(equip.sht30_44)
  {
    if(sht30_44.isConnected())
    {
      sht30_44.getError(); // clear pending error flags
      sht30_44.read();
      if(sht30_44.getError() != 0)
      {
        // Could not read T/RH -> Set payload error flags
      }
      payload_105.sht3x_0x44_temperature = round(sht30_44.getTemperature() * 100); // in 0.01 °C
      payload_105.sht3x_0x44_humidity = round(sht30_44.getHumidity()); // in percent
      payload_105.errors &= ~ ( (1 << ERR_SHT3x_44_TMP_105) | (1 << ERR_SHT3x_44_HUM_105) ); // clear error flags
    }
    else 
    {
      payload_105.errors |= (1 << ERR_SHT3x_44_TMP_105) | (1 << ERR_SHT3x_44_HUM_105); // set error flags
    }
  }

  if(equip.sht30_45)
  {
    if(sht30_45.isConnected())
    {
      payload_105.sht3x_0x45_temperature = round(sht30_45.getTemperature() * 100); // in 0.01 °C
      payload_105.sht3x_0x45_humidity = round(sht30_45.getHumidity()); // in percent
      payload_105.errors &= ~ ( (1 << ERR_SHT3x_45_TMP_105) | (1 << ERR_SHT3x_45_HUM_105) ); // clear error flags
    }
    else 
    {
      payload_105.errors |= (1 << ERR_SHT3x_45_TMP_105) | (1 << ERR_SHT3x_45_HUM_105); // set error flags
    }
  }

  if(equip.rainDavis)
  {
    payload_106.rainCountDavis = rainDavis.getCounts();
    payload_106.rainCountDavisAcc = rainDavis.getAccCounts();
    rainDavis.resetCounts();
  }

  if(equip.rainPronamic)
  {
    payload_106.rainCountPronamic = rainPronamic.getCounts();
    payload_106.rainCountPronamicAcc = rainPronamic.getAccCounts();
    rainPronamic.resetCounts();
  }

  DEBUG("");

  if(txPld.p101) {
    DEBUG(F("Payload 101:"));
    DEBUG_VAL(F("       Latitude"), payload_101.latitude);
    DEBUG_VAL(F("      Longitude"), payload_101.longitude);
    DEBUG_VAL(F("        DS18B20"), payload_101.temperature);
    DEBUG();
  }

  if(txPld.p103) {
    DEBUG(F("Payload 103:"));
    DEBUG_VAL(F("        FDC CH0"), payload_103.fdcCh[0]);
    DEBUG_VAL(F("        FDC CH1"), payload_103.fdcCh[1]);
    DEBUG_VAL(F("        FDC CH2"), payload_103.fdcCh[2]);
    DEBUG_VAL(F("        FDC CH3"), payload_103.fdcCh[3]);
    DEBUG();
  }

  if(txPld.p104) {
    DEBUG(F("Payload 104:"));
    DEBUG_VAL(F("       Weight 1"), payload_104.weight1);
    DEBUG_VAL(F("       Weight 2"), payload_104.weight2);
    DEBUG_VAL(F("    Temperature"), payload_104.temperature);
    DEBUG();
  }

  if(txPld.p105) {
    DEBUG(F("Payload 105:"));
    DEBUG_VAL(F("    Watermark 0"), payload_105.matrixPotential[0]);
    DEBUG_VAL(F("    Watermark 1"), payload_105.matrixPotential[1]);
    DEBUG_VAL(F("    Watermark 2"), payload_105.matrixPotential[2]);
    DEBUG_VAL(F("    Watermark 3"), payload_105.matrixPotential[3]);
    DEBUG_VAL(F(" SHT 0x44 Temp."), payload_105.sht3x_0x44_temperature);
    DEBUG_VAL(F(" SHT 0x44  Hum."), payload_105.sht3x_0x44_humidity);
    DEBUG_VAL(F(" SHT 0x45 Temp."), payload_105.sht3x_0x45_temperature);
    DEBUG_VAL(F(" SHT 0x45  Hum."), payload_105.sht3x_0x45_humidity);
    DEBUG_VAL(F("         Errors"), payload_105.errors);
    DEBUG();
  }

  if(txPld.p106) {
    DEBUG(F("Payload 106:"));
    DEBUG_VAL(F("    Temperature"), payload_106.temperature);
    DEBUG_VAL(F("   rainCountDav"), payload_106.rainCountDavis);
    DEBUG_VAL(F("rainCountDavAcc"), payload_106.rainCountDavisAcc);
    DEBUG_VAL(F("   rainCountPro"), payload_106.rainCountPronamic);
    DEBUG_VAL(F("rainCountProAcc"), payload_106.rainCountPronamicAcc);
    DEBUG_VAL(F("windDirAndError"), payload_106.windDirAndError);
    DEBUG_VAL(F("     windSpdAvg"), payload_106.windSpdAvg);
    DEBUG_VAL(F("     windSpdStd"), payload_106.windSpdStd);
    DEBUG();
  }

  if(txPld.p107) {
    DEBUG(F("Payload 107:"));
    DEBUG_VAL(F(" 76 Temperature"), payload_107.bme76_temperature);
    DEBUG_VAL(F(" 76    Humidity"), payload_107.bme76_humidity);
    DEBUG_VAL(F(" 76    Pressure"), payload_107.bme76_pressure);
    DEBUG_VAL(F(" 77 Temperature"), payload_107.bme77_temperature);
    DEBUG_VAL(F(" 77    Humidity"), payload_107.bme77_humidity);
    DEBUG_VAL(F(" 77    Pressure"), payload_107.bme77_pressure);
    DEBUG_VAL(F("         Errors"), payload_107.errors);
    DEBUG();
  }

  // Save data to SD card
  if(equip.sd)
  {
    String testString = "Test";
    sdlog.write(testString);
  }

  // Print raw hex payload message
//  Serial.println("Payload 100");
//  for(uint8_t i = 0; i < 12; i++) {
//    Serial.print(*((uint8_t*)&payload+i), HEX);
//    Serial.print(F(" "));
//  }
//  Serial.println();
//
//  Serial.println("Payload 104");
//  for(uint8_t i = 0; i < 12; i++) {
//    Serial.print(*((uint8_t*)&payload+i), HEX);
//    Serial.print(F(" "));
//  }
//  Serial.println();

  // Clears all pending interrupts
  SigFox.status();
  delay(1);

  #if !DEBUG_SKIP_TX
  int ret;

  // Transmit Sigfox Messages
  if(txPld.p101) {
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&payload_101, PAYLOAD_LENGTH_101);
    // May take 10 seconds
    ret = SigFox.endPacket();
    DEBUG_VAL(F("Sigfox status (PLD 101) (0=OK)"), ret);
  }

  if(txPld.p103) {
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&payload_103, PAYLOAD_LENGTH_103);
    // May take 10 seconds
    ret = SigFox.endPacket();
    DEBUG_VAL(F("Sigfox status (PLD 103) (0=OK)"), ret);
  }

  if(txPld.p104) {
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&payload_104, PAYLOAD_LENGTH_104);
    // May take 10 seconds
    ret = SigFox.endPacket();
    DEBUG_VAL(F("Sigfox status (PLD 104) (0=OK)"), ret);
  }

  if(txPld.p105) {
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&payload_105, PAYLOAD_LENGTH_105);
    // May take 10 seconds
    ret = SigFox.endPacket();
    DEBUG_VAL(F("Sigfox status (PLD 105) (0=OK)"), ret);
  }

  if(txPld.p106) {
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&payload_106, PAYLOAD_LENGTH_106);
    // May take 10 seconds
    ret = SigFox.endPacket();
    DEBUG_VAL(F("Sigfox status (PLD 106) (0=OK)"), ret);
  }

  if(txPld.p107) {
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&payload_107, PAYLOAD_LENGTH_107);
    // May take 10 seconds
    ret = SigFox.endPacket();
    DEBUG_VAL(F("Sigfox status (PLD 107) (0=OK)"), ret);
  }
  #endif

  SigFox.end();

  // Set internal time to zero after each data packet
  rtc.reset();
  // Rest RAIN count time stamp together with RTC. Otherwise the RTC stamp will never be larger than last rain count stamp.
  rainDavis.lastCountOstime = 0;
  rainPronamic.lastCountOstime = 0;
  rtc.getTimeSeconds(&ostimeLastPacketTx);
  pcktsSinceLastLoc++;
}

/*  Call this method to enter low power state.
 *  RTC wake up is configured automatically.
 */
void lpmodeEnterStop() {
  DEBUG();
  DEBUG(F("Entering LPMODE"));
  
  uint32_t nowTimeSeconds;
  rtc.getTimeSeconds(&nowTimeSeconds);
  DEBUG_VAL(F("       RTC Time"), (int)nowTimeSeconds);
  
  // Switch off builtin LED to signal low power mode
  digitalWrite(LED_BUILTIN, LOW);
  // calculate time delta until next wakeup alarm
  int delayUntilNextWakeup = lpmodeScheduleWakeup();
  delayUntilNextWakeup *= 1000; // from milliseconds to seconds
  #if debug_NO_LPMODE
  delay(delayUntilNextWakeup);
  #else
  // enter low power mode and schedule wakeup alert
  LowPower.sleep(delayUntilNextWakeup);
  #endif

  #if debug_LED
  digitalWrite(LED_BUILTIN, HIGH);
  #endif
  
  DEBUG(F("Waking up from LPMODE"));
  rtc.getTimeSeconds(&nowTimeSeconds);
  DEBUG_VAL(F("       RTC Time"), nowTimeSeconds);
  DEBUG();
  return;
}

/* Schedule the next wakeup alarm. */
int lpmodeScheduleWakeup() {
  // Specify a mapping from list indizes (0..N) -> lpmodeWkupReason ENUM
  lpmodeWkupReason_t mp[lpmodeWkupReasonSize];
  mp[lpmodeWkupReasonReport] = lpmodeWkupReasonReport;
  mp[lpmodeWkupReasonWind] = lpmodeWkupReasonWind;
  mp[lpmodeWkupReasonRainDavis] = lpmodeWkupReasonRainDavis;
  mp[lpmodeWkupReasonRainPronamic] = lpmodeWkupReasonRainPronamic;
  
  // Get current time. RTC counts seconds but is reset after each packet cycle.
  uint32_t nowTimeSeconds;
  rtc.getTimeSeconds(&nowTimeSeconds);

  // The package interval in seconds
  #if (PACKET_INTERVAL_SECONDS < 5) || (PACKET_INTERVAL_SECONDS > 43200)
    #error "Packet interval must lie within [5,720*60] seconds"
  #endif
  uint16_t packageIntervalSeconds = PACKET_INTERVAL_SECONDS;

  /* Initialize schedule. Set to maximum possible time stamp in the future. */
  uint32_t next[lpmodeWkupReasonSize];
  for(int i = 0; i < lpmodeWkupReasonSize; i++) {
    next[i] = 0xffffffff;
  }

  /* Schedule REPORT */
  next[lpmodeWkupReasonReport] = ((nowTimeSeconds / packageIntervalSeconds) + 1) * packageIntervalSeconds;
  // If - for some reason - we missed the last packet tx (overdue), reschedule immediately
  if ( (nowTimeSeconds - ostimeLastPacketTx) >= packageIntervalSeconds) {
    next[lpmodeWkupReasonReport] = nowTimeSeconds + 1;
  }
  DEBUG_VAL(F("    Next REPORT"), next[lpmodeWkupReasonReport]);

  /* Schedule WIND */
  if(equip.wind) {
    #if (WIND_SAMPLING_INTERVAL_SECONDS < 10) || (WIND_SAMPLING_INTERVAL_SECONDS > 3600)
      #error "Wind sampling interval must be within [10, 3600] seconds."
    #endif
    next[lpmodeWkupReasonWind] = ((nowTimeSeconds / WIND_SAMPLING_INTERVAL_SECONDS) + 1) * WIND_SAMPLING_INTERVAL_SECONDS;
    // Safety feature: Only schedule new wind measurements until next package is due
    if(next[lpmodeWkupReasonWind] >= (next[lpmodeWkupReasonReport] - 1)) {
      next[lpmodeWkupReasonWind] = 0xffffffff;
    }
    DEBUG_VAL(F("Next WIND"), next[lpmodeWkupReasonWind]);
  }

  uint8_t closestAlarm = 0;

  // Find the closest alarm
  for(int i = 1; i < lpmodeWkupReasonSize; i++) {
    if(next[i] < next[closestAlarm]) {
      closestAlarm = i;
    }
  }

  /* Register RTC event */
  rtcAlarmEvent = mp[closestAlarm];
  // DEBUG(F("rtcAlarmEvent="));
  // DEBUG(rtcAlarmEvent);
  
  // Get time delta until next alarm
  return (int)(next[closestAlarm] - nowTimeSeconds);
}

/* Wake up from Real Time Clock Alarm.
 * Set global wakeup reason flag to the identical value
 * as set before going to sleep on RTC wakeup (rtcAlarmEvent).
 */
void lpmodeInterruptRTC() {
  // DEBUG(F("wkupReason="));
  // DEBUG(wkupReason);
  wkupReason = rtcAlarmEvent;
  // DEBUG(F("wkupReason="));
  // DEBUG(wkupReason);
}

/*  Wake up from  Rain Gauge tipping spoon interrupt event.
 *  Set global wakeup reason flag.
 */
void lpmodeInterruptRainDavis() {
  wkupReason = lpmodeWkupReasonRainDavis;
}

void lpmodeInterruptRainPronamic() {
  wkupReason = lpmodeWkupReasonRainPronamic;
}

void isrRainDavisCountNoLpmode() {
  uint32_t nowTimeSeconds;
  rtc.getTimeSeconds(&nowTimeSeconds);
  DEBUG(F("RAIN DAVIS NO LPMODE"));
  rainDavis.increment(nowTimeSeconds);
  DEBUG_VAL(F("    RAIN DAVIS current: "), rainDavis.getCounts());
  DEBUG_VAL(F("RAIN DAVIS accumulated: "), rainDavis.getAccCounts());
}

void isrRainPronamicCountNoLpmode() {
  uint32_t nowTimeSeconds;
  rtc.getTimeSeconds(&nowTimeSeconds);
  DEBUG(F("RAIN PRONAMIC NO LPMODE"));
  rainPronamic.increment(nowTimeSeconds);
  DEBUG_VAL(F("    RAIN PRONAMIC current: "), rainPronamic.getCounts());
  DEBUG_VAL(F("RAIN PRONAMIC accumulated: "), rainPronamic.getAccCounts());
}

void windIncrementSpdCount() {
  windSpdCounter++;
}

/* Perform a reboot */
void reboot() {
  NVIC_SystemReset();
  delay(100);
}
