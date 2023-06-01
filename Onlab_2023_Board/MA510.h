#ifndef MA510_H
#define MA510_H

#include "Arduino.h"

class MA510{
  public:
    //MA510();
    void turnOn();
    void turnOff();
    bool isOn();
    
    String getIMEI();
    String getIP(char* domain, unsigned int timeout);
    String getCCID();
    
    void sendCommand(String command);
    void sendData(String telegram,String remote_ip,String port);
};

#endif
