#include "Arduino.h"
#include "TempSensor.h"
#include "stddef.h"
#include "SparkFun_MS5637_Arduino_Library.h"

float TemperatureSensor::readSensor(){
  // TE I2C Pressure/altitude and temperature sensor library
  MS5637 barometricSensor;
  barometricSensor.begin();
  return barometricSensor.getTemperature();
}

float TemperatureSensor::getTemperature(){
  return TemperatureSensor::readSensor();
}

String TemperatureSensor::convertToPayloadData(String c, int strlength, int multiplier){
  int Temperature;
  String TemperatureString;
  Temperature = int(TemperatureSensor::getTemperature()*multiplier);
  TemperatureString = String(Temperature);

  // Example 25,44 degC convertToPayloadData("T", 6, 100) results in "T002544"
  while (TemperatureString.length()<strlength) TemperatureString = "0"+ TemperatureString;
  TemperatureString = c+ TemperatureString;
  
  return TemperatureString;
}
