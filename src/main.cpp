#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <ArduinoJson.h>

#define SEALEVELPRESSURE_HPA (1013.25)

HardwareSerial SerialPort(2); // Serial 2 Instance
Adafruit_BME680 bme; // BME680 Instance

const int Enable = 2;
const int SlaveNumber = 1;
int Slave;
const int DebugLED = 14;
bool sensorConnected = true;

void sendSensorData(const char* key, float value) {
  StaticJsonDocument<200> doc;
  doc[key] = value;
  String json;
  serializeJson(doc, json);

  digitalWrite(Enable, HIGH);
  SerialPort.print(json);
  SerialPort.flush();
  digitalWrite(Enable, LOW);
}

void setup() {
  Serial.begin(115200);
  SerialPort.begin(115200, SERIAL_8N1, 16, 17);
  pinMode(Enable, OUTPUT);
  pinMode(DebugLED, OUTPUT);
  digitalWrite(Enable, LOW);
  digitalWrite(DebugLED, LOW);

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    digitalWrite(DebugLED, HIGH);
    sensorConnected = false;
  } else {
    sensorConnected = true;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void checkSensorConnection() {
  if (!sensorConnected || !bme.performReading()) {
    Serial.println("Failed to perform reading or sensor disconnected :(");
    digitalWrite(DebugLED, HIGH); // Acende o LED em caso de falha na leitura

    // Tenta reinicializar o sensor
    if (!bme.begin()) {
      Serial.println(F("Could not reinitialize the BME680 sensor, check wiring!"));
      sensorConnected = false;
    } else {
      Serial.println(F("BME680 sensor reinitialized successfully."));
      digitalWrite(DebugLED, LOW);
      sensorConnected = true;
    }
  } else {
    digitalWrite(DebugLED, LOW); // Desliga o LED se a leitura for bem-sucedida
    sensorConnected = true;
  }
}

void loop() {
  checkSensorConnection();

  if (!sensorConnected) {
    delay(1000); // Atraso para evitar tentativas rápidas de reconexão
    return;
  }

  digitalWrite(Enable, LOW);
  if (SerialPort.available()) {
    Slave = SerialPort.parseInt();
    if (Slave == SlaveNumber) {
      String receivedMessage = SerialPort.readString();
      receivedMessage.trim();
      Serial.println(receivedMessage);

      if (receivedMessage == "Temperatura") {
        bme.beginReading();
        delay(100);
        if (!bme.performReading()) {
          Serial.println("Failed to perform reading :(");
          digitalWrite(DebugLED, HIGH);
          return;
        }
        sendSensorData("Temperature", bme.temperature);
      } 
      else if (receivedMessage == "Humidade") {
        bme.beginReading();
        delay(100);
        if (!bme.performReading()) {
          Serial.println("Failed to perform reading :(");
          digitalWrite(DebugLED, HIGH);
          return;
        }
        sendSensorData("Humidity", bme.humidity);
      } 
      else if (receivedMessage == "Gases") {
        bme.beginReading();
        delay(100);
        if (!bme.performReading()) {
          Serial.println("Failed to perform reading :(");
          digitalWrite(DebugLED, HIGH);
          return;
        }
        sendSensorData("Gas", bme.gas_resistance);
      } 
      else if (receivedMessage == "Pressao") {
        bme.beginReading();
        delay(100);
        if (!bme.performReading()) {
          Serial.println("Failed to perform reading :(");
          digitalWrite(DebugLED, HIGH);
          return;
        }
        sendSensorData("Pressure", bme.pressure);
      } 
      else if (receivedMessage == "TemperaturaHumidadeGasesPressao") {
        bme.beginReading();
        delay(100);
        if(!bme.performReading()){
          Serial.println("Failed to perform reading :(");
          digitalWrite(DebugLED, HIGH);
          return;
        }

        StaticJsonDocument<200> doc;
        doc["Temperature"] = bme.temperature;
        doc["Humidity"] = bme.humidity;
        doc["Gas"] = bme.gas_resistance;
        doc["Pressure"] = bme.pressure;
        String json;
        serializeJson(doc, json);

        digitalWrite(Enable, HIGH);
        SerialPort.print(json);
        SerialPort.flush();
        digitalWrite(Enable, LOW);     
      }
    }
  }
  delay(500); // Pequeno atraso para não sobrecarregar o sensor e a comunicação
}