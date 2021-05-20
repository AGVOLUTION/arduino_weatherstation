/*************
 * TODO
 * 1. There is a dead time for rain count events during wind sampling and SigFox transmission
 * 2. Record BME76 error flag
 * 3. If BME76 looses initialization (power cut) -> Detect and Re-Init ? Which registers need to be written?
 * 4. Search for DS18B20 on bus instead of specifying address manually
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
 * 12. Include rainCount module: +15-20uA (probably the pull-up resistor)
 * 13. Include davisWind module: No additional cost during stop mode, but speed sample cycles more frequently than packet cycle
 * 14. BME280 not initialized or initialized (shutdown): 10uA
 * 
 * Final 3V3 current BME76, Davis Wind, Rain Count, No SD): 64uA. With SD Card: 130uA -> Half-life!
 */

uint8_t ds18b20Address[] = {40, 170, 237, 186, 83, 20, 1, 39}; 

#include "Inc/hardware.h"
#include "Inc/commission/commission.h"

/* Check which payload versions to include */
#define INC_PLD_100 (EQUIP_BME76 | EQUIP_DAVIS_RAIN | EQUIP_DAVIS_WIND)
#define INC_PLD_104 (EQUIP_WEIGHT | EQUIP_DS18B20)

#if INC_PLD_100
#include "Inc/payload_format/payload_format.h"
#endif

#if INC_PLD_104
#include "Inc/payload_format/payload_format_104.h"
#endif

// Include basic modules
#include "ArduinoLowPower.h"
#include <RTCZero.h>
#include <SigFox.h>
#include <Wire.h>
#include <SD.h>
#include <DS18B20.h>
#include "BlueDot_BME280.h"
#include <FlashStorage.h>

extern "C" {
  #include "rainCount.h"
  #include "davisWind.h"
}
#include "sdlog.h"

// Declare new payload buffer
#if INC_PLD_100
payload_t payload;
#endif

#if INC_PLD_104
payload_104_t payload_104;
#endif

// wake-up routing
#define lpmodeWkupReasonSize 3
typedef enum __lpmodeWkupReason_t {
  lpmodeWkupReasonReport,
  lpmodeWkupReasonDavisWind,
  lpmodeWkupReasonDavisRain
} lpmodeWkupReason_t;

// Global flag which is set by interrupt callbacks causing the wake up
lpmodeWkupReason_t wkupReason = lpmodeWkupReasonReport;
// Global flag which stores RTC wakeup reasons specifically
lpmodeWkupReason_t rtcAlarmEvent = lpmodeWkupReasonReport;
// Last transmit time
static uint32_t ostimeLastPacketTx = 0;

// RTC Handle
RTCZero rtc;

#if EQUIP_BME76
// BME 0x76 handle
BlueDot_BME280 bme1;
#endif

#if EQUIP_DAVIS_RAIN
// Rain Count handle
rainCount_t rainCount;
FlashStorage(rainCountAccFlash, uint16_t);
FlashStorage(rainCountAccFlashSeal, uint16_t);
#endif

#if EQUIP_DAVIS_WIND
// Davis Wind handle
davisWind_t davisWind;
#endif

#if EQUIP_SDLOG
// SD LOG handle
sdlog_t sdlog;
#endif

#if EQUIP_DS18B20
DS18B20 ds(7);
#endif

