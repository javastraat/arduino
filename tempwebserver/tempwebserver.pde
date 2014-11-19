/*
  Example 16.1 
 http://tronixstuff.com/tutorials
 Arduino and Ethernet
 */

#include <Ethernet.h>
#include <SPI.h>

float voltage=0;
float sensor=0;
float celsius=0;
float fahrenheit=0;
float photocell=0;
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {
  192,168,2,177 }; // don't forget to change this IP address for your own situation
Server server(80);

void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  Client client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (c == '\n' && current_line_is_blank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          for (int loopy=0; loopy<10; loopy++) 
          // here the sketch will read the temperature and analogue value from the photocell ten times with a delay of one second each iteration.
          // data will be displayed after the last iteration
          {
            sensor=analogRead(0);
            voltage=((sensor*5000)/1024);
            voltage=voltage-500;
            celsius=voltage/10;
            fahrenheit=((celsius*1.8)+32);
            client.print("Temperature: ");
            client.print(celsius,2);
            client.print(" degrees C; ");
            client.print(fahrenheit,2);
            client.print(" degrees F");
            client.println("<br />"); // new line
            photocell=analogRead(1);
            client.print("Light reading ");
            client.println(photocell, 2);
            client.println("<br />"); // new line
            delay(1000);
          }
          break;
        }
        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } 
        else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}


