/*
Push.h - Arduino Push Notifications for Arduino Control
 Created by M. Heeres, March 5 2012
 Copyright 2012 M. Heeres
 
 This Library enables push notifications to be send from
 the Arduino to your iDevice if this is enabled in the 
 Arduino Control app.
 */

#ifndef Push_h
#define Push_h

#include "Arduino.h"

class Push
{
public:
  Push(char* key, boolean dbug);
  void push(String message, int badge);
private:
  char* _key;
  boolean _dbug;
};

#endif