void setup() {
  #if DEBUG_SERIAL
  // Setup UART
  Serial1.begin(38400);
  Serial1.println(F("*************************************************"));
  Serial1.println(F("*A G V O L U T I O N    M A C R O C L I M A T E *"));
  Serial1.println(F("*************************************************"));
  #endif

  // Setup Real Time Clock RTC
  rtc.begin();

  uint8_t sigfoxState = SigFox.begin();

  #if DEBUG_SERIAL
  Serial1.print(F("SigFox State: "));
  Serial1.println(sigfoxState);
  #endif

  if (!sigfoxState) {
    // Something is really wrong, try rebooting
    // Reboot is useful if we are powering the board using an unreliable power source
    // (eg. solar panels or other energy harvesting methods)
    reboot();
  }

  String version = SigFox.SigVersion();
  String ID = SigFox.ID();
  String PAC = SigFox.PAC();

  Serial.begin(38400); // output SigFox parameters over USB serial in any case
  Serial.println("SigFox FW version " + version);
  Serial.println("ID  = " + ID);
  Serial.println("PAC = " + PAC);

  #if DEBUG_SERIAL
  Serial1.println("SigFox FW version " + version);
  Serial1.println("ID  = " + ID);
  Serial1.println("PAC = " + PAC);
  #endif

  //Send module to standby until we need to send a message
  SigFox.end();

  #if EQUIP_SDLOG
  // Initialize SD Card
  if (!SD.begin(SDLOG_CS_Pin)) {
    #if DEBUG_SERIAL
    Serial1.println(F("SD card failed or not present."));
    #endif
    sdlog.cardPresent = 0;
  }
  else {
    #if DEBUG_SERIAL
    Serial1.println(F("SD card initialized."));
    #endif
    sdlog.cardPresent = 1;
  }

  if(sdlog.cardPresent) {
    File headerfile = SD.open("header.txt", FILE_WRITE);
    if (! headerfile) {
      #if DEBUG_SERIAL
      Serial1.println(F("Error opening SD headerfile."));
      #endif
    }
    String headerString = "T,RH,P,RAIN/LATEST,RAIN/ACC,WIND/DIR,WIND/SPD/AVG,WIND/SPD/STD";
    headerfile.println(headerString);
    String unitString = "°C,%,hPa,COUNTS,COUNTS,°,km h-1,km h-1";
    headerfile.println(unitString);
    headerfile.close();
  
    sdlog.datafile = SD.open("log.txt", FILE_WRITE);
    if (! sdlog.datafile) {
      #if DEBUG_SERIAL
      Serial1.println(F("Error opening SD logfile."));
      #endif
    }
    #if DEBUG_SERIAL
      Serial1.println(F("Completed SD setup."));
    #endif
  }
  #endif
  
  // Setup GPIOs
  setupGpios();

  // Attach wakeup interrupts
  attachWakeupInterrupts();

  // Setup BME280
  #if EQUIP_BME76
  setupBME280(&bme1);
    #if DEBUG_SERIAL
      Serial1.println(F("BME setup completed."));
    #endif
  #endif
  // Setup raincount peripheral
  #if EQUIP_DAVIS_RAIN
  setupRainCount(&rainCount);
  // Check if Flash area which stores the rain accumulation is properly initialized
  if(rainCountAccFlashSeal.read() != 0xaffe) {
    // Initialize -> set acc counter to zero and destroy seal
    rainCountAccFlash.write(0);
    rainCountAccFlashSeal.write(0xaffe);
  }
  #endif

  // Reset RTC
  rtcReset();

  #if DEBUG_SERIAL
  Serial1.println(F("Setup completed."));
  Serial1.println();
  #endif
}

void setupGpios() {
  #if EQUIP_DAVIS_RAIN
  pinMode(DAVIS_RAIN_Pin, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(DAVIS_RAIN_Pin), davisRainCountIncrement, FALLING);
  #endif

  #if EQUIP_DAVIS_WIND
  pinMode(DAVIS_WIND_SPD_Pin, INPUT_PULLUP);
  pinMode(DAVIS_WIND_SUP_Pin, OUTPUT);
  digitalWrite(DAVIS_WIND_SUP_Pin, LOW);
  #endif

  /* Use leaf wetness connector to output 3.3V and measure the voltage of the weight cell */
  #if EQUIP_WEIGHT
  pinMode(WEIGHT_SUP_Pin, OUTPUT);
  digitalWrite(WEIGHT_SUP_Pin, LOW);
  #endif

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void attachWakeupInterrupts() {
  // Attach wake-up interrupts during LPMODE
  #if EQUIP_DAVIS_RAIN
  LowPower.attachInterruptWakeup(DAVIS_RAIN_Pin, lpmodeInterruptDavisRain, FALLING);
  #endif
  LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, lpmodeInterruptRTC, CHANGE);
}

uint32_t rtcGetTimeSeconds(uint32_t *t) {
  *t = rtc.getSeconds() + 60*(rtc.getMinutes() + 60*(rtc.getHours() + 24*rtc.getDay() ) );
}

