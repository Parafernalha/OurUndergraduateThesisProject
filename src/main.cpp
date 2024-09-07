#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#define RELAY1_PIN  27
#define RELAY2_PIN  26
#define RELAY3_PIN  25
#define RELAY4_PIN  33
#define ENABLE_PIN  2  // Pin to control when the transmission is activated

HardwareSerial SerialPort(2); // Use UART1 for communication (RX: 16, TX: 17)

const int Enable = 2;
const int SlaveNumber = 2;
int id_ModuloRele;
String ModuloRele = "Modulo Relé";
bool StatusRelay1;
bool StatusRelay2;
bool StatusRelay3;
bool StatusRelay4;

String Zone1_On = "Abrindo Portão da garagem"; //Example of use
String Zone1_Off = "Fechando Portão da garagem"; //Example of use
String Zone2_On = "Abrindo a Valvula do gás"; //Example of use
String Zone2_Off = "Fechando a Valvula do gás"; //Example of use
String Zone3_On = "Abrindo a Janela do quarto das crianças"; //Example of use
String Zone3_Off = "Fechando a Janela do quarto das crianças"; //Example of use
String Zone4_On = "Acendendo a lampâda do escritório"; //Example of use
String Zone4_Off = "Apagando a lampâda do escritório"; //Example of use

void setup() 
{
  // Initialize serial communication
  Serial.begin(115200);  // For debug output via the serial monitor

  // Initialize relay pins
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);
  pinMode(Enable, OUTPUT);
  digitalWrite(Enable, LOW);

  // Initialize UART1 for communication
  SerialPort.begin(9600, SERIAL_8N1, 16, 17); // RX (GPIO 16), TX (GPIO 17)
  SerialPort.setTimeout(250);
  // Initialize enable pin
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); // Disable transmission initially
}

void loop()
{
  digitalWrite(Enable, LOW);
  if (SerialPort.available())  // Check for available data on the SerialPort (UART1)
    {
        Serial.println("ok serial port");
        id_ModuloRele = SerialPort.parseInt();
        if (id_ModuloRele == SlaveNumber) 
          {
            Serial.println("ok slave");
            String receivedMessage = SerialPort.readString();
            receivedMessage.trim();
            Serial.println(receivedMessage);
            if (receivedMessage == "TurnOn_RelayOne") 
               {
                digitalWrite(RELAY1_PIN, HIGH);
                StatusRelay1 = true;
                delay(100);
                StaticJsonDocument<200> doc;
                doc["dispositivo"] = ModuloRele;
                doc["Rele 1"] = Zone1_On;
                doc["Status"] = StatusRelay1;
                String json;
                serializeJson(doc, json);
                Serial.println(json);
                digitalWrite(Enable, HIGH);
                SerialPort.print(json);
                SerialPort.flush();
                delay(20);
                digitalWrite(Enable, LOW);
               }
                if (receivedMessage == "TurnOff_RelayOne") 
               {
                digitalWrite(RELAY1_PIN, LOW);
                StatusRelay1 = false;
                delay(100);
                StaticJsonDocument<200> doc;
                doc["dispositivo"] = ModuloRele;
                doc["Rele 1"] = Zone1_Off;
                doc["Status"] = StatusRelay1;
                String json;
                serializeJson(doc, json);
                Serial.println(json);
                digitalWrite(Enable, HIGH);
                SerialPort.print(json);
                SerialPort.flush();
                delay(20);
                digitalWrite(Enable, LOW);
               }
                if (receivedMessage == "TurnOn_RelayTwo") 
               {
                digitalWrite(RELAY2_PIN, HIGH);
                StatusRelay2 = true;
                delay(100);
                StaticJsonDocument<200> doc;
                doc["dispositivo"] = ModuloRele;
                doc["Rele 2"] = Zone2_On;
                doc["Status"] = StatusRelay2;
                String json;
                serializeJson(doc, json);
                Serial.println(json);
                digitalWrite(Enable, HIGH);
                SerialPort.print(json);
                SerialPort.flush();
                delay(20);
                digitalWrite(Enable, LOW);
               }
                if (receivedMessage == "TurnOff_RelayTwo") 
               {
                digitalWrite(RELAY2_PIN, LOW);
                StatusRelay2 = false;
                delay(100);
                StaticJsonDocument<200> doc;
                doc["dispositivo"] = ModuloRele;
                doc["Rele 2"] = Zone2_Off;
                doc["Status"] = StatusRelay2;
                String json;
                serializeJson(doc, json);
                Serial.println(json);
                digitalWrite(Enable, HIGH);
                SerialPort.print(json);
                SerialPort.flush();
                delay(20);
                digitalWrite(Enable, LOW);
               }
                if (receivedMessage == "TurnOn_RelayThree") 
               {
                digitalWrite(RELAY3_PIN, HIGH);
                StatusRelay3 = true;
                delay(100);
                StaticJsonDocument<200> doc;
                doc["dispositivo"] = ModuloRele;
                doc["Rele 3"] = Zone3_On;
                doc["Status"] = StatusRelay3;
                String json;
                serializeJson(doc, json);
                Serial.println(json);
                digitalWrite(Enable, HIGH);
                SerialPort.print(json);
                SerialPort.flush();
                delay(20);
                digitalWrite(Enable, LOW);
               }
                if (receivedMessage == "TurnOff_RelayThree") 
               {
                digitalWrite(RELAY3_PIN, LOW);
                StatusRelay3 = false;
                delay(100);
                StaticJsonDocument<200> doc;
                doc["dispositivo"] = ModuloRele;
                doc["Rele 3"] = Zone3_Off;
                doc["Status"] = StatusRelay3;
                String json;
                serializeJson(doc, json);
                Serial.println(json);
                digitalWrite(Enable, HIGH);
                SerialPort.print(json);
                SerialPort.flush();
                delay(20);
                digitalWrite(Enable, LOW);
               }
                if (receivedMessage == "TurnOn_RelayFour") 
               {
                digitalWrite(RELAY4_PIN, HIGH);
                StatusRelay4 = true;
                delay(100);
                StaticJsonDocument<200> doc;
                doc["dispositivo"] = ModuloRele;
                doc["Rele 4"] = Zone4_On;
                doc["Status"] = StatusRelay4;
                String json;
                serializeJson(doc, json);
                Serial.println(json);
                digitalWrite(Enable, HIGH);
                SerialPort.print(json);
                SerialPort.flush();
                delay(20);
                digitalWrite(Enable, LOW);
               }
                if (receivedMessage == "TurnOff_RelayFour") 
               {
                digitalWrite(RELAY4_PIN, LOW);
                StatusRelay4 = false;
                delay(100);
                StaticJsonDocument<200> doc;
                doc["dispositivo"] = ModuloRele;
                doc["Rele 4"] = Zone4_Off;
                doc["Status"] = StatusRelay4;
                String json;
                serializeJson(doc, json);
                Serial.println(json);
                digitalWrite(Enable, HIGH);
                SerialPort.print(json);
                SerialPort.flush();
                delay(20);
                digitalWrite(Enable, LOW);
               }
          } 
     }                  
}
