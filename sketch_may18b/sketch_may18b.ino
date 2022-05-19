#include <WiFi.h>

// WiFi Vars
const char* ssid = "PinkLand"; // wifi name
const char* password = "Pool349Now"; // wifi password

const int ledPin = 2;

void setup() {
  // put your setup code here, to run once:
  WiFiClient espClient;
  // Initialize the LED Pin
  pinMode(ledPin, OUTPUT); // init as an output
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
  while (WiFi.status() == WL_CONNECTED) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}
