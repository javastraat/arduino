#include <DataServeriOS.h>
#include <SPI.h>
#include <Ethernet.h>

/* Arduino Control example
 - Comments are welcome, arduinocontrolapp@gmail.com
 
 Created 18-03-2012 by M. Heeres
 */

//***************************************************//
// MAC address can be anything that is unique within your network.                                                           //
// IP is the address the Arduino Ethernet Card would respond to.  It needs to be an unused address within your network.      //
// Security password, limited to 15 characters, use to prevent unauthorized access. Mandatory!                               //
int macArray[]= {0x00, 0x1E, 0x2A, 0x77, 0x24, 0x02 };
int ipArray[]= {10,0,20,100};
char secpass[] = "PASSWORD";
int Port = 80; // Port 80 is http
//************************************************//

byte mac[]= {macArray[0], macArray[1], macArray[2], macArray[3], macArray[4], macArray[5]};
byte ip[]= {ipArray[0], ipArray[1], ipArray[2], ipArray[3]};
int PWMPorts[] = {6,9};
int ownFunctionForPorts[] = {5,7};
int ownReturnForPorts[] = {0,3};
EthernetServer server(Port);
DataServeriOS dataServer(&server,secpass,true,"UNO",&substitudeLoop,PWMPorts,2,ownFunctionForPorts,2,&ownFunction,&ownDigitalReturn,ownReturnForPorts,2,&ownAnalogReturn);
/*                        ^        ^      ^      ^        ^            ^      ^        ^            ^      ^              ^                ^          ^      ^
                          |        |      |      |        |            |      |        |            |      |              |                |          |      Pointer to own analog return
                          |        |      |      |        |            |      |        |            |      |              |                |          Number of ports for own return (length of array)
                          |        |      |      |        |            |      |        |            |      |              |                Array of ports for which you wish to return your own value
                          |        |      |      |        |            |      |        |            |      |              Pointer to own digital return, belongs to own function
                          |        |      |      |        |            |      |        |            |      Poiter to own function
                          |        |      |      |        |            |      |        |            Number of ports for which your own function (length of array)
                          |        |      |      |        |            |      |        Array of ports for which you wish your own function to be triggered
                          |        |      |      |        |            |      Number of ports whihc are to be used as PWM (length of array)
                          |        |      |      |        |            Array of ports which should act as PWM
                          |        |      |      |        Pointer to the substitude loop()
                          |        |      |      "UNO" or "MEGA", dicating the number of available ports
                          |        |      Debug? Prints request and return from and to the app
                          |        The password
                          Pointer to the EthernetServer
*/
                                                                                                                                            

void setup()
{
  Ethernet.begin(mac,ip);
  //Serial.begin(9600);
  // Enable if debugging is required 
}

void loop()
{
  // The dataServer loop takes care of communication, use the substitude loop as normal loop
  dataServer.loop();
}

void substitudeLoop()
{
  // Normal loop() contents goes here..
}

void ownFunction(int pin)
{
  // Do some action other than the default switching for the previously defined ports
  // The app expects a return, you are responcable to provide this here. This should be "1","0" or "\"p255\"" in case of PWM
  // The same value is requested in 'ownDigitalReturn' when getting all values
  // (prefix 'p' indicates to the app that this port is used for PWM)
  dataServer.printValueForDigitalPort("1",pin);
}

char* ownDigitalReturn(int pin)
{
  // Return "1","0" or "\"p255\""
  // (prefix 'p' indicates to the app that this port is used for PWM)
  return "1";
}

char* ownAnalogReturn(int pin)
{
  // Return a value for the previously defined ports
  if (pin == 0) {
    // Return a text value, or variable stored alsewhere
    return "Door open";
  }
  else if (pin == 3) {
    char t[128];
    // Return a formatted value (deg C), you can use unicode
    sprintf(t,"%d\u00B0C", analogRead(3));
    return t;
  }
}
