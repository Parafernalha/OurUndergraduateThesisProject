#include <Arduino.h>
#include <WiFi.h>

#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "Amilton"
#define WIFI_PASSWORD "contra619"

#define API_KEY "AIzaSyAWbJSXavdPsT5JBFmErz64w1TY2Zgw490"
#define DATABASE_URL "https://tcc-teste-c9d74-default-rtdb.firebaseio.com/" 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
bool buttonState;
const int buttonPin = 23;

const int ledPin = 22;
const int led1 = 16;
const int led2 = 19;
int intValue;
float floatValue;

void setup() {

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback; 
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
    //////////////////////SETs (Int, Float e String)////////////////////////////////
  if (Firebase.RTDB.getInt(&fbdo, "/test/int")) {
    intValue = fbdo.intData();
    Serial.print("intValue:");
    Serial.println(intValue);
  }
  if (Firebase.RTDB.getFloat(&fbdo, "/test/float")) {
    Serial.print("floatValue:");
    Serial.println(fbdo.floatData());
  }
  if (Firebase.RTDB.getString(&fbdo, "/test/string")) {
    Serial.print("stringValue:");
    Serial.println(fbdo.to<const char *>());
  }
  //////////////////////SETs (Int, Float e String)////////////////////////////////
  if (Firebase.RTDB.setFloat(&fbdo, "test/int", 1)) {}
  if (Firebase.RTDB.setFloat(&fbdo, "test/float", 1.0)) {}
  if (Firebase.RTDB.setString(&fbdo, "/test/string", "Hello World!")) {}
  ////////////////////////////////////////////////////////////////////////////////
  delay(2000);
}