void rtcReset() {
  rtc.setSeconds(0);
  rtc.setMinutes(0);
  rtc.setHours(0);
  rtc.setDay(1);
  rtc.setMonth(1);
  rtc.setYear(1);
}

void loop() {
  // Enter low power mode by default
  lpmodeEnterStop();

  // After wakeup, act based on return value
  //Serial1.print(F("wkupReason="));
  //Serial1.println(wkupReason);
  uint32_t nowTimeSeconds;
  rtcGetTimeSeconds(&nowTimeSeconds);
  
  switch(wkupReason) {
    case lpmodeWkupReasonReport:
      #if DEBUG_SERIAL
      Serial1.println(F("REPORT"));
      #endif
      report();
    break;
    
    #if EQUIP_DAVIS_WIND
    case lpmodeWkupReasonDavisWind:
      #if EQUIP_DAVIS_RAIN  // do not ignore Rain Count Interrupts during wind sampling
      attachInterrupt(DAVIS_RAIN_Pin, lpmodeInterruptDavisRain, FALLING);
      #endif
    
      #if DEBUG_SERIAL
      Serial1.println(F("DAVIS_WIND"));
      #endif
      davisWindAddSpdSample(&davisWind);
      #if DEBUG_SERIAL
        Serial1.print(F("       davisWind->sample: "));
        Serial1.println(davisWind.sample-1);
        Serial1.print(F("davisWind->spdSamples[i]: "));
        Serial1.println(davisWind.spdSamples[davisWind.sample-1]);
        Serial1.println();
      #endif

      #if EQUIP_DAVIS_RAIN
      detachInterrupt(DAVIS_RAIN_Pin);
      #endif
    break;
    #endif
    
    #if EQUIP_DAVIS_RAIN
    case lpmodeWkupReasonDavisRain:
      {
      #if DEBUG_SERIAL
      Serial1.println(F("DAVIS_RAIN"));
      #endif
      // Increment rain counter by one
      uint16_t rainAccTmp = rainCountAccFlash.read();
      if(davisRainCountIncrement(&rainCount, nowTimeSeconds)) {
        // Increment accumulated value by one
        rainCountAccFlash.write(rainAccTmp+1);
      }
      #if DEBUG_SERIAL
        Serial1.print(F("    DAVIS_RAIN current: "));
        Serial1.println(rainCount.current);
        Serial1.print(F("DAVIS_RAIN accumulated: "));
        Serial1.println(rainAccTmp);
      #endif
      }
    break;
    #endif
    
    default:
    // do nothing
    break;
  }
}

/* Transmit a data packet.
 *
 */
