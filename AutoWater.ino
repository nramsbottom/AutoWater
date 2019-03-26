
int sensor_pin             = A0;     // sensor input. analog.
int sensor_power_pin       = 7;      // power up this pin to wake the sensor. digital.
int relay_pin              = 8;      // signal pin for relay. digital.
int sensor_high_mark       = 90;     // calibrated "wet" value for the sensor
int sensor_low_mark        = 860;    // calibrated "dry" value for the sensor
int relay_enable_threshold = 45;     // percent

int sensor_value;
bool relay_enabled = false;
int cycle_delay = 1000;

void setup() {

  pinMode(sensor_pin, INPUT);
  pinMode(relay_pin, OUTPUT);
  pinMode(sensor_power_pin, OUTPUT);

  relay_off();
  sensor_off();
  
  Serial.begin(9600);
  Serial.println("Reading from sensor...");
}

void loop() {

  sensor_on();
  sensor_value = analogRead(sensor_pin);
  sensor_off();
   
  // convert the analog value to a percentage
  sensor_value = map(sensor_value, sensor_low_mark, sensor_high_mark, 0, 100);

  Serial.print("Soil moisture level: ");
  Serial.print(sensor_value);
  Serial.println("%");

  if (sensor_value < relay_enable_threshold) {
    if (!relay_enabled) {
      Serial.print("Moisture below threshold of ");
      Serial.print(relay_enable_threshold);
      Serial.println("%. Enabling relay.");
      relay_on();
    }
  } else {
    if (relay_enabled) {
      Serial.print("Moisture now above threshold of ");
      Serial.print(relay_enable_threshold);
      Serial.println("%. Disabling relay.");
      relay_off();
    }
  }

  if (relay_enabled) {
    Serial.println("Relay is active.");
  }

  delay(cycle_delay);
}

void sensor_off() {
  digitalWrite(sensor_power_pin, LOW);
  delay(50);
}

void sensor_on() {
  digitalWrite(sensor_power_pin, HIGH);
  delay(100); // give module time to start.
}

void relay_off() {
  Serial.println("Turning relay off.");
  relay_enabled = false;
  digitalWrite(relay_pin, LOW);
}

void relay_on() {
  Serial.println("Turning relay on.");
  relay_enabled = true;
  digitalWrite(relay_pin, HIGH);
}
