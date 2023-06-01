#include "Arduino.h"
#include "stddef.h"
#include "MA510.h"

void MA510::turnOn(){
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH); 
  delay(1000);
  digitalWrite(12, LOW);  
  delay(2000);    
}

void MA510::turnOff(){
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH); 
  delay(3000);
  digitalWrite(12, LOW);
}

bool MA510::isOn(){
  Serial1.println("AT");
  delay(1000);
  String s = "";
  while(Serial1.available()>0){
    if(Serial1.available() > 0){
      s+=(char)Serial1.read();   
    }
  }
  Serial.print(s);
  if(s.indexOf("OK") == -1)
    return false;
  return true;
}

String MA510::getIMEI(){
  String imei = "";
  Serial1.println("AT+CGSN");
  delay(1000);
  while(Serial1.available() > 0){
    if(Serial1.available() > 0){
      imei += (char)Serial1.read();
    }
  }
  int idx = imei.indexOf("AT+CGSN");
  String final = imei.substring(idx+10, idx+25);

  Serial.print("IMEI: |");
  Serial.print(final);
  Serial.println("|");
  
  return final;
}

String MA510::getIP(char* domain, unsigned int timeout){
  String ip = "";
  char answer[100];

  memset(answer, '\0', 100);

  //to empty the input buffer
  while(Serial1.available() > 0){
    Serial1.read();  
  }    
  delay(100);

  //we need this command to get the IP --> we do not needs its response
  Serial1.println("AT+MIPCALL=1");
  delay(2000);

  //empty the input buffer
  while(Serial1.available() > 0){
    Serial1.read();
  }
  delay(100);


  char command_buffer[50];
  sprintf(command_buffer, "AT+MIPDNS=\"%s\"", domain);
  Serial1.println(command_buffer);
  
  unsigned int original_time = millis();
  int ctr = 0;
  while((millis()-original_time) < timeout){
    if(Serial1.available() != 0){
      answer[ctr] = Serial1.read();
      ctr++;
    }
  }
  
  //empty the input buffer in case more data came in
  while(Serial1.available() > 0){
    Serial1.read();
  }


  delay(1000);
  //Close connection
  Serial1.println("AT+MIPCALL=0");
  delay(500);
  Serial.println("Modem disconnected");

  char expected_answer[50];
  memset(expected_answer, '\0', 100);
  
  sprintf(expected_answer, "+MIPDNS: \"%s\",", domain);
  String expected_answer_string = (String)expected_answer;

  String response = (String)answer;
  int position_of_string = response.indexOf(expected_answer_string);

  
  if(position_of_string == -1)
    return "ERROR";

  int char_ctr = position_of_string + expected_answer_string.length();
  
  while(response.charAt(char_ctr) != '\r' && char_ctr < strlen(answer)){
    if(response.charAt(char_ctr) != '\r'){
      ip += (char)response.charAt(char_ctr);
    }
      char_ctr++; 
  }
  //ip+='\0';
  return ip;
}

String MA510::getCCID(){
  String sim = "";
  Serial1.println("AT+CCID");
  delay(1000);
  while(Serial1.available() > 0){
    if(Serial1.available() > 0){
      sim += (char)Serial1.read();
    }
  }
    
  int idx = sim.indexOf("AT+CCID");
  String final = sim.substring(idx+17, idx+37);

  Serial.print("CCID: |");
  Serial.print(final);
  Serial.println("|"); 
  
  return final;
}
    
void MA510::sendCommand(String command){
  String reply = "";
  Serial.print("SentCommand:");Serial.println(command);
  Serial1.println(command);
  delay(500);
  while(Serial1.available() > 0){
      reply += (char)Serial1.read();
  }
  Serial.print(reply);
}
    
void MA510::sendData(String telegram,String remote_ip,String port){
    int i;
    String hexChar;

  Serial.println(F("------------------------------------------------------------------------------------------------------------------------------------------------"));
  Serial.println(F("Sending in progress ....."));
  // Create a Wireless Link
  delay(3000); // Wait for the modem to be absolutely ready for sending
  sendCommand("AT+MIPCALL=1");
  Serial.println();
  delay(2000);
  
  // Open a Socket (UDP)
  // Endrich cloud Connection socket cloud.e-iot.info:41234 
  Serial.println(F("Open a Socket"));
  delay(2000);
  // Format of the command : sendCommand("AT+MIPOPEN=1,41234,\"79.120.218.116\",41234,1");
  sendCommand("AT+MIPOPEN=1,"+port+",\""+remote_ip+"\","+port+",1");
  
  delay(2000);
  Serial.println();

  
  // Sending HEX encoded telegram
  // Endrich JSON telegram format :
  // {"IMEI": "867420040658082" , "msgref" : "3BIGModDemo FW:4.0 2022-01-14 ICCID:89882280666020359042 IMEI:867420040658082", "payload" : "T002658|A002388|L1|F0618|V0473|P102100|M-07049|H0|R000000|G49.4185,11.1180", "gpsdata" : "49.4185,11.1180"}
  Serial.println("Send telegram");
  Serial1.print("AT+MIPSEND=1,\"");  
  Serial.print("AT+MIPSEND=1,\"");  
  
   
  for (i=0; i< telegram.length();i++) {
     hexChar = String(telegram[i], HEX);
     do {} while (Serial1.availableForWrite()<60);
     Serial1.print(hexChar);
     Serial.print(telegram[i]);   
  }

  Serial1.println("\"");
  Serial.println("\"");
  delay(2000);
  
  // Push Data into Protocol Stack
  sendCommand("AT+MIPPUSH=1");
  delay(3000);
  //while (Serial1.available()<1) {};
  Serial.println();
  
  // Close the Socket   
  sendCommand("AT+MIPCLOSE=1");
  delay(300);
  Serial.println();
  
  // Close the GSM Link
  sendCommand("AT+MIPCALL=0");
  delay(300);  
  Serial.println();
  Serial.println(F("------------------------------------------------------------------------------------------------------------------------------------------------"));
}
