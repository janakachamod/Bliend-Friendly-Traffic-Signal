#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
 
#define FIREBASE_HOST "blindfriendlytrafficsignal-default-rtdb.firebaseio.com" // Firebase host
#define FIREBASE_AUTH "PX3YtCPoAflP4D9zyZWuKU0fMjjjxuOE6iZgJ889" //Firebase Auth code
#define WIFI_SSID "AndroidAP_5607" //Enter your wifi Name
#define WIFI_PASSWORD "ccnacisco" // Enter your password

// Define pin numbers for LEDs and sensor
const int LED_RED_PIN = D1;
const int LED_YELLOW_PIN = D2;
const int LED_GREEN_PIN = D6;
const int SENSOR_PIN = D0;
const int SENSOR_PIN1 = D5;

// Define time intervals in milliseconds
const unsigned long RED_INTERVAL = 8000;
const unsigned long YELLOW_INTERVAL = 2000;
const unsigned long GREEN_INTERVAL = 8000;
unsigned long greenStartTime;

void setup() {
  // Initialize pins
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(SENSOR_PIN1, INPUT);
   greenStartTime = 0;
   
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected.");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("LED_RED", 0);
  Firebase.set("LED_GREEN", 0);
  Firebase.set("LED_YELLOW", 0);
  Firebase.set("TCRT5000", 0);
  Firebase.set("TCRT50001", 0);
  Firebase.set("SOME_VALUE", 1);
  
}

void loop() {

 

  // Turn on red LED and update Firebase
  digitalWrite(LED_RED_PIN, HIGH);
  Firebase.setInt("LED_RED", 1);
  
  updateSensorValue(RED_INTERVAL);
  
  // Turn off red LED and turn on yellow LED
  digitalWrite(LED_RED_PIN, LOW);
  Firebase.setInt("LED_RED", 0);
  digitalWrite(LED_YELLOW_PIN, HIGH);
  Firebase.setInt("LED_YELLOW", 1);
   
  updateSensorValue(YELLOW_INTERVAL);
  
  // Turn off yellow LED and turn on green LED
  digitalWrite(LED_YELLOW_PIN, LOW);
  Firebase.setInt("LED_YELLOW", 0);
  digitalWrite(LED_GREEN_PIN, HIGH);
  Firebase.setInt("LED_GREEN", 1);
   greenStartTime = millis();
   updateSensorValue(GREEN_INTERVAL);


 unsigned long greenStartTime = millis();
  // Check if green interval is less than 3000 milliseconds
  while (millis() - greenStartTime < GREEN_INTERVAL) {
    if (millis() - greenStartTime > GREEN_INTERVAL - 3000) {
      Firebase.setInt("SOME_VALUE", 0); // Update value in Firebase
    }
    else if (millis() - greenStartTime < GREEN_INTERVAL - 3000) {
      Firebase.setInt("SOME_VALUE", 1);
    }
    //updateSensorValue(100); // Call the updateSensorValue function every 100ms
  }
 
  
  // Turn off green LED
  digitalWrite(LED_GREEN_PIN, LOW);
  Firebase.setInt("LED_GREEN", 0);
  Firebase.setInt("SOME_VALUE", 1); 
}

void updateSensorValue(int interval) {
  unsigned long startTime = millis();
  while (millis() - startTime < interval) {
    int sensorValue = digitalRead(SENSOR_PIN);
    int sensorValue1 = digitalRead(SENSOR_PIN1);
    if(sensorValue == 1) {
      Firebase.setInt("TCRT5000", 0);
    } else {
      Firebase.setInt("TCRT5000", 1);
    }
    if(sensorValue1 == 1) {
      Firebase.setInt("TCRT50001", 0);
    } else {
      Firebase.setInt("TCRT50001", 1);
    }
    delay(100); // Delay to prevent reading the sensor too frequently
  }
}
