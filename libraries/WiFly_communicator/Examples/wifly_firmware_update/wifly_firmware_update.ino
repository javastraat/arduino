/*  This script is made for a WiFly connected to a Arduino Leonardo (with the protoshield for example) or the
    Helios gadget ( http://www.heliosgadget.nl/ )

    This script will update your WiFly to the following firmware files
    wifly7-400.img
    wifly7-400.mif   
 
    Please change those file names in case there is new firmware available.
    http://www.rovingnetworks.com/Current_Firmware    
*/

#include "WiFly_communicator.h"

#define SSID      "Your-SSID"
#define KEY       "passphrase"

// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA1_2

WiFly wifly(&Serial1);  // WiFly connected to Serial1 on Leonardo/Helios

boolean firmwareUpdated = false; // set to true when firmware is updated, next boot sequence the .mif files are done

void setup()
{
  Serial.begin(9600); // Serial monitor
  
  // don't do anything until the serial monitor is opened
  while (!Serial) {
    ; // wait for serial port to connect. 
  }
  
  Serial.println("starting...");
  delay(200); // give WiFly some time to power up
  
  // for debugging this script we can reset the firmware again
  //Serial1.begin(9600); // Wifly
  //delay(50);
  //wifly.sendCommand("boot image 2\r", "=OK");        
  //wifly.sendCommand("save\r", "Storing");   
  //wifly.reboot();  
  
  // run factory reset otherwise ftp connection doesn't work sometimes
  Serial1.begin(9600); 
  wifly.reset(); // factory reset in case settings where changed
  wifly.sendCommand("save\r", "Storing"); // save
  wifly.reboot();
  delay(200);
  Serial1.flush();
  Serial1.end();  

    
  initWifly();
}

void initWifly()
{   delay(200);

    Serial1.begin(9600); // rx/tx WiFly
    Serial1.flush();    // errors without doing this
    wifly.clear();      // errors without doing this
    wifly.dataMode();   // errors without doing this
    
    delay(200);
    
    wifly.sendCommand("set i d 1\r", "AOK");
    wifly.sendCommand("set w j 0\r", "AOK");  
    wifly.sendCommand("set o j 2000\r", "AOK");  
    wifly.sendCommand("save\r", "Storing"); 
    
    while (1) {
      Serial.println("Try to join " SSID );
      if (wifly.join(SSID, KEY, AUTH)) {
        Serial.println("Succeed to join " SSID);
        wifly.clear();
        updateFirmware();
        break;
      } else {
        Serial.println("Failed to join " SSID);
        Serial.println("Wait 3 second and try again...");
        delay(3000);
      }
    }
}

void updateFirmware()
{
  wifly.sendCommand("set ftp address 0\r", "AOK");
  wifly.sendCommand("set dns name rn.microchip.com\r", "AOK");
  wifly.sendCommand("save\r", "Storing"); 
   
  if(firmwareUpdated != true)
  {   // first update the firmware img   
      Serial.println("Starting firmware update from ftp, feedback is not visible, this can take a some seconds.");
      
      if(wifly.sendCommand("ftp update wifly7-400.img\r", "UPDATE OK", 30000) == true)
      { Serial.println("Firmware update succeeded!");   
        firmwareUpdated = true;
        wifly.sendCommand("factory RESET\r", "Defaults", 2000);
        wifly.reboot();  
        delay(500);
        initWifly(); 
      } 
      else Serial.println("Firmware update failed. Please retry by reseting the board."); 
  }
  else
  {   // then update the additional files .mif
      Serial.println("Starting additional files update from ftp, feedback is not visible, this can take a some seconds.");      
  
      if(wifly.sendCommand("ftp update wifly7-400.mif\r", "UPDATE OK", 60000))
      { Serial.println("Additional files update succeeded!"); 
      } 
      else Serial.println("Additional files update failed. Please retry by reseting the board."); 
  }  
 
}

void loop()
{ while(wifly.available())
  {  /*
     if(wifly.peek() == '*')  
     { 
        if(wifly.find("*Reboot*�")) 
        { if(reInitOnReboot == true)
          { Serial.println("re-init");
            initWifly();
          }
        }
        
     }
     */
       
     Serial.write(wifly.read());
   }
   
   if (Serial.available()) {
     wifly.write(Serial.read());
   }
   
}


