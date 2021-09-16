/*************
 * TODO
 * 1. Leaf wetness detect module equipment
 * 2. Record BME76 error flag
 * 3. If BME76 looses initialization (power cut) -> Detect and Re-Init ? Which registers need to be written?
 * 
 * Energy Consumption
 * 1. Serial1 usage makes approx. 50uA in Stop Mode
 * 2. 19uA in Stop Mode without any shields
 * 3. 19uA in Stop Mode + SD Proto Shield without SD Card
 * 4. 740uA in Stop Mode when SD Card is suddenly inserted or not properly initialized
 * 5. 93uA in Stop Mode + SD Proto Shield + SD Card properly initialized (SPI + SD Controller shutdown current ca. 74uA (it is not the SD shutdown current!)) ==> SD library costs 70uA in Stop Mode
 * 6. Wire.h library + Wire.begin (I2C master) -> no additional shutdown current (no slaves plugged)
 * 7. RTCZero approx 2uA in Stop Mode
 * 8. SigFox 16mA in active mode, additional 20uA in Stop Mode (SigFox.end())
 * 9. Input pullups do not really make a difference
 * 10. External Serial1 makes around 70-100uA in Stop Mode and can't simply be 'ended' by Serial.end() (-> requires additional register modifications to restore lowest stop current)
 * 11. USB Serial is not drawing any stop mode current!
 * 12. Include Rain module: +15-20uA (probably the pull-up resistor)
 * 13. Include wind module: No additional cost during stop mode, but speed sample cycles more frequently than packet cycle
 * 14. BME280 not initialized or initialized (shutdown): 10uA
 * 
 * Final 3V3 current BME76, Wind, Rain, No SD): 64uA. With SD Card: 130uA -> Half-life!
 * 
 * Energy Consumption Measurement Protocol 17.7.2021. Measured mA at +3V3. Equipped with: GPS, DS18B20, BME280 0x76, BME 0x77, 1x Watermark + Multiplexer, DEBUG_SERIAL = 0 or 1 ( does not make a difference! )
 * 1. GPS Looking For Fix 50mA @ 90s = 4500mAs
 * 2. Stop Mode All mentioned sensors equipped: 190uA = 100uA (uC listening for interrupts + RTC) + 90uA (peripherals)
 * 3. TX Sigfox: 60mA (approx. 10s per data packet) = 600mAs per packet
 * 4. 
 * 5. Charge per day (24 packets, 1x position): 24 * 2 * 600mAs (Sigofx) + 4500mAs (GPS) + 600mAs (Sigfox Position) + 16300mAs (Stop Mode) = 28800 Sigfox + 5100 Position + 16300 (Stop) = 46mWh
 * 6. Charge LiIon: 3.8V * 2200mAh = 8360mWh ---> 182 days = half a year
 * 7. 
 * 8. 
 */


/* Pin definitions and equipment */
#include "Inc/hardware.h"     // Contains Pinout
#include "equipment.h"        // Stores the modules equipped
#include "Inc/commission/commission.h"  // Configuration of packet interval, ...

/* Payload definition */
#include "Inc/payload_format/payload_format_100.h"
#include "Inc/payload_format/payload_format_101.h"
#include "Inc/payload_format/payload_format_102.h"
#include "Inc/payload_format/payload_format_103.h"
#include "Inc/payload_format/payload_format_104.h"

/* Modules */
#include "wind.h"             // Wind sensor
#include "rain.h"             // Rain counter
#include "bme280.h"           //
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
payload_100_t payload_100;
payload_101_t payload_101;
payload_102_t payload_102;
payload_103_t payload_103;
payload_104_t payload_104;

// The payload formats to be sent
typedef struct __txPld_t {
  uint8_t p100 : 1;
  uint8_t p101 : 1;
  uint8_t p102 : 1;
  uint8_t p103 : 1;
  uint8_t p104 : 1;
} txPld_t;
txPld_t txPld;

