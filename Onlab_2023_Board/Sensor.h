#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"

class Sensor{
  public:
    String createJSON(String myimei, String mymsgRef, String mygpsData);
};
#endif
