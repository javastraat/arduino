# WiFly Library for Arduino

There are already some Arduino libraries for WiFly, such as http://sourceforge.net/projects/arduinowifly/.
It seems these libraries are either too simple or too complex. So I write a new one.
Libraries using SoftwareSerial have a common problem --- SoftwareSerial is not stable enough, It often receives wrong characters.

There are some examples:
+ HTTP GET/POST
+ update firmware
+ setup WiFi AP
+ join Ad-Hoc network
+ UDP communication