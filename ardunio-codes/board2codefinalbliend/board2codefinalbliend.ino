#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
 
#define FIREBASE_HOST "blindfriendlytrafficsignal-default-rtdb.firebaseio.com" // Firebase host
#define FIREBASE_AUTH "PX3YtCPoAflP4D9zyZWuKU0fMjjjxuOE6iZgJ889" // Firebase Auth code
#define WIFI_SSID "AndroidAP_5607" //Enter your wifi Name
#define WIFI_PASSWORD "ccnacisco" // Enter your password

// Define pin number for buzzer
const int BUZZER_PIN = D1;
const int vibrate_pin = D6;

void setup() {
  // Initialize buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(vibrate_pin, OUTPUT);
  
  // Connect to Wi-Fi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected.");
  Serial.println(WiFi.localIP());
  Firebase.set("buzzer", 0);
   Firebase.set("vibrate", 0);
  
  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // Read TCRT5000 sensor value from Firebase
 
}

void loop() {

   int tcrtValue = Firebase.getInt("TCRT5000");
   int LED_GREEN=Firebase.getInt("LED_GREEN");
   int Some_value=Firebase.getInt("SOME_VALUE");
  // Control the buzzer based on the TCRT5000 value
  if (tcrtValue == 1) {
    digitalWrite(BUZZER_PIN, HIGH);
    Firebase.setInt("buzzer", 1);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    Firebase.setInt("buzzer", 0);
  }
  if (tcrtValue == 1 && LED_GREEN==1 && Some_value==1) {
    digitalWrite(vibrate_pin, HIGH);
    Firebase.setInt("vibrate", 1);
  } else {
    digitalWrite(vibrate_pin, LOW);
    Firebase.setInt("vibrate", 0);
  }
 
  
  delay(100);
  
}
