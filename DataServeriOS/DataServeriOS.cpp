/*
 Arduino JSON Data Server for iOS
 created by M. Heeres, March 5 2012
 Copyright 2012 M. Heeres
 
 Library belonging to the Arduino Control app for iOS.
 */

#include "DataServeriOS.h"

#define bufferMax 128
int bufferSize;
char buffer[bufferMax];
//-- Commands and parameters size --
char pass[15];    // Limits length to 15 characters
char cmd[15];
char port[15];
char action[15];

int *_PWMPorts;
int _PWMValues[8];
int *_CFPorts;
int *_CRPorts;

EthernetClient _client;

DataServeriOS::DataServeriOS(EthernetServer *server, char *pass, boolean serial, char *Arduino, void (*substitudeLoop)(void),int pwmports[],byte nports,int cfports[],byte ncfports,void(*customfunction)(int),char*(*customdigitalreturn)(int),int crports[],byte ncrports,char*(*customanalogreturn)(int))
{
  _serial = serial;
  
  _nPWMports = nports;
  if (nports > 0) {
    _pwm = true;
    _PWMPorts = pwmports;
  }
  else _pwm = false;
  
  _nCRports = ncrports;
  if (ncrports > 0) {
    _cr = true;
    _CRPorts = crports;
    _returnAnalogForPort = customanalogreturn;
  }
  else _cr = false;
  
  _nCFports = ncfports;
  if (ncfports > 0) {
    _cf = true;
    _CFPorts = cfports;
    _function = customfunction;
    _returnDigitalForPort = customdigitalreturn;
  }
  else _cf = false;

  _server = server;
  _pass = pass;
  _Arduino = Arduino;
  _substitudeLoop = substitudeLoop;

  SetupPorts();
}

void DataServeriOS::SetupPorts()
{
  for (int i=0;i<=9;i++) {
    pinMode(i,OUTPUT);
  }
  if (strcmp(_Arduino,"MEGA") == 0) {
    for (int i=14;i<=53;i++) {
      pinMode(i,OUTPUT);
    }
  }
}

void DataServeriOS::loop()
{
  _client = _server->available();
  if (_client)
  {
    WaitForRequest();
    ParseReceivedRequest();
    PerformRequestedCommand();
    _client.flush();
    _client.stop();
  }
  else {
    _client.stop();
    _substitudeLoop();
  }
}

void DataServeriOS::WaitForRequest()
{
  bufferSize = 0;
  while (_client.connected()) {
    if (_client.available()) {
      char c = _client.read();
      if (c == '\n')
      {
        _client.println("HTTP/1.1 200 OK\nContent-Type: application/json\n\n");
        break;
      }
      else
        if (bufferSize < bufferMax) {
          buffer[bufferSize++] = c;
        }
        else
          break;
    }
  }
}

void DataServeriOS::ParseReceivedRequest()
{
  //Received buffer contains "GET ?pass= &cmd= &action= HTTP/1.1".  Break it up.
  char* en1;
  char* en2;
  char* en3;
  char* space2;
  char* qm;

  qm      = strstr(buffer, "?pass=") + 6; // Look for questionmark and pass
  en1     = strstr(qm, "&cmd=") + 5; // Look for first en
  en2     = strstr(en1, "&port=") + 6; // second en
  en3     = strstr(en2, "&action=") + 8; // third en
  space2  = strstr(en2, " ") + 1; // space after second en (in case there is no third en)
  if (en3 > space2) en3=en2;

  // strncpy does not automatically add terminating zero, but strncat does! So start with blank string and concatenate.
  pass[0] = 0;
  cmd[0] = 0;
  port[0] = 0;
  action[0] = 0;
  strncat(pass, qm, en1-qm-5);
  strncat(cmd, en1, en2-en1-6);
  strncat(port, en2, en3-en2-8);
  strncat(action, en3, space2-en3-1);
  
  if (_serial) {
    if (strcmp(_pass,pass)==0) Serial.println("pass=correct");
    else Serial.println("pass=invallid");
    Serial.print("cmd="); 
    Serial.println(cmd);
    Serial.print("port="); 
    Serial.println(port);
    Serial.print("action="); 
    Serial.println(action);
  }
}

