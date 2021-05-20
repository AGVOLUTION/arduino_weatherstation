#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  Wire.begin();
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Contacting 0x76");
  Wire.beginTransmission(0x76);
  Wire.write(0xD0); // id
  Wire.endTransmission();
  Wire.requestFrom(0x76, 1);
  char c = Wire.read();
  Serial.println(c, HEX);
  delay(1000);
}
