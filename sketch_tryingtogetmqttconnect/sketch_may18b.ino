#include <stdio.h>
#include <string.h>
#include "WiFi.h"
#include "PubSubClient.h"
#include "client.h"

// WiFi Vars
char *ssid = "PinkLand"; // wifi name
const char *password = "Pool349Now"; // wifi password

// ESP32 Vars
const int ledPin = 2;

// VarS for Callback Function
byte *message;
unsigned int length;

// MQTT Broker Vars
const char *mqtt_broker = "192.168.56.1"; // Local
const char *topic = "esp32/led";
const char *mqtt_username = "Abby";
const char *mqtt_password = "PassWord1$Terrible";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// Setup Code, to Run Once:
void setup() {
  Serial.begin(115200);
 
  // Connecting to MQTT Broker
  client.setServer(mqtt_broker, mqtt_port);
  //client.setCallback(callback);

  // Connect to client
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.localIP());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public MQTT broker connected\n");
    } else {
      Serial.print("failed with state ");
      Serial.print("client.state()");
      delay(500);
    }//end if else
  }// end while

  // Publish and Subscribe
  client.publish(topic, "led");
  client.subscribe(topic);

  // Initialize the LED Pin
  pinMode(ledPin, OUTPUT); // init as an output
}// end setup

// main code here, to run repeatedly:
void loop() {
  // Connect to WiFi
  WiFi.begin(ssid, (const char*) password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }//end while
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
  
  if (!client.connected()) {
    reconnect();
  }//end if

  // Callback code
  Serial.print("Hello ESP32, Message arrived from topic: \n");
  Serial.print(topic);
  Serial.println();
  Serial.print("Message: ");
  String messageBuffer;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char) message[i]);
    messageBuffer += ((char) message[i]);
  }//end for
  Serial.println();

  // Check for messages from Computer
  if (String(topic) == "led") {
    Serial.println("Led State sent by Broker..");
    if (messageBuffer == "1") {
      Serial.print("On");
      digitalWrite(ledPin, HIGH);
    } else if (messageBuffer == "2") {
      Serial.print("Off");
      digitalWrite(ledPin, LOW);
    } else {
      Serial.print("Error with light");
    } //end else if
  }//end if
      
  /*while ((WiFi.status() == WL_CONNECTED)) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
   }*/
}//end loop

// Reconnect to MQTT
void reconnect() {
  //Try to reestablish connection
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(mqtt_broker, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT");
      // Subscribe
      client.publish("esp32/output", "OnOrOff");
      client.subscribe("led");
    } else {
      Serial.print("failed, rc =");
      Serial.print(client.state());
      Serial.println(" trying again...");
      delay(5000);
    }//end if else
  }//end while
}//end reconnect
