 const float lowTempTreshold = 20.00;
 const float highTempTreshold = 24.00;
 const int lowLuxTreshold = 30;
 const int highLuxTreshold = 90;
 float actualTemperature, lastTemperature;
 int actualLightIntensity,lastLightIntensity;  

void setup() {
 //HC-05 BT modul is connected to the Tx/RX PIN1,2 of the Leonardo, which is handled by Serial1
 Serial1.begin(9600); // Safe communication rate of the Bluetooth module

 actualTemperature  = 21; 
 actualLightIntensity = 210;
 lastTemperature  = 10; 
 lastLightIntensity = 10;
 
}

void loop() {
  /*
  float actualTemperature    = temp_sensor.getTemperature();
    int actualLightIntensity = am_light_sensor.getIntensity(); //Ambient Light Intensity Outside 
  */

  if (actualTemperature != lastTemperature) {
    Serial1.println("----------------------------------------------------------");
    Serial.print("Temperature is [C]:");Serial.println(actualTemperature);
    Serial1.print("Temperature is [C]:");Serial1.println(actualTemperature);
    Serial.print("Tresholds [C]: [");Serial.print(lowTempTreshold);
    Serial.print(";");Serial.print(highTempTreshold);Serial.println("]");
    Serial1.print("Tresholds [C]: [");Serial1.print(lowTempTreshold);
    Serial1.print(";");Serial1.print(highTempTreshold);Serial1.println("]");
    Serial1.flush();
    
    if (actualTemperature > highTempTreshold){
      Serial.println("AirCo Cooler ON, too warm here");  // For displaying on local Serial port 
      Serial1.println("AirCo Cooler ON , too warm here"); // Sending data to the HC-05 BT module
      Serial1.flush();
    } else {
      if (actualTemperature < lowTempTreshold) {
        Serial.println("AirCo Heater ON, too cold here");  // For displaying on local Serial port 
        Serial1.println("AirCo Heater ON, too cold here"); // Sending data to the HC-05 BT module
        Serial1.flush();
      } else {
        Serial.println("AirCo OFF, convenient here");  // For displaying on local Serial port 
        Serial1.println("AirCo OFF , convenient here"); // Sending data to the HC-05 BT module
        Serial1.flush();
      }  
    }  
  }
  
  if (actualLightIntensity != lastLightIntensity){
    Serial1.println("----------------------------------------------------------");    
    Serial.print("Light intensity is [Lux]:");Serial.println(actualLightIntensity);
    Serial1.print("Light Intensity is [Lux]:");Serial1.println(actualLightIntensity);
    Serial.print("Tresholds [Lux]: [");Serial.print(lowLuxTreshold);
    Serial.print(";");Serial.print(highLuxTreshold);Serial.println("]");
    Serial1.print("Tresholds [Lux]: [");Serial1.print(lowLuxTreshold);
    Serial1.print(";");Serial1.print(highLuxTreshold);Serial1.println("]");
    Serial1.flush();
    
    if (actualLightIntensity > highLuxTreshold ) {
      Serial.println("Shutter UP");  // For displaying on local Serial port 
      Serial.println("Lighting OFF");  // For displaying on local Serial port 
      Serial1.println("Shutter UP"); // Sending data to the HC-05 BT module
      Serial1.println("Lighting OFF"); // Sending data to the HC-05 BT module 
      Serial1.flush();
    } else {
      if (actualLightIntensity < lowLuxTreshold ) {
        Serial.println("SHUTTER DOWN");  // For displaying on local Serial port 
        Serial1.println("SHUTTER DOWN"); // Sending data to the HC-05 BT module
        Serial.println("Lighting ON, it's evening");  // For displaying on local Serial port 
        Serial1.println("Lighting ON , it's evening"); // Sending data to the HC-05 BT module
        Serial1.flush();
      } else {
        Serial.println("Light ON but not evenning yet");  // For displaying on local Serial port 
        Serial1.println("Light ON but not evenning yet"); // Sending data to the HC-05 BT module
        Serial1.flush();
      } 
    }
  }
  
  
  // See if there is a message from the devices through the Bluetooth connection
  // Display this locally
  String msg = "";
  while (Serial1.available() > 0){ // Checks whether data is comming from the serial port
    char s = Serial1.read(); // Reads the data from the serial port
    Serial.print(s);
    msg += s;
  }
  Serial.println();

  lastTemperature = actualTemperature ;
  if ((msg[0]=='T') || (msg[0]=='t')) {
    msg.remove(0,1);
    actualTemperature  = msg.toFloat(); 
  }

  lastLightIntensity = actualLightIntensity;
  if ((msg[0]=='L') || (msg[0]=='l')) {
    msg.remove(0,1);
    actualLightIntensity = msg.toInt(); 
  }
  
  
  delay(2000);
}
