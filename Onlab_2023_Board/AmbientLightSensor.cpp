#include "Arduino.h"
#include "AmbientLightSensor.h"
#include "stddef.h"


int AmbientLightSensor::readSensor(int input_pin){
  return analogRead(input_pin);  
}

int AmbientLightSensor::getIntensity(){
  int raw_data = readSensor(A2);
  return (int)raw_data*0.263; //converts to lux
}

String AmbientLightSensor::convertToPayloadData(String c, int strlength, int multiplier){
  int Intensity;
  String IntensityString;
  Intensity = int(AmbientLightSensor::getIntensity()*multiplier);
  IntensityString = String(Intensity);
  
  // Example 192 convertToPayloadData("A", 6, 100) results in "A019200"
  while (IntensityString.length()<strlength) IntensityString = "0"+ IntensityString;
  IntensityString = c+ IntensityString;
  
  return IntensityString;



  
}