// Wake-Up routing
#define lpmodeWkupReasonSize 4
typedef enum __lpmodeWkupReason_t {
  lpmodeWkupReasonReport,
  lpmodeWkupReasonWind,
  lpmodeWkupReasonRain,
  lpmodeWkupReasonButton
} lpmodeWkupReason_t;

// Global flag which is set by interrupt callbacks causing the wake up
lpmodeWkupReason_t wkupReason = lpmodeWkupReasonReport;
// Global flag which stores RTC wakeup reasons specifically
lpmodeWkupReason_t rtcAlarmEvent = lpmodeWkupReasonReport;
// Last transmit time
static uint32_t ostimeLastPacketTx = 0;
// Packet counter between location packets
static uint32_t pcktsSinceLastLoc = 50; // init with large value to transmit position after reset

Wind wind(WIND_SPD_Pin, WIND_SUP_Pin, WIND_DIR_Pin); // Wind sensor handle
static uint16_t windSpdCounter; // Global speed counter for wind speed measurement
void windIncrementSpdCounter(); // Interrupt callback to increment windSpdCounter
FlashStorage(rainAccFlash, uint16_t);     // Persistent memory area for rain counts
FlashStorage(rainAccFlashSeal, uint16_t); // and rain accumulator
Rain rain(&rainAccFlash, &rainAccFlashSeal); // Rain handle
Bme280 bme76(0x76); // BME 0x76 handle
Bme280 bme77(0x77); // BME 0x77 handle
Watermark watermark(WM_EXC1_Pin, WM_EXC2_Pin, WM_RESISTANCE_Pin, WM_SWITCH_SUP, WM_A_Pin, WM_B_Pin);
DS18B20 ds(7); // DS18B20 handle
static uint8_t ds18b20Address[8];
Weight weight(WEIGHT_EN_Pin, WEIGHT_1_Pin, WEIGHT_2_Pin);
RtcUser rtc;

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
  // 1. Assume Wind by default
  equip.wind = 1;

  // 2. Assume Rain by default
  equip.rain = 1;

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
  Serial1.end();
  #endif
  digitalWrite(GPS_SUP_Pin, LOW);

  // 4. Both BME280's -> Try to initialize
  equip.bme76 = bme76.setup();
  equip.bme77 = bme77.setup();

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
  digitalWrite(WM_EXC1_Pin, LOW);
  delay(20); // wait until voltage level stabilization
  int wmEquipTestValLow = analogRead(WM_RESISTANCE_Pin);
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

  // 7. Assume Weight Cells by default
  equip.weight1 = 1;
  equip.weight2 = 1;

  // TODO DETECT GPS or LEAF-WET (at the moment -> neglect leaf wet)
  // TODO DETECT CLIMAVI

  equip.print();
  equip.mutuallyExclude();
  equip.print();

  // Determine which payload formats shall be transmitted
  txPld.p100 = equip.bme76 | equip.rain | equip.wind;
  txPld.p101 = equip.gps;
  txPld.p102 = equip.watermark | equip.leafWet | equip.bme77;
  txPld.p103 = equip.climavi;
  txPld.p104 = equip.weight1 | equip.weight2 | equip.ds18b20;

  // Setup GPIOs
  setupGpios();

  // Attach wakeup interrupts
  attachWakeupInterrupts();

  // Reset RTC
  rtc.reset();
}

void setupGpios() {
  // Rain Interrupt pin as input
  if(equip.rain) pinMode(RAIN_Pin, INPUT_PULLUP);

  // Wind Poti Supply as output, velocity counter as input
  if(equip.wind) wind.init();

  // GPS enable output and disable GPS module
  if(equip.gps) {
    pinMode(GPS_SUP_Pin, OUTPUT);
    digitalWrite(GPS_SUP_Pin, LOW);
  }

  if(equip.leafWet) {
    pinMode(LEAFWET_SUP_Pin, OUTPUT);
    digitalWrite(LEAFWET_SUP_Pin, LOW);
  }

  if(equip.weight1 || equip.weight2) weight.init();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(BUTTON_Pin, INPUT_PULLUP);
}