void report() {
  #if DEBUG_SERIAL
  Serial1.println(F("SigFox Module Boot"));
  #endif

  // Start the module
  SigFox.begin();
  //#if DEBUG_LED
  SigFox.debug();
  //#endif
  
  // Submit sensor read tasks
  #if EQUIP_BME76
  #if DEBUG_SERIAL
  Serial1.println(F("Trigger BME280"));
  #endif
  bme1.writeCTRLMeas(); // Trigger BME280 measurement by writing mode parameter (set to force) again
  #endif

  // Turn on voltage supply for wind poti
  #if EQUIP_DAVIS_WIND
  #if DEBUG_SERIAL
  Serial1.println(F("Turn on DAVIS_WIND_SUP"));
  #endif
  digitalWrite(DAVIS_WIND_SUP_Pin, HIGH);
  #endif

  // Turn on voltage supply for weight cell
  #if EQUIP_WEIGHT
  #if DEBUG_SERIAL
  Serial1.println(F("Turn on WEIGHT_SUP"));
  #endif
  digitalWrite(WEIGHT_SUP_Pin, HIGH);
  #endif

  // Wait (BME280 measurement, WIND supply stabilization, WEIGHT supply stabilization, SigFox boot (at least 100ms))
  delay(10000);

  // Read davis wind poti voltage
  #if EQUIP_DAVIS_WIND
  #if DEBUG_SERIAL
  Serial1.println(F("Read DAVIS_WIND_DIR"));
  #endif
  double davisWindPoti;
  analogReadResolution(10);
  davisWindPoti = analogRead(DAVIS_WIND_DIR_Pin);
  digitalWrite(DAVIS_WIND_SUP_Pin, LOW);
  // ADC resolution is set to 10 bit -> 0 = 0°, 1023 = 360°
  double davisWindDirDeg = davisWindPoti / 1023.0 * 360.0;
  #endif

  // Read DS18B20 temperature sensor
  #if EQUIP_DS18B20
  if(ds.select(ds18b20Address)) { //check if device can be found on 1-Wire bus
    payload_104.temperature = round(ds.getTempC() * 100);
  }
  #endif

  // Read weight cell voltage
  #if EQUIP_WEIGHT
  #if DEBUG_SERIAL
  Serial1.println(F("Read WEIGHT_SIG"));
  #endif
  double weightVoltagePos;
  double weightVoltageNeg;
  analogReadResolution(12); // MKR series has up to 12 bits ADC resolution -> 4096 divisions
  weightVoltagePos = analogRead(WEIGHT_SIG_POS_Pin);
  weightVoltageNeg = analogRead(WEIGHT_SIG_NEG_Pin);
  Serial1.print("Weight cell ADC (+):");
  Serial1.println(weightVoltagePos);
  Serial1.print("Weight cell ADC (-):");
  Serial1.println(weightVoltageNeg);
  digitalWrite(WEIGHT_SUP_Pin, LOW);
  // ADC resolution is set to 12 bit -> 0 = 0g, 4095 = 150kg
  float weightKilograms = (weightVoltagePos - weightVoltageNeg) / 4095.0 * 150.0;

  // save to payload
  payload_104.weight = weightKilograms;
  #endif

  #if EQUIP_BME76
  payload.temperature = round(bme1.readTempC() * 100); // in 0.01 °C
  payload.humidity = round(bme1.readHumidity()); // in percent
  payload.pressure = round(bme1.readPressure() * 10); // in 0.1 hPa
  #endif

  #if EQUIP_DAVIS_RAIN
  uint16_t rainCntAccTmp = rainCountAccFlash.read();
  payload.rainCount = rainCount.current;
  payload.rainCountAcc = rainCntAccTmp;
  // Reset rain counter
  rainCount.current = 0;
  #endif

  #if EQUIP_DAVIS_WIND
  payload.windDirAndError = (uint8_t)(round(davisWindDirDeg / 4.0)) << 1; // in 4° steps
  payload.windSpdAvg = round(davisWindSpdAvg(&davisWind));
  payload.windSpdStd = round(davisWindSpdStd(&davisWind));
  // Reset wind sample counter
  davisWind.sample = 0;
  #endif

  #if DEBUG_SERIAL
  Serial1.println();
  Serial1.println(F("Payload Message:"));

  #if INC_PLD_100
  Serial1.println("    Temperature: " + String(payload.temperature));
  Serial1.println("       Humidity: " + String(payload.humidity));
  Serial1.println("       Pressure: " + String(payload.pressure));
  Serial1.println("      rainCount: " + String(payload.rainCount));
  Serial1.println("   rainCountAcc: " + String(payload.rainCountAcc));
  Serial1.println("windDirAndError: " + String(payload.windDirAndError));
  Serial1.println("     windSpdAvg: " + String(payload.windSpdAvg));
  Serial1.println("     windSpdStd: " + String(payload.windSpdStd));
  #endif
  
  #if INC_PLD_104
  Serial1.println("         Weight: " + String(payload_104.weight));
  Serial1.println("   DS18B20 Temp: " + String(payload_104.temperature));
  #endif
  
  Serial1.println();
  #endif

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
  // Transmit Sigfox message
  #if INC_PLD_100
  SigFox.beginPacket();
  SigFox.write((uint8_t*)&payload, 12);
  // May take 10 seconds
  ret = SigFox.endPacket();

    #if DEBUG_SERIAL
    Serial1.print(F("Sigfox status (0=OK): "));
    Serial1.println(ret, DEC);
    #endif
  #endif

  // Transmit Sigfox message Payload version 104
  #if INC_PLD_104
    // Transmit Sigfox message
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&payload_104, PAYLOAD_LENGTH_104);
    // May take 10 seconds
    ret = SigFox.endPacket();
  
    #if DEBUG_SERIAL
    Serial1.print(F("Sigfox status (PLD 104) (0=OK): "));
    Serial1.println(ret, DEC);
    #endif
  #endif
