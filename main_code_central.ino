// TCC

#include <SPI.h>
#include <Ethernet.h>
#include <Firebase_ESP_Client.h>

#define pinBotao 14

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Set the static IP address to use if the DHCP fails to assign
#define MYIPADDR 192,168,1,28
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,1
#define MYGW 192,168,1,1

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
//---------------- FIREBASE -------------------------
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBX1JnLTsQ6tjy9kE8Ovf6MuX5qxuQcRdc"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://my-undergraduate-thesis-f90da-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo; //fbdo stands for firebase data object

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool buttonState;
int buttonPin = 23;
bool signupOK = false;

//---------------------------------------------------------- VOID SETUP --------------------------------------------------------
void setup() {
  Serial.begin(115200);
  pinMode(pinBotao, INPUT);
  delay(1000);

  //---------------- ETHERNET -------------------------
  Serial.println("Begin Ethernet");
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  Ethernet.init(5);   // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet

  if (Ethernet.begin(mac)) { // Dynamic IP setup
    Serial.println("--------------");
    Serial.println("DHCP OK!");
  } else {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }

    IPAddress ip(MYIPADDR);
    IPAddress dns(MYDNS);
    IPAddress gw(MYGW);
    IPAddress sn(MYIPMASK);
    Ethernet.begin(mac, ip, dns, gw, sn);
    Serial.println("STATIC OK!");
  }
  delay(5000);

  Serial.print("Local IP : ");
  Serial.println(Ethernet.localIP());
  Serial.print("Subnet Mask : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Gateway IP : ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS Server : ");
  Serial.println(Ethernet.dnsServerIP());

  Serial.println("Ethernet Successfully Initialized");
  Serial.println("--------------");
//---------------- FIREBASE -------------------------

 if (client.connect(DATABASE_URL, 80)) {
    Serial.println("Connected to Firebase!");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection to Firebase failed");
  }
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

}

//---------------------------------------------------------- VOID LOOP ------------------------------------------------------------
void loop() {

  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH)
  {
    Serial.println("1");
  }
  else
  {
    Serial.println("0");
  }
  delay(10);
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    Firebase.RTDB.setBool(&fbdo, "button_state/button", buttonState);

  }
}
