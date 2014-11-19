#include <Arduino.h>
#include "WiFly.h"

WiFly wifly(&Serial1); // connected to 0 and 1 on Arduino Leonardo


void setup() {
  Serial.begin(9600);  // default baudrate //
  Serial1.begin(9600); // default baudrate of WiFly //
   while(!Serial){ //Leonardo specific. keep this in here
    ;
  }
  
  delay(10000); 
  Serial.println("WiFly Version conversion and Config Tool");

  float ver;
  Serial.print("WiFly Version: ");
  ver = wifly.version();
  Serial.println(ver);
  Serial.println("Starting to set version 2.45");
  wifly.reset();                                  // factory RESET
  Serial.println("Factory Reset wait 2 seconds");
  delay(2000);
  wifly.sendCommand("boot image 34\r", "Set");        // set bootimage to 2.45
  Serial.println("Set boot image to 34");
  delay(1000);  
  wifly.sendCommand("save\r", "Storing");         // save
  Serial.println("Storing config");
  delay(1000);
  wifly.sendCommand("reboot\r");
  Serial.println("reboot");
  delay(4000);
  wifly.reset();                                  // factory RESET
  Serial.println("Factory RESET wait 2 seconds");
  delay(2000);
  wifly.sendCommand("save\r", "Storing");         // save
  Serial.println("Storing config");
  Serial.println("Entering Commandmode");
  wifly.write(36); // $ = 36
  wifly.write(36);
  wifly.write(36);
  delay(1000);
  wifly.write(13);
 
}

void loop() {
  while (wifly.available()) {
    Serial.write(wifly.read());
  }
  
  while (Serial.available()) {
    wifly.write(Serial.read());
  }
}
