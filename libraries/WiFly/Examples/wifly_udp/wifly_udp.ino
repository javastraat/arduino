
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFly.h"

#define SSID      "Your-SSID"
#define KEY       "passphrase"
// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA2_PSK

#define UDP_HOST_IP        "255.255.255.255"      // broadcast
#define UDP_REMOTE_PORT    55555
#define UDP_LOCAL_PORT     55555

// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
WiFly wifly(2, 3);

void setupUDP(const char *host_ip, uint16_t remote_port, uint16_t local_port)
{
  char cmd[32];
  
  wifly.sendCommand("set w j 1\r", "AOK");   // enable auto join
  
  wifly.sendCommand("set i p 1\r", "AOK");
  snprintf(cmd, sizeof(cmd), "set i h %s\r", host_ip);
  wifly.sendCommand(cmd, "AOK");
  snprintf(cmd, sizeof(cmd), "set i r %d\r", remote_port);
  wifly.sendCommand(cmd, "AOK");
  snprintf(cmd, sizeof(cmd), "set i l %d\r", local_port);
  wifly.sendCommand(cmd, "AOK");
  wifly.sendCommand("save\r");
  wifly.sendCommand("reboot\r");
}

void setup() {

  Serial.begin(9600);
  Serial.println("--------- WIFLY UDP --------");

  wifly.reset();
  
  while (1) {
    Serial.println("Try to join " SSID );
    if (wifly.join(SSID, KEY, AUTH)) {
      Serial.println("Succeed to join " SSID);
      wifly.clear();
      break;
    } else {
      Serial.println("Failed to join " SSID);
      Serial.println("Wait 1 second and try again...");
      delay(1000);
    }
  }
  
  setupUDP(UDP_HOST_IP, UDP_REMOTE_PORT, UDP_REMOTE_PORT);
  
  delay(1000);
  wifly.clear();
}

unsigned int time_point = 0;

void loop() {
  if (wifly.available()) {
    Serial.print((char)wifly.read());
  }
  
  // send an UDP packet in every 10 second
  if ((millis() - time_point) > 10000) {
    time_point = millis();
    
    wifly.send("I'm wifly, I'm living\r\n");
  }
}
