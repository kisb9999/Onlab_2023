#ifndef AMBIENTSENSOR_H
#define AMBIENTSENSOR_H

#include "Arduino.h"
#include "Sensor.h"

class AmbientLightSensor : public Sensor{
  public:
    int readSensor(int input_pin);
    int getIntensity();
    String convertToPayloadData(String c, int strlength, int multiplier);
};

#endif
