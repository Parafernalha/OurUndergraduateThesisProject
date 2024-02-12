#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "LYKENET_BEATRIZ"
#define WIFI_PASSWORD "27041205"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBX1JnLTsQ6tjy9kE8Ovf6MuX5qxuQcRdc"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://my-undergraduate-thesis-f90da-default-rtdb.firebaseio.com/" 

TaskHandle_t Task1;
TaskHandle_t Task2;


void Task1code( void * pvParameters );
void Task2code( void * pvParameters );

//Define Firebase Data object
FirebaseData fbdo; //fbdo stands for firebase data object


FirebaseAuth auth;
FirebaseConfig config;



unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
bool buttonState;
const int buttonPin = 23;
// LED pins
const int ledPin = 22;
const int led1 = 16;
const int led2 = 19;

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

   /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(led1, HIGH);
    delay(1000);
    digitalWrite(led1, LOW);
    delay(1000);
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters )
{
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;)
  {
    digitalWrite(led2, HIGH);
    delay(700);
    digitalWrite(led2, LOW);
    delay(700);

    buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH)
   {
        // Switch on the led
        digitalWrite(ledPin, HIGH);
        delay(100);
        Serial.println("1");
    } 
    else 
    {
        // Switch off the led
        digitalWrite(ledPin, LOW);
        delay(100);
        Serial.println("0");
    }
    delay(10);
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
  
    Firebase.RTDB.setBool(&fbdo, "button_state/button", buttonState);
  }
  }
}

void loop() {
  
}