int DataServeriOS::SetValueForPWMPort(int value, int PWMPort)
{
  for (int i=0;i<_nPWMports;i++) {
    if (_PWMPorts[i] == PWMPort) {
      _PWMValues[i] = value;
    }
  }
}

int DataServeriOS::ValueForPWMPort(int PWMport)
{
  for (int i=0;i<_nPWMports;i++) {
    if (_PWMPorts[i] == PWMport) {
      return _PWMValues[i];
    }
  }
}

boolean DataServeriOS::InArray(int array[],byte n,int value)
{
  for (int i=0;i<n;i++) {
    if (array[i] == value) {
      return true;
    }
  }
  return false;
}

void DataServeriOS::JSON_send(char* command)
{
  // Send requested information
  String bffr = "{\"arduino_io\":{";
  char temp[128];
  // If used on a UNO or equivalent
  if (strcmp(_Arduino, "UNO")==0) {
    // ReadAll
    if (strcmp(command,"ReadAll")==0) {
      // Analog
      bffr+="\"an\":{";
      for (int i=0;i<=4;i++) {
        if (_cr && InArray(_CRPorts,_nCRports,i)) sprintf(temp,"\"a%d\":\"%s\",",i,_returnAnalogForPort(i));
        else sprintf(temp,"\"a%d\":%d,",i,analogRead(i));
        bffr+=String(temp);
      }
      if (_cr && InArray(_CRPorts,_nCRports,5)) sprintf(temp,"\"a%d\":\"%s\"",5,_returnAnalogForPort(5));
      else sprintf(temp,"\"a%d\":%d}",5,analogRead(5));
      bffr+=String(temp);
      
      if (_serial) Serial.print(bffr);
      _server->print(bffr);
      
      // Digital
      bffr=",\"di\":{";
      for (int i=0;i<=8;i++) {
        if (_pwm && InArray(_PWMPorts,_nPWMports,i)) sprintf(temp,"\"d%d\":\"p%d\",",i,ValueForPWMPort(i));
        else if (_cf && InArray(_CFPorts,_nCFports,i)) sprintf(temp,"\"d%d\":%s,",i,_returnDigitalForPort(i));
        else sprintf(temp,"\"d%d\":%d,",i,digitalRead(i)); 
        bffr+=String(temp);
      }
      if (_pwm && InArray(_PWMPorts,_nPWMports,9)) sprintf(temp,"\"d%d\":\"p%d\"}",9,ValueForPWMPort(9));
      else if (_cf && InArray(_CFPorts,_nCFports,9)) sprintf(temp,"\"d%d\":%s,",9,_returnDigitalForPort(9));
      else sprintf(temp,"\"d%d\":%d}",9,digitalRead(9));
      bffr+=String(temp);
      bffr+="}}";
      if (_serial) Serial.println(bffr);
      _server->println(bffr);
    }

    // PWMWrite
    else if (strcmp(command,"PWMWrite")==0) {
      bffr+="\"di\":{";
      sprintf(temp,"\"d%d\":\"p%d\"}",atoi(port),ValueForPWMPort(atoi(port)));
      bffr+=String(temp);
      bffr+="}";
      if (_serial) Serial.println(bffr);
      _server->println(bffr);
    }
    else _server->println("{\"arduino_io\":{\"ERROR\":\"006\"}}"); //ERROR 006; unknown command
  }

  // If used on a MEGA or equivalent
  else if (strcmp(_Arduino,"MEGA")==0) {
    if (strcmp(command,"ReadAll")==0) {
      // Analog
      bffr+="\"an\":{";
      for (int i=0;i<=14;i++) {
        if (_cr && InArray(_CRPorts,_nCRports,i)) sprintf(temp,"\"a%d\":\"%s\",",i,_returnAnalogForPort(i));
        else sprintf(temp,"\"a%d\":%d,",i,analogRead(i));
        bffr+=String(temp);
      }
      if (_cr && InArray(_CRPorts,_nCRports,15)) sprintf(temp,"\"a%d\":\"%s\"",15,_returnAnalogForPort(15));
      else sprintf(temp,"\"a%d\":%d}",15,analogRead(15));
      bffr+=String(temp);
      
      if (_serial) Serial.print(bffr);
      _server->print(bffr);
      
      // Digital
      bffr=",\"di\":{";
      for (int i=0;i<=9;i++) {
        if (_pwm && InArray(_PWMPorts,_nPWMports,i)) sprintf(temp,"\"d%d\":\"p%d\",",i,ValueForPWMPort(i));
        else if (_cf && InArray(_CFPorts,_nCFports,i)) sprintf(temp,"\"d%d\":%s,",i,_returnDigitalForPort(i));
        else sprintf(temp,"\"d%d\":%d,",i,digitalRead(i)); 
        bffr+=String(temp);
      }
      for (int i=14;i<=52;i++) {
        if (_pwm && InArray(_PWMPorts,_nPWMports,i)) sprintf(temp,"\"d%d\":\"p%d\",",i,ValueForPWMPort(i));
        else if (_cf && InArray(_CFPorts,_nCFports,i)) sprintf(temp,"\"d%d\":%s,",i,_returnDigitalForPort(i));
        else sprintf(temp,"\"d%d\":%d,",i,digitalRead(i)); 
        bffr+=String(temp);
      }
      if (_pwm && InArray(_PWMPorts,_nPWMports,53)) sprintf(temp,"\"d%d\":\"p%d\"}",53,ValueForPWMPort(53));
      else if (_cf && InArray(_CFPorts,_nCFports,53)) sprintf(temp,"\"d%d\":%s,",53,_returnDigitalForPort(53));
      else sprintf(temp,"\"d%d\":%d}",53,digitalRead(53));
      bffr+=String(temp);
      bffr+="}}";
      if (_serial) Serial.println(bffr);
      _server->println(bffr);
    }

    // PWMWrite
    else if (strcmp(command,"PWMWrite")==0) {
      bffr+="\"di\":{";
      sprintf(temp,"\"d%d\":\"p%d\"}",atoi(port),ValueForPWMPort(atoi(port)));
      bffr+=String(temp);
      bffr+="}";
      if (_serial) Serial.println(bffr);
      _server->println(bffr);
    }
    else _server->println("{\"arduino_io\":{\"ERROR\":\"006\"}}"); //ERROR 006; unknown command
  }
  bffr="";
}

