/*
DataServeriOS.h
 Arduino JSON Data Server for iOS
 created by M. Heeres, March 5 2012
 Copyright 2012 M. Heeres
 
 Library belonging to the Arduino Control app for iOS.
 */

#ifndef DataServeriOS_h
#define DataServeriOS_h

#include "Arduino.h"
#include "Ethernet.h"

class DataServeriOS
{
public:
  DataServeriOS(EthernetServer *server, char *pass, boolean serial, char *Arduino, void (*substitudeLoop)(void),int pwmports[],byte nports,int cfports[],byte ncfports,void(*customfunction)(int),char*(*customdigitalreturn)(int),int crports[],byte ncrports,char*(*customanalogreturn)(int));
  void loop();
  void printValueForDigitalPort(char *value,int port);
private:
  int *_mac;
  int *_ip;
  char *_key;
  int _Port;
  byte _nPWMports;
  byte _nCFports;
  byte _nCRports;
  boolean _serial;
  boolean _pwm;
  boolean _cf;
  boolean _cr;
  char* _pass;
  char* _Arduino;
  int _customFunctionArray[];
  void(*_function)(int);
  void (*_substitudeLoop)(void);
  char*(*_returnDigitalForPort)(int);
  char*(*_returnAnalogForPort)(int);
  void SetupPorts();
  void WaitForRequest();
  void ParseReceivedRequest();
  int SetValueForPWMPort(int value, int PWMPort);
  int ValueForPWMPort(int PWMport);
  boolean InArray(int array[],byte n,int value);
  void JSON_send(char* cmd);
  void RemoteDigitalWrite();
  void PerformRequestedCommand();
  void RemotePWMWrite();
  void CallCustomFunction();
  EthernetServer *_server;
};

#endif
