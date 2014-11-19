/*
Push.h - Arduino Push Notifications for Arduino Control
 Created by M. Heeres, March 5 2012
 Copyright 2012 M. Heeres
 
 This Library enables push notifications to be send from
 the Arduino to your iDevice if this is enabled in the 
 Arduino Control app.
 */

#include "Arduino.h"
#include "Ethernet.h"
#include "Push.h"

char pushServer[] = "push.mheeres.com";
Push::Push(char* key, boolean dbug)
{
  _dbug = dbug;
  _key = key;
}

void Push::push(String message, int badge)
{
  // message has to be conposed using '+' between words
  // like: "Arduino+has+booted"
  EthernetClient _pushClient;
  for (int i=0; i<3; i++) {
    // try to connect 3 times in case of connection failure
    if (_key != "") {
      //unique key has to be supplied
      if (_dbug) Serial.println("->connecting to server..");
      if (_pushClient.connect(pushServer, 80)) {
        // connected
        if (_dbug) Serial.println("->connected");

        // create notification destination and content
        String notification = "U=";
        notification.concat(String(_key));
        notification.concat("&M=");
        notification.concat(message);
        notification.concat("&B=");
        notification.concat(String(badge, DEC));

        // create POST request
        String post = "POST /send.php HTTP/1.1\nHost: push.mheeres.com\nContent-Length: ";
        post.concat(notification.length());
        post.concat("\nContent-Type: application/x-www-form-urlencoded\n\n");
        post.concat(notification);

        // write POST request to server
        _pushClient.println(post);
        
        post = "";
        notification = "";

        if (_dbug) {
          Serial.println("->send");
          while (true) {
            // wait for server to respond..
            if (_pushClient.available()) {
              // server responded, output responce
              char c = _pushClient.read();
              Serial.print(c);
            }
            if (!_pushClient.connected()) {
              // server has broken connection (it's finished sending responce)
              Serial.println("\n->stopping");
              _pushClient.stop();
              _pushClient.flush();
              break;
            }
          }
        }
        else {
          // disconnect from server
          _pushClient.flush();
          _pushClient.stop();
        }
        // connection was successful and message was send, break the for loop
        break;
      }
    }
    else break;
  }
  _pushClient.flush();
  _pushClient.stop();
  delay(500); // Give the Arduino some rest..
}

