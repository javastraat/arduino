/*
	WifiGarage Version 002
	Developed by Brendon Milleker
	
	Please visit http://wifigarage.com for more details.
*/

#include <WiServer.h>
#include <Time.h>;
#include <DS1307RTC.h> 
#include <Wire.h>
#include <md5.h>
#include "wifigarage_config.h";

#define WIRELESS_MODE_INFRA	1
#define RELAY               6

// save last triggered time and last reset time
time_t lastTriggered = 0;
time_t lastReset = 0;

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;


long getTimeKey() {
	long key;
	
	key = (day() + month() + year() + hour() + minute()) * MULTIPLIER;
	return key;
}

time_t triggerTime() {
	time_t time;
	
	//time = hour() + minute() + second();
	time = now();
	
	return time;
}

boolean handleURL(char* URL) {
	String strURL = String(URL);
	String password = String(PASSWORD);
	
	MD5 md5Hasher;
	md5_hash_t destination;
	String unhashedKey = String(PASSWORD);
	char hashedKey[32];
	
	if (strURL.startsWith("/getkey") == true) {
		// request for key has been made.. send a timestamp * multiplier
		// this key is only good for a minute or less
		// there is an odd chance the key will expire by the time the iPhone app makes the second request
		WiServer.print((String)getTimeKey());  
		return true;
	
	
	} else if (strURL.startsWith("/trigger=") == true) {
		if (lastTriggered <= (triggerTime() - 10) || lastTriggered == 0) {
			// concat to make a new key	
			unhashedKey.concat(getTimeKey());
			unhashedKey.concat("\0");
	
	
			// create a new char array from the concated key 
			char blah[unhashedKey.length()];
			unhashedKey.toCharArray(blah, sizeof(blah) + 1);
	
			// md5 hash the key for comparison
			md5Hasher.ComputeHash(&destination, blah);
			MD5::ToHexString(destination, hashedKey);


			// check if the received key matches our newly generated key
			if (strURL.endsWith(hashedKey) == true) {  
                                WiServer.print("done");  
                                
				digitalWrite(RELAY, HIGH);
				delay(1500); 
				digitalWrite(RELAY, LOW); 
                                delay(1000);

				// save the last trigger time
				lastTriggered = triggerTime();

				return true;
			} else {
				WiServer.print("Incorrect password, or time out has occurred."); 
				return true;
			}
		} else {
			WiServer.print("done"); 
			return true;
		}
	} else {
		WiServer.print("Invalid destination"); 
		return true;
	}

	return false;
}


void setup() {
	// Initialize WiServer
	WiServer.init(handleURL);

	// Enable Serial output and ask WiServer to generate log messages (optional)
	Serial.begin(9600);
	WiServer.enableVerboseMode(true);

	// pin setup
	pinMode(RELAY, OUTPUT);
	
	// time setup for keys
	setTime(0, 0, 0, 1, 1, 2010);
}

void loop(){
        /*if (hour() >= 5) {
           setTime(0);
           Serial.println("Time set back to zero");
        }*/
        
        // reset every 2 hours
        //if ((hour() % 2) == 0 && hour() != 0 && minute() == 1) {
        if ((hour() == 2 || hour() == 4 || hour() == 6 || hour() == 8 || hour() == 10 || hour() == 12
            || hour() == 14 || hour() == 16 || hour() == 18 || hour() == 20 || hour() == 24) 
            && minute() == 1 && second() >= 0 && second() <= 20) { //&& minute() == 1 && 
              
            WiServer.init(handleURL);
            lastReset = hour();
 
            Serial.println ("Wifi has been reset @ " + (String)hour());
        }   
  
	// Run WiServer
	WiServer.server_task();
	delay(10);
}

