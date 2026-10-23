
const int tempPin = A0;
const int fanPin = 6;  // PWM pin connected to transistor driver

int sensorValue;
float voltage;
float temperature;
int pwmPercent;
int pwmValue;

void setup() {
  pinMode(fanPin, OUTPUT);
  Serial.begin(9600);

  Serial.println("TEMPERATURE BASED FAN SPEED CONTROL");
  Serial.println("LM35 SENSOR + ARDUINO UNO");
  Serial.println("------------------------------------");
}

void loop() {
  // 1. Acquire temperature from LM35
  sensorValue = analogRead(tempPin);

  voltage = sensorValue * (5.0 / 1023.0);
  temperature = voltage * 100.0;

  // 2. Set PWM based on temperature
  if (temperature < 25.0) {
    pwmPercent = 0;
  }
  else if (temperature < 30.0) {
    pwmPercent = 30;
  }
  else if (temperature < 35.0) {
    pwmPercent = 60;
  }
  else {
    pwmPercent = 90;
  }

  // 3. Convert percentage to Arduino PWM value
  pwmValue = map(pwmPercent, 0, 100, 0, 255);

  // 4. Control fan speed
  analogWrite(fanPin, pwmValue);

  // 5. Display temperature acquisition
  Serial.println();
  Serial.println("========== FAN CONTROL ==========");

  Serial.print("ADC Value       : ");
  Serial.println(sensorValue);

  Serial.print("Temperature     : ");
  Serial.print(temperature, 2);
  Serial.println(" C");

  // 6. Display PWM percentage
  Serial.print("PWM Percentage  : ");
  Serial.print(pwmPercent);
  Serial.println("%");

  Serial.print("PWM Output      : ");
  Serial.print(pwmValue);
  Serial.println(" / 255");

  // 7. Display temperature condition
  if (temperature < 25.0) {
    Serial.println("Condition       : LOW TEMPERATURE");
    Serial.println("Fan Status      : OFF");
  }
  else if (temperature < 30.0) {
    Serial.println("Condition       : MODERATE TEMPERATURE");
    Serial.println("Fan Status      : LOW SPEED");
  }
  else if (temperature < 35.0) {
    Serial.println("Condition       : HIGH TEMPERATURE");
    Serial.println("Fan Status      : MEDIUM SPEED");
  }
  else {
    Serial.println("Condition       : VERY HIGH TEMPERATURE");
    Serial.println("Fan Status      : HIGH SPEED");
  }

  Serial.println("Control Status  : RUNNING");
  Serial.println("=================================");

  delay(1000);
}
