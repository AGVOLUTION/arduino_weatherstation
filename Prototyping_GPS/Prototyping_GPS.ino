#define SUP A2


// Location of board specs
//Documents\ArduinoData\packages\arduino\hardware\samd\1.8.11\variants\mkrfox1200
// Reconfigure one of the SERIAL modules of AT SAM to use RX on A0

void setup() {
  // put your setup code here, to run once:
  pinMode(SUP, OUTPUT);
  digitalWrite(SUP, HIGH);

  Serial1.begin(9600);
  Serial.begin(9600);
}

/*
 * Experiments show: Time until Fixture on window bench: 45s; until time reception: 15s
 */
void loop() {
  // put your main code here, to run repeatedly:
  //if(Serial1.available()) {
  //  Serial1.write(Serial1.read());
  //}
    uint32_t tBeg, tNow;
    String line;
    float lat, lon;
    uint8_t hh, mm, ss; // UTC time
    String latOrient, lonOrient, latStr, lonStr;
    tBeg = millis();
    Serial1.println("Waiting for fix");
    while((millis() - tBeg) < 60000) {
      line = Serial1.readStringUntil('\n'); // read a single line
      /* The NMEA Global Positioning System Fix Data (GGA) format:
       * $GPGGA,HHMMSS.ss,BBBB.BBBB,b,LLLLL.LLLL,l,Q,NN,D.D,H.H,h,G.G,g,A.A,RRRR*PP or
       * $GPRMC,HHMMSS,A,BBBB.BBBB,b,LLLLL.LLLL,l,GG.G,RR.R,DDMMYY,M.M,m,F*PP
       * -> Use substrings due to fixed-width string format
       */
      // Search for $GPRMC command
      if(!line.substring(0, 6).equals("$GPRMC")) continue; // skip
      if(!line.substring(18, 19).equals("A")) continue; // character at pos. 18 has to be A=OK (V=WARNING). Otherwise -> no position fix
      Serial1.println("Found fix");
      Serial1.println(line); // forward line to Debug console

      // Extract UTC time
      hh = line.substring(7, 9).toInt();
      mm = line.substring(9, 11).toInt();
      ss = line.substring(11, 13).toInt();
      Serial1.print("UTC Time: "); Serial1.print(hh, DEC); Serial1.print(":"); Serial1.print(mm, DEC); Serial1.print(":"); Serial1.println(ss, DEC);

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

        Serial1.println(lat, 6);
        Serial1.println(lon, 6);
        break; // Leave after first position fix
      }
    }
    //digitalWrite(SUP, LOW); // Disable module
    delay(1000);
    digitalWrite(SUP, HIGH); // Enable module
    
}