#endif
  SigFox.end();

  // Write log string to SD Card
  #if EQUIP_SDLOG
    String sep = "\t";
    String logString = String(payload.temperature / 100.0) + sep + String(payload.humidity) + sep + String(payload.pressure / 10.0) + sep + String(payload.rainCount) + sep + String(payload.rainCountAcc) + sep + String( (payload.windDirAndError >> 1) * 4) + sep + String(payload.windSpdAvg) + sep + String(payload.windSpdStd);
    sdlog.datafile.println(logString);
    sdlog.datafile.flush();
  #endif

  // Set internal time to zero after each data packet
  rtcReset();
  // Rest RAIN count time stamp together with RTC. Otherwise the RTC stamp will never be larger than last rain count stamp.
  rainCount.lastCountOstime = 0;
  rtcGetTimeSeconds(&ostimeLastPacketTx);
}

/*  Call this method to enter low power state.
 *  RTC wake up is configured automatically.
 */
void lpmodeEnterStop() {
  #if DEBUG_SERIAL
  Serial1.println();
  Serial1.println(F("Entering LPMODE"));
  
  uint32_t nowTimeSeconds;
  Serial1.print(F("       RTC Time: "));
  rtcGetTimeSeconds(&nowTimeSeconds);
  Serial1.println((int)nowTimeSeconds, DEC);
  #endif
  
  // Switch off builtin LED to signal low power mode
  digitalWrite(LED_BUILTIN, LOW);
  // calculate time delta until next wakeup alarm
  int delayUntilNextWakeup = lpmodeScheduleWakeup();
  delayUntilNextWakeup *= 1000; // from milliseconds to seconds
  #if DEBUG_NO_LPMODE
  delay(delayUntilNextWakeup);
  #else
  // enter low power mode and schedule wakeup alert
  LowPower.sleep(delayUntilNextWakeup);
  #endif

  #if DEBUG_LED
  digitalWrite(LED_BUILTIN, HIGH);
  #endif
  
  #if DEBUG_SERIAL
  Serial1.println(F("Waking up from LPMODE"));
  Serial1.print(F("       RTC Time: "));
  rtcGetTimeSeconds(&nowTimeSeconds);
  Serial1.println(nowTimeSeconds, DEC);
  Serial1.println();
  #endif
  return;
}

/* Schedule the next wakeup alarm. */
int lpmodeScheduleWakeup() {
  // Specify a mapping from list indizes (0..N) -> lpmodeWkupReason ENUM
  lpmodeWkupReason_t mp[lpmodeWkupReasonSize];
  mp[lpmodeWkupReasonReport] = lpmodeWkupReasonReport;
  mp[lpmodeWkupReasonDavisWind] = lpmodeWkupReasonDavisWind;
  mp[lpmodeWkupReasonDavisRain] = lpmodeWkupReasonDavisRain;
  
  // Get current time. RTC counts seconds but is reset after each packet cycle.
  uint32_t nowTimeSeconds;
  rtcGetTimeSeconds(&nowTimeSeconds);

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
  #if DEBUG_SERIAL
  Serial1.print(F("    Next REPORT: "));
  Serial1.println(next[lpmodeWkupReasonReport], DEC);
  #endif

  /* Schedule DAVIS_WIND */
#if EQUIP_DAVIS_WIND
  // sample every 30s
    next[lpmodeWkupReasonDavisWind] = ((nowTimeSeconds / WIND_SAMPLING_RATE) + 1) * WIND_SAMPLING_RATE;
    // Safety feature: Only schedule new wind measurements until next package is due
    if(next[lpmodeWkupReasonDavisWind] >= (next[lpmodeWkupReasonReport] - 1)) {
      next[lpmodeWkupReasonDavisWind] = 0xffffffff;
    }
    #if DEBUG_SERIAL
    Serial1.print(F("Next DAVIS_WIND: "));
    Serial1.println(next[lpmodeWkupReasonDavisWind], DEC);
    #endif
#endif

  uint8_t closestAlarm = 0;

  // Find the closest alarm
  for(int i = 1; i < lpmodeWkupReasonSize; i++) {
    if(next[i] < next[closestAlarm]) {
      closestAlarm = i;
    }
  }

  /* Register RTC event */
  rtcAlarmEvent = mp[closestAlarm];
  //Serial1.print(F("lpmodeWkupReasonDavisWind="));
  //Serial1.println(lpmodeWkupReasonDavisWind);
  // Serial1.print(F("rtcAlarmEvent="));
  // Serial1.println(rtcAlarmEvent);
  
  // Get time delta until next alarm
  return (int)(next[closestAlarm] - nowTimeSeconds);
}

