
 
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <WiFly.h>
#include "HTTPClient.h"

#define SSID      "Your-SSID"
#define KEY       "passphrase"
// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA2_PSK

#define HTTP_POST_URL "http://api.yeelink.net/Your Sensor URL"
#define HTTP_POST_HEADERS "U-ApiKey: Your API Key\r\n"

// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
WiFly wifly(2, 3);
HTTPClient http;
char get;

void setup() {
  Serial.begin(9600);
  Serial.println("------- WIFLY HTTP --------");
  
  // Wait WiFly to init
//  delay(3000);
  
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
}

unsigned long start_millis = 0;
char post_data_buf[32];

void loop() {
  if ((millis() - start_millis) < 20000) {
    return;
  } else {
    start_millis = millis();
  }
  
  Serial.println("\r\n\r\nTry to post data to url - " HTTP_POST_URL);
  Serial.println("-------------------------------");
  
  snprintf(post_data_buf, sizeof(post_data_buf), "{\"value\": %d}\r\n", random(10, 100));
  while (http.post(HTTP_POST_URL, HTTP_POST_HEADERS, post_data_buf, 10000) < 0) {
  }
  
  char r;
  while (wifly.receive((uint8_t *)&r, 1, 1000) == 1) {
    Serial.print(r);
  }
}
