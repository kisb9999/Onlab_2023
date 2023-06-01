#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

#include "Arduino.h"
#include "Sensor.h"

class TemperatureSensor : public Sensor{
  public:
    float readSensor();
    float getTemperature();
    String convertToPayloadData(String c, int strlength, int multiplier);
};

#endif
