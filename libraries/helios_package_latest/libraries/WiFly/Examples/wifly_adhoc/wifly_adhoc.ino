
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFly.h"

#define SSID      "WiFly-Ad-Hoc"

#define UDP_HOST_IP        "255.255.255.255"      // broadcast
#define UDP_REMOTE_PORT    55555
#define UDP_LOCAL_PORT     55555

WiFly wifly(2, 3);

char cmd[32];
char ip[24];
char packet[64];
int id;          // WiFly's ID, between 1 and 100
unsigned long time_point = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("--------- WiFly Ad-Hoc --------");
  
  wifly.reset();
  
  if (wifly.version() >= 2.45) {
     Serial.println("The version of this WiFly's firmware is >= 2.45.");
     Serial.println("Warnning! This WiFly's firmware does not support Ad-Hoc mode. Try Ad-Hoc anyway"); 
  }
  
  // set up Ad-Hoc mode
  wifly.sendCommand("set w j 4\r", "AOK");
  wifly.sendCommand("set w s " SSID "\r", "AOK");
  wifly.sendCommand("set w c 1\r", "AOK");
  
  // get random id
  randomSeed(analogRead(3));
  id = random(1, 100);
    
  snprintf(ip, sizeof(ip), "10.0.0.%d\r", id);
  snprintf(cmd, sizeof(cmd), "set i a %s\r", ip);
  wifly.sendCommand(cmd, "AOK");
//  wifly.sendCommand("set i a 10.0.0.10\r", "AOK");

  wifly.sendCommand("set i n 255.255.255.0\r", "AOK");
  wifly.sendCommand("set i d 0\r", "AOK");
  
  // set up UDP
  wifly.sendCommand("set i p 1\r", "AOK");
  snprintf(cmd, sizeof(cmd), "set i h %s\r", UDP_HOST_IP);
  wifly.sendCommand(cmd, "AOK");
  snprintf(cmd, sizeof(cmd), "set i r %u\r", UDP_REMOTE_PORT);
  wifly.sendCommand(cmd, "AOK");
  snprintf(cmd, sizeof(cmd), "set i l %u\r", UDP_LOCAL_PORT);
  wifly.sendCommand(cmd, "AOK");
  
  // set up time
 // wifly.sendCommand("set t a 202.155.212.78\r", "AOK"); // 2.pool.ntp.org
  wifly.sendCommand("set t e 0\r", "AOK");
  wifly.sendCommand("set t r 1\r", "AOK");
  
  wifly.sendCommand("save\r");
  wifly.sendCommand("reboot\r");
  
  delay(1000);
  wifly.clear();
  Serial.print("IP: ");
  Serial.println(ip);
  Serial.println("SSID: " SSID);
}

char line_buf[64];
int index = 0;

void loop() {
  while (wifly.available()) {
    Serial.print((char)wifly.read());
  }

#if 1
  while (Serial.available()) {
    wifly.write(Serial.read());
  }
#endif
  
  // send an UDP packet in every 10 second
  if ((millis() - time_point) > 10000) {
    time_point = millis();
    
    snprintf(packet, sizeof(packet), "\r\nI'm WiFly NO %d. My IP is %s. I'm living\r\n", id, ip);
    wifly.send(packet);
  }
}
