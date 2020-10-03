// Control based on serial input feed by connecting to ESP8266 or similar

void controlSetup() {
  Serial2.begin(115200);
}

void controlLoop() {
  if (Serial2.available() > 0) {
    incomingByte = Serial2.read();
    snake.input(incomingByte);
  }
}
