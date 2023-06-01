#include "Arduino.h"
#include "Sensor.h"
#include "stddef.h"
#include "TempSensor.h"
#include "AmbientLightSensor.h"

String Sensor::createJSON(String myimei, String mymsgRef, String mygpsData){
  AmbientLightSensor am_light_sensor;
  TemperatureSensor temp_sensor;
  String myJSON="";  
  String myTEMP = temp_sensor.convertToPayloadData("T", 6, 100);
  Serial.print("|");Serial.print(myTEMP);Serial.println("|");
  myJSON = "{\"IMEI\": \""+myimei+"\" , \"msgref\" : \""+mymsgRef+"\", \"payload\" :\"|"+am_light_sensor.convertToPayloadData("A", 6, 100)+"|"+myTEMP+"|\",\"gpsdata\" :\""+mygpsData+"\"}";
  return myJSON;
}