void DataServeriOS::RemoteDigitalWrite()
{
  // Switch port as requested
  if (strcmp(action,"s")==0) {
    digitalWrite(atoi(port),!digitalRead(atoi(port)));
  }
}

void DataServeriOS::PerformRequestedCommand()
{
  // Check password and initiate requested command
  if (strcmp(_pass,pass)==0) {
    // Password is correct
    if (strcmp(cmd,"digitalWrite")==0) {
      // Check if custom functions are requered, check if required for this port
      if (_cf && InArray(_CFPorts,_nCFports,atoi(port))) CallCustomFunction();
      else RemoteDigitalWrite();
    }
    else if (strcmp(cmd,"PWMWrite")==0) RemotePWMWrite();
    else JSON_send(cmd);
  }
  else {
    // Password was incorrect
    _server->println("{\"arduino_io\":{\"ERROR\":\"007\"}}"); //ERROR 007; incorrect password
  }
}

void DataServeriOS::CallCustomFunction()
{
  _function(atoi(port));
}

void DataServeriOS::RemotePWMWrite()
{
  // Write PWM
  int pin = atoi(port);
  int value = atoi(action);
  analogWrite(pin,value);
  SetValueForPWMPort(value,pin);
  JSON_send("PWMWrite");
}

void DataServeriOS::printValueForDigitalPort(char *value,int port)
{
  char temp[128];
  sprintf(temp,"{\"arduino_io\":{\"di\":{\"d%d\":%s}}}",port,value);
  _server->println(String(temp));
}
