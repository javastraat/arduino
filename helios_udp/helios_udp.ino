    #include <Helios_Temperature_Sensor_TMP006.h>
    #include <BEURS_LEDS.h>
    #include <Helios_Colour_Sensor_TCS3414.h>
    #include <Helios_EEPROM_24AA014.h>
    #include "WiFly_communicator.h"

    #include <Wire.h>
    #include <SPI.h>

    Helios_Temperature_Sensor_TMP006 ts;
    Helios_EEPROM_24AA014 ee;
    Helios_Colour_Sensor_TCS3414 ct;
    BEURS_LEDS bl;

    #define SSID      "Cubie_Wifi_AP"
    #define KEY       "XXXX"

    #define UDP_HOST_IP        "255.255.255.255"      // broadcast
    #define UDP_REMOTE_PORT    12345
    #define UDP_LOCAL_PORT     UDP_REMOTE_PORT

    // WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
    #define AUTH      WIFLY_AUTH_OPEN


    #define htons(x) ( ((x)<<8) | (((x)>>8)&0xFF) )
    #define ntohs(x) htons(x)

    #define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                       ((x)<< 8 & 0x00FF0000UL) | \
                       ((x)>> 8 & 0x0000FF00UL) | \
                       ((x)>>24 & 0x000000FFUL) )
    #define ntohl(x) htonl(x)

    WiFly wifly(&Serial1);  // WiFly connected to Serial1 on Leonardo/Helios

    void setup()
    {
      Serial.begin(9600); // Serial monitor
      Serial.println("Booting");
     
      bl.Init();
      bl.ClearAll(); 
      Wire.begin();

      delay(200); // give WiFly some time to power up
      Serial1.begin(9600);
     
      Serial.println("Join " SSID );
      if (wifly.join(SSID, KEY, AUTH)) {
        Serial.println("OK");
      } else {
        Serial.println("Failed");
      }
     
      wifly.sendCommand("get everthing\r");
      char c;
      while (wifly.receive((uint8_t *)&c, 1, 300) > 0) {
        Serial.print((char)c);
      }
      Serial.println("-- end of settings --");

      char cmd[32];
      wifly.sendCommand("set w j 1\r", "AOK");   // enable auto join
      wifly.sendCommand("set i p 1\r", "AOK");
      snprintf(cmd, sizeof(cmd), "set i h %s\r", UDP_HOST_IP);
      wifly.sendCommand(cmd, "AOK");
      snprintf(cmd, sizeof(cmd), "set i r %d\r", UDP_REMOTE_PORT);
      wifly.sendCommand(cmd, "AOK");
      snprintf(cmd, sizeof(cmd), "set i l %d\r", UDP_LOCAL_PORT);
      wifly.sendCommand(cmd, "AOK");
      wifly.sendCommand("save\r");
      wifly.sendCommand("reboot\r");

      Serial.println("--------- WIFLY uodp set up --------");

      bl.SetRGB(0,0,0);
     
      Serial.print("Device: "); 
      if(strlen(ee.StringRead(0)) == 0){
          Serial.println("<No Id>");
      }
      else{
          Serial.println(ee.StringRead(0));
      }
     
      Serial.println("Started");
    }

    void loop()
    {
       LuxMeting();
    }


    void LuxMeting()
    {
      unsigned int lux,cct;
      ct.Read(&lux,&cct);
     
      int temp = ts.ReadObject();


      int inttime = 400;
      int gain = 1;
      int presc = 4;
     
      unsigned int red,green,blue,white,rawLux,rawCct; 
      ct.ReadLow(inttime,gain,presc,&red,&green,&blue,&white,&rawLux,&rawCct);

        char buff[120]; 
        sprintf(buff,"%d lux %d K %d C (%d/%d/%d), %d C",lux,cct,temp,red,green,blue, temp);
      Serial.println(buff);

        struct {
          unsigned int version;
          unsigned int lux;
          unsigned int cct;
          unsigned int red;
          unsigned int blue;
          unsigned int green;
          unsigned int temp;
       } packet = {
            htons(0x101), // version 1.01
            htons(lux), htons(cct),
            htons(red), htons(green), htons(blue),  /// RGB
            htons(temp)                    // Temp
        };
       
       wifly.send((uint8_t *)&packet, sizeof(packet), DEFAULT_WAIT_RESPONSE_TIME);
     
      delay(100); // give WiFly some time to transmit the data.
    }
       

