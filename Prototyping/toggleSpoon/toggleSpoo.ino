void setup() {
  // put your setup code here, to run once:
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(0, LOW);
  delay(50);
  digitalWrite(0, HIGH);
  delay(10000);
}
