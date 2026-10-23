
#include <Wire.h>

const byte LM75_ADDRESS = 0x48;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println("LM75AD TEMPERATURE MONITOR");
  Serial.println("--------------------------");
}

void loop() {
  float temperature = readTemperature();

  if (temperature == -999.0) {
    Serial.println("Sensor Error: Check I2C connection");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.println(" C");

    if (temperature >= 35.0) {
      Serial.println("Status: HIGH TEMPERATURE");
    } else {
      Serial.println("Status: NORMAL TEMPERATURE");
    }
  }

  Serial.println("--------------------------");
  delay(1000);
}

float readTemperature() {
  // Select temperature register
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x00);

  if (Wire.endTransmission() != 0) {
    return -999.0;
  }

  // Read two temperature bytes
  Wire.requestFrom(LM75_ADDRESS, (byte)2);

  if (Wire.available() < 2) {
    return -999.0;
  }

  byte msb = Wire.read();
  byte lsb = Wire.read();

  // LM75 temperature uses 11-bit signed data
  int raw = ((int)msb << 8) | lsb;
  raw >>= 5;

  if (raw & 0x0400) {
    raw -= 2048;
  }

  return raw * 0.125;
}