void attachWakeupInterrupts() {
  // Attach wake-up interrupts during LPMODE
  if(equip.rain) {
    LowPower.attachInterruptWakeup(RAIN_Pin, lpmodeInterruptRain, FALLING);
  }
  if(equip.weight1 || equip.weight2) {
    LowPower.attachInterruptWakeup(BUTTON_Pin, lpmodeInterruptButton, FALLING);
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
      if(equip.rain) attachInterrupt(RAIN_Pin, isrRainCountNoLpmode, FALLING);  // do not ignore Rain Count Interrupts during report
      
      DEBUG(F("REPORT"));
      report();

      if(equip.rain) {
        detachInterrupt(RAIN_Pin); // The attachInterrupt is resetting the LowPower wakeupInterrupt functionality
        LowPower.attachInterruptWakeup(RAIN_Pin, lpmodeInterruptRain, FALLING);
      }
    break;
    
    case lpmodeWkupReasonWind:
      if(equip.rain) attachInterrupt(RAIN_Pin, isrRainCountNoLpmode, FALLING);  // do not ignore Rain Count Interrupts during wind sampling
    
      if(equip.wind) {
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

      if(equip.rain) {
        detachInterrupt(RAIN_Pin); // The attachInterrupt is resetting the LowPower wakeupInterrupt functionality
        LowPower.attachInterruptWakeup(RAIN_Pin, lpmodeInterruptRain, FALLING);
      }
    break;
    
    case lpmodeWkupReasonRain:
      {
        if(equip.rain) {
          DEBUG(F("RAIN"));
          // Increment rain counter by one
          rain.increment(nowTimeSeconds);
          
          DEBUG_VAL(F("    RAIN current"), rain.getCounts());
          DEBUG_VAL(F("RAIN accumulated"), rain.getAccCounts());
        }
      }
    break;

    case lpmodeWkupReasonButton:
    if(equip.weight1 || equip.weight2) weight.tara();
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
  if(equip.gps && ((PACKET_INTERVAL_INITIAL * pcktsSinceLastLoc) > 48*60)) { // every two days position packet
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
    while((tNow - tBeg) < GPS_FIXTURE_TIMEOUT) { // wait 90s for a fixture
      rtc.getTimeSeconds(&tNow);
      line = Serial1.readStringUntil('\n'); // read a single line
      /* The NMEA Global Positioning System Fix Data (GGA) format:
       * $GPGGA,HHMMSS.ss,BBBB.BBBB,b,LLLLL.LLLL,l,Q,NN,D.D,H.H,h,G.G,g,A.A,RRRR*PP or
       * $GPRMC,HHMMSS,A,BBBB.BBBB,b,LLLLL.LLLL,l,GG.G,RR.R,DDMMYY,M.M,m,F*PP
       * -> Use substrings due to fixed-width string format
       */

      // Search for $GPRMC command
      if(!line.substring(0, 6).equals("$GPRMC")) continue; // skip
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
      latStr = line.substring(20,29);
      latOrient = line.substring(30, 31);
      /* Convert the latitude ddmm.mmmm format to decimal degrees. 60 angular minutes = 1 degree */
      lat = latStr.substring(0,2).toFloat() + latStr.substring(2).toFloat() / 60.0f;

      lonStr = line.substring(32, 42);
      lonOrient = line.substring(43, 44);
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
    Serial1.end();
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
  if(equip.bme76) {
    DEBUG(F("Trigger BME280 0x76"));
    bme76.handle.writeCTRLMeas(); // Trigger BME280 measurement by writing mode parameter (set to force) again
  }

  if(equip.bme77) {
    DEBUG(F("Trigger BME280 0x77"));
    bme77.handle.writeCTRLMeas(); // Trigger BME280 measurement by writing mode parameter (set to force) again
  }

  // Turn on voltage supply for weight cell
  if(equip.weight1 || equip.weight2) weight.enable();

  // Take soil matrix potential measurements
  if(equip.watermark) {
    watermark.activate();
    for(uint8_t ch = 0; ch < 4; ch++) {
      payload_102.matrixPotential[ch] = watermark.matrixPotentialCbars(ch);
    }
    watermark.deactivate();
  }

  // Read leaf wetness analog value
  if(equip.leafWet) {
    digitalWrite(LEAFWET_SUP_Pin, HIGH);
    delay(50);
    analogReadResolution(8);
    payload_102.leafWetness = analogRead(LEAFWET_SIG_Pin);
    digitalWrite(LEAFWET_SUP_Pin, LOW);
  }

  // Read DS18B20 temperature sensor
  if(equip.ds18b20) {
    if(ds.select(ds18b20Address)) { //check if device can be found on 1-Wire bus
      int16_t temperature = round(ds.getTempC() * 100);
      payload_101.temperature = temperature;
      payload_104.temperature = temperature;
    }
  }

  // Wait (BME280 measurement, WEIGHT supply stabilization, SigFox boot (at least 100ms))
  delay(100);

  // Read wind poti voltage + sum up speed measurements
  if(equip.wind) {
    DEBUG(F("Read WIND_DIR"));
    payload_100.windDirAndError = (uint8_t)(round(wind.direction() / 4.0)) << 1; // in 4° steps
    payload_100.windSpdAvg = round(wind.spdAvg());
    payload_100.windSpdStd = round(wind.spdStd());
    // Reset wind sample counter
    wind.sample = 0;
  }

  // Read weight cell voltage
  if(equip.weight1) {
    // save to payload
    payload_104.weight1 = weight.getGrams(0);
  }

  if(equip.weight2) {
    // save to payload
    payload_104.weight2 = weight.getGrams(1);
  }

  if(equip.weight1 || equip.weight2) {
    weight.disable();
  }

  if(equip.bme76) {
    payload_100.temperature = round(bme76.handle.readTempC() * 100); // in 0.01 °C
    payload_100.humidity = round(bme76.handle.readHumidity()); // in percent
    payload_100.pressure = round(bme76.handle.readPressure() * 10); // in 0.1 hPa
    payload_100.windDirAndError &= ~1; // clear error flag
  }
  else { // set error flag
    payload_100.windDirAndError |= 1;
  }

  if(equip.bme77) {
    payload_102.temperature = round(bme77.handle.readTempC() * 100); // in 0.01 °C
    payload_102.humidity = round(bme77.handle.readHumidity()); // in percent
    payload_102.pressure = round(bme77.handle.readPressure() * 10); // in 0.1 hPa
  }

  if(equip.rain) {
    payload_100.rainCount = rain.getCounts();
    payload_100.rainCountAcc = rain.getAccCounts();
    rain.resetCounts();
  }

  DEBUG("");

  if(txPld.p100) {
    DEBUG(F("Payload 100:"));
    DEBUG_VAL(F("    Temperature"), payload_100.temperature);
    DEBUG_VAL(F("       Humidity"), payload_100.humidity);
    DEBUG_VAL(F("       Pressure"), payload_100.pressure);
    DEBUG_VAL(F("      rainCount"), payload_100.rainCount);
    DEBUG_VAL(F("   rainCountAcc"), payload_100.rainCountAcc);
    DEBUG_VAL(F("windDirAndError"), payload_100.windDirAndError);
    DEBUG_VAL(F("     windSpdAvg"), payload_100.windSpdAvg);
    DEBUG_VAL(F("     windSpdStd"), payload_100.windSpdStd);
    DEBUG();
  }

  if(txPld.p101) {
    DEBUG(F("Payload 101:"));
    DEBUG_VAL(F("       Latitude"), payload_101.latitude);
    DEBUG_VAL(F("      Longitude"), payload_101.longitude);
    DEBUG_VAL(F("        DS18B20"), payload_101.temperature);
    DEBUG();
  }

  if(txPld.p102) {
    DEBUG(F("Payload 102:"));
    DEBUG_VAL(F("    Watermark 0"), payload_102.matrixPotential[0]);
    DEBUG_VAL(F("    Watermark 1"), payload_102.matrixPotential[1]);
    DEBUG_VAL(F("    Watermark 2"), payload_102.matrixPotential[2]);
    DEBUG_VAL(F("    Watermark 3"), payload_102.matrixPotential[3]);
    DEBUG_VAL(F("   Leaf Wetness"), payload_102.leafWetness);
    DEBUG_VAL(F("    Temperature"), payload_102.temperature);
    DEBUG_VAL(F("       Humidity"), payload_102.humidity);
    DEBUG_VAL(F("       Pressure"), payload_102.pressure);
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

  // Print raw hex payload message
  #if FALSE
  Serial.println("Payload 100");
  for(uint8_t i = 0; i < 12; i++) {
    Serial.print(*((uint8_t*)&payload+i), HEX);
    Serial.print(F(" "));
  }
  Serial.println();

  Serial.println("Payload 104");
  for(uint8_t i = 0; i < 12; i++) {
    Serial.print(*((uint8_t*)&payload+i), HEX);
    Serial.print(F(" "));
  }
  Serial.println();
  #endif

  // Clears all pending interrupts
  SigFox.status();
  delay(1);

  #if !DEBUG_SKIP_TX
  int ret;

  // Transmit Sigfox Messages
  if(txPld.p100) {
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&payload_100, PAYLOAD_LENGTH_100);
    // May take 10 seconds
    ret = SigFox.endPacket();
    DEBUG_VAL(F("Sigfox status (PLD 100) (0=OK)"), ret);
  }

  if(txPld.p101) {
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&payload_101, PAYLOAD_LENGTH_101);
    // May take 10 seconds
    ret = SigFox.endPacket();
    DEBUG_VAL(F("Sigfox status (PLD 101) (0=OK)"), ret);
  }

  if(txPld.p102) {
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&payload_102, PAYLOAD_LENGTH_102);
    // May take 10 seconds
    ret = SigFox.endPacket();
    DEBUG_VAL(F("Sigfox status (PLD 102) (0=OK)"), ret);
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
  #endif

  SigFox.end();

  // Set internal time to zero after each data packet
  rtc.reset();
  // Rest RAIN count time stamp together with RTC. Otherwise the RTC stamp will never be larger than last rain count stamp.
  rain.lastCountOstime = 0;
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
  mp[lpmodeWkupReasonRain] = lpmodeWkupReasonRain;
  mp[lpmodeWkupReasonButton] = lpmodeWkupReasonButton;
  
  // Get current time. RTC counts seconds but is reset after each packet cycle.
  uint32_t nowTimeSeconds;
  rtc.getTimeSeconds(&nowTimeSeconds);

  // The package interval in seconds
  uint16_t packageIntervalSeconds = PACKET_INTERVAL_INITIAL * 60;

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
    next[lpmodeWkupReasonWind] = ((nowTimeSeconds / WIND_SAMPLING_RATE) + 1) * WIND_SAMPLING_RATE;
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
void lpmodeInterruptRain() {
  wkupReason = lpmodeWkupReasonRain;
}

void lpmodeInterruptButton() {
  wkupReason = lpmodeWkupReasonButton;
}

void isrRainCountNoLpmode() {
  uint32_t nowTimeSeconds;
  rtc.getTimeSeconds(&nowTimeSeconds);
  DEBUG(F("RAIN NO LPMODE"));
  rain.increment(nowTimeSeconds);
  DEBUG_VAL(F("    RAIN current: "), rain.getCounts());
  DEBUG_VAL(F("RAIN accumulated: "), rain.getAccCounts());
}

void windIncrementSpdCount() {
  windSpdCounter++;
}

/* Perform a reboot */
void reboot() {
  NVIC_SystemReset();
  delay(100);
}
