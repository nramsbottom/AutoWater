
const int sensor_pin             = A0;     // sensor input. analog.
const int sensor_power_pin       = 7;      // power up this pin to wake the sensor. digital.
const int relay_pin              = 8;      // signal pin for relay. digital.
const int sensor_high_mark       = 90;     // calibrated "wet" value for the sensor
const int sensor_low_mark        = 860;    // calibrated "dry" value for the sensor
const int relay_enable_threshold = 45;     // percent
const int cycle_delay            = 1000;   // poll sensor every x millisseconds

const int EventType_Reading     = 1;
const int EventType_Low         = 2;
const int EventType_High        = 3;
const int EventType_Information = 4;

int sensor_value;
bool relay_enabled = false;

void setup() {

  pinMode(sensor_pin, INPUT);
  pinMode(relay_pin, OUTPUT);
  pinMode(sensor_power_pin, OUTPUT);

  relay_off();
  sensor_off();
  
  Serial.begin(9600);

  output_event_xml(EventType_Information, "Reading from sensor...");
}

void loop() {

  sensor_on();
  sensor_value = analogRead(sensor_pin);
  sensor_off();
   
  // convert the analog value to a percentage
  sensor_value = map(sensor_value, sensor_low_mark, sensor_high_mark, 0, 100);

  output_event_xml(EventType_Reading, String(sensor_value));

  if (sensor_value < relay_enable_threshold) {
    if (!relay_enabled) {
      output_event_xml(EventType_Low, "Deactivating pump.");
      relay_on();
    }
  } else {
    if (relay_enabled) {
      output_event_xml(EventType_Low, "Activating pump.");
      relay_off();
    }
  }

  if (relay_enabled) {
    output_event_xml(EventType_Information, "Relay is active.");
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
  output_event_xml(EventType_Information, "Turning relay off.");
  relay_enabled = false;
  digitalWrite(relay_pin, LOW);
}

void relay_on() {
  output_event_xml(EventType_Information, "Turning relay on.");
  relay_enabled = true;
  digitalWrite(relay_pin, HIGH);
}

void output_event_xml(int type, String message) {
  Serial.print("<event>");
    
    Serial.print("<type>");
    Serial.print(type);
    Serial.print("</type>");
    
    Serial.print("<message>");
    Serial.print(message);
    Serial.print("</message>");
    
  Serial.print("</event>");
  Serial.println();
}