/* Wake up from Real Time Clock Alarm.
 * Set global wakeup reason flag to the identical value
 * as set before going to sleep on RTC wakeup (rtcAlarmEvent).
 */
void lpmodeInterruptRTC() {
  // Serial1.print(F("wkupReason="));
  // Serial1.println(wkupReason);
  wkupReason = rtcAlarmEvent;
  // Serial1.print(F("wkupReason="));
  // Serial1.println(wkupReason);
}

/*  Wake up from Davis Rain Gauge tipping spoon interrupt event.
 *  Set global wakeup reason flag.
 */
#if EQUIP_DAVIS_RAIN
void lpmodeInterruptDavisRain() {
  wkupReason = lpmodeWkupReasonDavisRain;
}
#endif

/* Perform a reboot */
void reboot() {
  NVIC_SystemReset();
  delay(100);
}

void setupBME280(BlueDot_BME280 *bme) {
  //This program is set for the I2C mode
  bme->parameter.communication = 0;                    //I2C communication for Sensor 1 (bme)
  bme->parameter.I2CAddress = 0x76;                    //I2C Address for Sensor 1 (bme)
  
  //0b00:     In sleep mode no measurements are performed, but power consumption is at a minimum
  //0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode
  //0b11:     In normal mode the sensor measures continually (default value)
  bme->parameter.sensorMode = 0b01;                    //Setup Sensor mode for Sensor 1
                
  //Great! Now set up the internal IIR Filter
  //The IIR (Infinite Impulse Response) filter suppresses high frequency fluctuations
  //In short, a high factor value means less noise, but measurements are also less responsive
  //You can play with these values and check the results!
  //In doubt just leave on default
  
  //0b000:      factor 0 (filter off)
  //0b001:      factor 2
  //0b010:      factor 4
  //0b011:      factor 8
  //0b100:      factor 16 (default value)
  
  bme->parameter.IIRfilter = 0b000;                   //IIR Filter for Sensor 1
  
  //Next you'll define the oversampling factor for the humidity measurements
  //Again, higher values mean less noise, but slower responses
  //If you don't want to measure humidity, set the oversampling to zero
  
  //0b000:      factor 0 (Disable humidity measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)
  
  bme->parameter.humidOversampling = 0b001;            //Humidity Oversampling for Sensor 1
  
  //Now define the oversampling factor for the temperature measurements
  //You know now, higher values lead to less noise but slower measurements
  
  //0b000:      factor 0 (Disable temperature measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)
  
  bme->parameter.tempOversampling = 0b001;              //Temperature Oversampling for Sensor 1
  
  //Finally, define the oversampling factor for the pressure measurements
  //For altitude measurements a higher factor provides more stable values
  //On doubt, just leave it on default
  
  //0b000:      factor 0 (Disable pressure measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)  
  
  bme->parameter.pressOversampling = 0b001;             //Pressure Oversampling for Sensor 1
  
  //For precise altitude measurements please put in the current pressure corrected for the sea level
  //On doubt, just leave the standard pressure as default (1013.25 hPa);
  
  bme->parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 1)
  
  //Also put in the current average temperature outside (yes, really outside!)
  //For slightly less precise altitude measurements, just leave the standard temperature as default (15°C and 59°F);
  
  bme->parameter.tempOutsideCelsius = 15;               //default value of 15°C
  bme->parameter.tempOutsideFahrenheit = 59;            //default value of 59°F

  uint8_t bmeStatus = bme->init();
  #if DEBUG_SERIAL
  Serial1.println(F("Init BME280"));
  if (bmeStatus != 0x60) {  // Check CHIP ID
    Serial1.println(F("BME280 not found!"));
  }
  #endif
}
