#include <Wire.h>
#include <Arduino.h>
#include "MA510.h"
#include "TempSensor.h"
#include "Sensor.h"
#include "AmbientLightSensor.h"

// Global variables
 String imei;
 String ccid;
 String ip;

 MA510 modem; 

void setup() {
  // initialize serial communications at 115200 bps
  // SERIAL - display port of the IDE
  // SERIAL1- UART connected to the MA510 modem
  Serial.begin(115200);
  Serial1.begin(115200);
  Wire.begin();

  // giving 5 sec for the Serial port to be available
  // waiting for port being available
  // while (!Serial|| millis() < 5000);

  for(int i = 10; i>0; i--){
    delay(1000);
    Serial.print("Countdown: ");
    Serial.println(i);
  }

  Serial.println("The program has started!");

  /*while (!barometricSensor.begin()) {
    Serial.println("MS5637 Init failed! Retry in 5 seconds...");
    delay(5000);
  }
  barometricSensor.setResolution();*/

  //MA510 modem; 

  // tennek be egy timeoutot parameterkent, pl. max ciklusszam hogy ne keruljon vegtelen ciklusba ...
  modem.turnOn();
  while(!modem.isOn()){
    modem.turnOn();
    delay(1000);
  }
  
  imei = modem.getIMEI();


  ccid = modem.getCCID();

  
  ip = modem.getIP("cloud.e-iot.info", 2000);
  while(ip=="ERROR"){
    if(ip == "ERROR"){
      Serial.println("An error occured! Retrying IP...");
      delay(2000);
      ip = modem.getIP("cloud.e-iot.info", 2000);
    }
  }

  delay(500);

  Serial.print("Server IP: ");
  Serial.print("|");Serial.print(ip);Serial.println("|");
  Serial.println("IP done, so start loop in 10 secs");
  delay(10000);

  // Setting fan speed to Zero
  analogWrite(9,0); // Seeting PWM on PIN9 to zero to stop the fan


  
}

void loop() {
  // put your main code here, to run repeatedly:
  AmbientLightSensor am_light_sensor;
  Serial.print("Light intensity: ");
  Serial.println(am_light_sensor.getIntensity());

  TemperatureSensor temp_sensor;
  Serial.print("Temperature: ");
  Serial.println(temp_sensor.getTemperature());

  Serial.print("|");
  Serial.print(imei);
  Serial.println("|");

  Sensor my_sensor;
  String myJSON = my_sensor.createJSON(imei, "message reference", "49.4185,111.1180");
  Serial.print("|");
  Serial.print(myJSON);
  Serial.println("|");


  modem.sendData(myJSON,ip,"41234");
  delay(120000);
  
}
