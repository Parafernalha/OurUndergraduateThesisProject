#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

HardwareSerial SerialPort(2);

const int Enable = 2;
const int SlaveID = 1;
String Temp = "Temperatura";
String Humi = "Humidade";
String Gas = "Gases";
String Pre = "Pressao";

void setup() 
{
  Serial.begin(115200);
  SerialPort.begin(115200, SERIAL_8N1, 16, 17);
  pinMode(Enable, OUTPUT);
  digitalWrite(Enable, LOW);
}

void loop() 
{
  delay(500);

  // Enable transmission mode
  digitalWrite(Enable, HIGH);

  // Send a test message to the slave
  SerialPort.print(SlaveID);
  SerialPort.print(Temp);
  SerialPort.print(Humi);
  SerialPort.print(Gas);
  SerialPort.print(Pre);
  SerialPort.flush();

  // Switch to receiving mode
  digitalWrite(Enable, LOW);

  // Wait for a response with a timeout
  long startTime = millis();
  while (!SerialPort.available()) {
    if (millis() - startTime > 2000) { // 2-second timeout
      Serial.println("Timeout waiting for response");
      return;
    }
  }

  // Read and print the response from the slave
  if (SerialPort.available()) {
    String response = SerialPort.readString();
    Serial.println(response);
  }
  delay(1000);
}