#include <Arduino.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <SPI.h>

const int Enable = 2;  // RS485 control pin
const int SlaveID = 2;


void setup()
{
  Serial.begin(9600);
  pinMode(Enable, OUTPUT);
  digitalWrite(Enable, LOW);
}
 

void loop() 
{
  delay(100);
  digitalWrite(Enable, HIGH); //Enable transmission
  Serial.println(String(SlaveID) + "TurnOn_RelayOne");
  delay(1000);
  Serial.println(String(SlaveID) + "TurnOff_RelayOne");
  Serial.flush();
  delay(1000);
  digitalWrite(Enable, LOW); //Disable Transmission


  long startTime = millis();
  while (!Serial.available()) 
  {
    if (millis() - startTime > 2000) 
    { // 2-second timeout
      Serial.println("Timeout waiting for response");
      return;
    }
  }
  // Read and print the response from the slave
  if (Serial.available()) 
  {
    String response = Serial.readString();
    Serial.println(response);
  }
  delay(1000);
}