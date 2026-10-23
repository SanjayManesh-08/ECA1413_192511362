
const int gasPin = A0;
const int trigPin = 8;
const int echoPin = 9;
const int pirPin = 2;
const int ledPin = 11;
const int buzzerPin = 10;

const int gasLimit = 100;
const float occupancyLimit = 100.0;

int gasValue;
int motion;
long duration;
float distance;

bool gasAlert;
bool motionDetected;
bool nearObject;
bool occupancy;
bool systemSafe;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);

  Serial.println("SAFETY MONITORING SYSTEM");
  Serial.println("System Started");
}

void loop() {
  // Read gas sensor
  gasValue = analogRead(gasPin);

  // Read PIR sensor
  motion = digitalRead(pirPin);
  motionDetected = (motion == HIGH);

  // Measure ultrasonic distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    distance = -1;
  } else {
    distance = duration * 0.0343 / 2.0;
  }

  // Determine gas condition
  gasAlert = (gasValue > gasLimit);

  // Determine object proximity
  nearObject = (distance > 0 &&
                distance < occupancyLimit);

  // Determine occupancy
  occupancy = motionDetected || nearObject;

  // Determine overall safety
  systemSafe = !gasAlert && !occupancy;

  // LED operates during PIR motion
  if (motionDetected) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Buzzer operates during gas alert
  if (gasAlert) {
    tone(buzzerPin, 1000);
  } else {
    noTone(buzzerPin);
  }

  // Display all output parameters
  Serial.println();
  Serial.println("========== SAFETY MONITOR ==========");

  // Case 1, 2, 3: Gas level and gas alert
  Serial.print("Gas ADC Value       : ");
  Serial.println(gasValue);

  if (gasAlert) {
    Serial.println("Gas Status          : GAS ALERT");
  } else {
    Serial.println("Gas Status          : NORMAL");
  }

  // Case 4: PIR status
  Serial.print("PIR Status          : ");
  if (motionDetected) {
    Serial.println("MOTION DETECTED");
  } else {
    Serial.println("NOT DETECTED");
  }

  // Case 5, 6, 7: Distance and occupancy
  Serial.print("Ultrasonic Distance : ");
  if (distance < 0) {
    Serial.println("NO ECHO");
  } else {
    Serial.print(distance, 1);
    Serial.println(" cm");
  }

  Serial.print("Occupancy Status    : ");
  if (occupancy) {
    Serial.println("DETECTED");
  } else {
    Serial.println("NOT DETECTED");
  }

  // LED status
  Serial.print("LED Status          : ");
  Serial.println(motionDetected ? "ON" : "OFF");

  // Buzzer status
  Serial.print("Buzzer Status       : ");
  Serial.println(gasAlert ? "ON" : "OFF");

  // Safe condition
  Serial.print("System Status       : ");
  if (systemSafe) {
    Serial.println("SYSTEM SAFE");
  } else {
    Serial.println("WARNING");
  }

  Serial.println("====================================");

  delay(1000);
}
