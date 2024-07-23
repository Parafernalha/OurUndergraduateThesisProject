#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define SEALEVELPRESSURE_HPA (1013.25)

HardwareSerial SerialPort(2); // Serial 2 Instance
Adafruit_BME680 bme; // BME680 Instance

const int Enable = 2;
const int SlaveNumber = 1;
int Slave;

void setup() 
{
  Serial.begin(115200);
  SerialPort.begin(115200, SERIAL_8N1, 16, 17);
  pinMode(Enable, OUTPUT);
  digitalWrite(Enable, LOW);
    if (!bme.begin()) 
  {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}
void loop() 
{
  digitalWrite(Enable, LOW);
  if (SerialPort.available()){
    Slave = SerialPort.parseInt();
       if(Slave == SlaveNumber){
            String receivedMessage = SerialPort.readString();  
            Serial.println(receivedMessage);
              if(receivedMessage == "Temperatura"){
                  // Tell BME680 to begin measurement.
                  bme.beginReading();
                  delay(100);
                  if (!bme.performReading()){
                    Serial.println("Failed to perform reading :(");
                    return;}
                  float temperature = bme.temperature;
                  Serial.println(temperature);
                  // Enable transmission mode
                  digitalWrite(Enable, HIGH);
                  SerialPort.print("Temperature = ");
                  SerialPort.print(temperature);
                  SerialPort.print(" ºC");
                  SerialPort.flush();
                  // Switch to receiving mode
                  digitalWrite(Enable, LOW);}
                
                  else if(receivedMessage == "Humidade"){
                    // Tell BME680 to begin measurement.
                  bme.beginReading();
                  delay(100);
                  if (!bme.performReading()){
                    Serial.println("Failed to perform reading :(");
                    return;}
                  float humidity = bme.humidity;
                  Serial.println(humidity);
                  // Enable transmission mode
                  digitalWrite(Enable, HIGH);
                  SerialPort.print("Humidity =  ");
                  SerialPort.print(humidity);
                  SerialPort.print(" %");
                  SerialPort.flush();
                  // Switch to receiving mode
                  digitalWrite(Enable, LOW);}

                   else if(receivedMessage == "Gases"){
                    // Tell BME680 to begin measurement.
                  bme.beginReading();
                  delay(100);
                  if (!bme.performReading()){
                    Serial.println("Failed to perform reading :(");
                    return;}
                  float Gas = bme.gas_resistance;
                  Serial.println(Gas);
                  // Enable transmission mode
                  digitalWrite(Enable, HIGH);
                  SerialPort.print("Gas = ");
                  SerialPort.print(Gas);
                  SerialPort.print(" KOhms");
                  SerialPort.flush();
                  // Switch to receiving mode
                  digitalWrite(Enable, LOW);}

                  else if(receivedMessage == "Pressao"){
                    // Tell BME680 to begin measurement.
                  bme.beginReading();
                  delay(100);
                  if (!bme.performReading()){
                    Serial.println("Failed to perform reading :(");
                    return;}
                  float Pre = bme.pressure;
                  Serial.println(Pre);
                  // Enable transmission mode
                  digitalWrite(Enable, HIGH);
                  SerialPort.print("Pressure: ");
                  SerialPort.print(Pre);
                  SerialPort.print(" hPa");
                  SerialPort.flush();
                  // Switch to receiving mode
                  digitalWrite(Enable, LOW);}

                  else if(receivedMessage = "TemperaturaHumidadeGasesPressao"){
                  // Tell BME680 to begin measurement.
                  bme.beginReading();
                  delay(100);
                  if (!bme.performReading()){
                    Serial.println("Failed to perform reading :(");
                    return;}
                  float temperature = bme.temperature;
                  float humidity = bme.humidity;
                  float Gas = bme.gas_resistance;
                  float Pre = bme.pressure;
                  // Enable transmission mode
                  digitalWrite(Enable, HIGH);
                  SerialPort.print("Temperature = ");
                  SerialPort.print(temperature);
                  SerialPort.print(" ºC");
                  SerialPort.print("Humidity =  ");
                  SerialPort.print(humidity);
                  SerialPort.print(" %");
                  SerialPort.print("Pressure: ");
                  SerialPort.print(Pre);
                  SerialPort.print(" hPa");
                  SerialPort.print("Gas = ");
                  SerialPort.print(Gas);
                  SerialPort.print(" KOhms");
                  SerialPort.flush();
                  // Switch to receiving mode
                  digitalWrite(Enable, LOW);
        }
      }             
   } 
}