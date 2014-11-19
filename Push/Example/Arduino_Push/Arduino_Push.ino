#include <Push.h>
#include <Ethernet.h>
#include <SPI.h>

/* Push example for Arduino Control
 - Each message will cost one credit, credits can be bought using in-app purchase
 - Requests are welcome: push@mheeres.com
 
 Created 18-03-2012 by M. Heeres
 */

char UUID[] = "UUID_of_iDevice";
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {localip};

Push notifications(UUID,false);
/*                  ^     ^
                    |     Debug? Prints steps and responce to serial
                    Universally Unique Identifier, see the push window in settings. Already filled in if retrieved from device (iTunes file sharing)
 */

void setup() {
  //Serial.begin(9600);
  // Enable if debugging is required
  Ethernet.begin(mac,ip);
  notifications.push("Arduino+has+booted",1);
  /*                        ^             ^
                            |             Badge number, appears on app icon if the app is not running. Can be any number up till 4 characters
                            Text in the message body, use '+' between words. Does also accept numbers. Maximum length is 100 characters
   */
}

void loop() {

}