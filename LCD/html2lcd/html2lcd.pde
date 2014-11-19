/* ***********************************************************************************************
 * HTML to LCD Server v1.1
 * Everett Robinson
 *
 * This project uses the Arduino ethernet shield and a sparkfun 16x2 character LCD to produce a
 * webpage which shows what the LCD is currently displaying, and where a user can change what's
 * displayed by entering something into a couple text fields (one for each row of the lcd). The Arduino 
 * then parses the Html Header using some code adapted from Kevin Haw's RoboSapien Server project:
 * (http://www.arduino.cc/playground/Main/RoboSapienServer).
 * The result of this parsing is the extraction and updating of the LCD with the strings the user 
 * entered, which are also converted back to something readable at the same time. 
 *
 * Bug fixes:
 * A problem in which any zero's were removed from the input has been fixed
 *
 * Remaining Bugs:
 * - Some minor memory leaks may still remain, since strings are nasty like that
 *
 *************************************************************************************************
 * LCD pin to Arduino Digital pin Connection Map (for Sparkfun 16x2 LCD)
 *
 *  // LCD pin 1  to GND
 *  // LCD pin 2  to +5V
 *  // LCD pin 3  to Resistor(2.2K) to GND
 *  // LCD pin 4  (RS) to Arduino pin 2
 *  // LCD pin 5  (RW) to Arduino pin 3
 *  // LCD pin 6  (enable) to Arduino pin 5
 *  // LCD pin 7
 *  // LCD pin 8
 *  // LCD pin 9
 *  // LCD pin 10
 *  // LCD pin 11 (Data 4) to Arduino pin 6
 *  // LCD pin 12 (Data 5) to Arduino pin 7
 *  // LCD pin 13 (Data 6) to Arduino pin 8
 *  // LCD pin 14 (Data 7) to Arduino pin 9
 *  // LCD pin 15 to +5V
 *  // LCD pin 16 to GND
 ************************************************************************************************/
 
//////////////////////////////////////////////////////////////////
//Include all necessary libraries
//////////////////////////////////////////////////////////////////
#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>
//#include <MemoryFree.h>  // Used to try and track down mem leaks, requires http://www.arduino.cc/playground/Code/AvailableMemory
                           // Use the code from the forums half way down the page


//////////////////////////////////////////////////////////////////
//Start initializing webserver, lcd and other miscellaneous stuff
//////////////////////////////////////////////////////////////////

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x4C, 0x64 };
byte ip[] = { 192,168,2, 177 };
byte gateway[] = { 192,168,2, 1 };
byte subnet[] = { 255, 255, 0, 0 };

// Server for web requests
Server server(80);

// Initialize lcd pins according to previous mapping
LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);

// String variables fo each line on the LCD declared globally because I'm lazy, but also
// because it needs to survive loop(), without being cleared from the stack (also to hell with good practice)
String line1 = "                ";
String line2 = "                ";

/////////////////////////////////////////////////////////////////////////
// Constant strings for rendering the Webpage are stored in flash 
// memory to ration it for other non constant variables
/////////////////////////////////////////////////////////////////////////

// Strings stored in flash mem for the Html Header
prog_char Header_0[] PROGMEM = "HTTP/1.1 200 OK";            //
prog_char Header_1[] PROGMEM = "Content-Type: text/html";    //
prog_char Header_2[] PROGMEM = "";                           //
prog_char Header_3[] PROGMEM = "<HTML>\n<HEAD>";             // The header lines
prog_char Header_4[] PROGMEM = "<TITLE>HTML to LCD</TITLE>"; //
prog_char Header_5[] PROGMEM = "</HEAD><BODY>";              //

// A table of pointers to the flash memory strings for the header
PROGMEM const char *Header_table[] =
{   
  Header_0,
  Header_1,
  Header_2,
  Header_3,
  Header_4,
  Header_5};

// Strings stored in flash mem for the body of the webpage, including the input forms
// Only the longer lines, not involving variables are stored here
prog_char Body_0[] PROGMEM = "<H2>Currently Displayed:</H2>";            
prog_char Body_1[] PROGMEM = "<H2>Change it to something else!</H2>";    
prog_char Body_2[] PROGMEM = "<form action=\"/?\" method=get>";                           
prog_char Body_3[] PROGMEM = "<b>Line 1: </b><input type=\"text\" name=\"L1\" maxlength=\"16\" size=\"16\" /><br />";            
prog_char Body_4[] PROGMEM = "<b>Line 2: </b><input type=\"text\" name=\"L2\" maxlength=\"16\" size=\"16\" /><br />"; 
prog_char Body_5[] PROGMEM = "<input type=\"submit\" value=\"Submit\" /></form>";              

// A table of pointers to the flash memory strings for the body
PROGMEM const char *Body_table[] =
{   
  Body_0,
  Body_1,
  Body_2,
  Body_3,
  Body_4,
  Body_5};



//////////////////////////////////////////////////////////////////
// Begin Webserver Specific Code
//////////////////////////////////////////////////////////////////
 
// Print out MIME and HTML header at top of webpage using the
// strings previously stored in flash memory
void HtmlHeader(Client client) {
  
    char buffer[30]; //A character array to hold the strings from the flash mem
    
    for (int i = 0; i < 6; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(Header_table[i]))); // Necessary casts and dereferencing, just copy. 
      client.println( buffer );
    }
}
 
// Print the footer at the bottom of the webpage
void HtmlFooter(Client client) {
    client.println("</BODY></HTML>");
}
 
 
// Parse an HTTP request header one character at a time, seeking string variables (modified from Kevin Haw's code)
void ParseHttpHeader(Client &client) {
    char c;
    int i = 0;    //An integer use to limit the size of rawUrlText (prevents crashing due to running out of memory)
    String rawUrlText = "";
 
    // Skip through until we hit a question mark (first one)
    while((c = client.read()) != '?' && client.available()) {
      // Debug - print data
      Serial.print(c);
    }
    
    // Are we here for a question mark or did we run out of data?
    if(client.available() > 2) {
 
      // Read the data and add it to our unmodified string rawURLText! 
      // the incrementer limits the input to about 1 line of plain text and 1/2 line of symbols, more causes line2 to truncate
      while((c = client.read()) != ' ' && client.available() && i < 55) {
          rawUrlText = rawUrlText + c;
          Serial.print(c);
          i++;
      }
      
      htmlToHuman(rawUrlText);            // Make it readable
      line1.setCharAt(line1.lastIndexOf('0'), ' '); // the lcd prints the 0's in the null terminator, so we remove them
      line2.setCharAt(line2.lastIndexOf('0'), ' '); //This has been revised to prevent remove of legit 0's
      lcd.clear();
      lcd.setCursor(0,0);                 // set cursor to column 0, row 0 (the first row), and print
      lcd.print(line1);
      lcd.setCursor(0,1);                 // set cursor to column 0, row 1 (the second row), and print
      lcd.print(line2);
    }   
  return;
}
  
// A function that takes the html formatted string and makes it readable again
void htmlToHuman(String URLstring) {

    int indexOfDelim = 0;  //This variable stores the location of our delimiter so we can find where line1 ends and line2 begins
    
    // The following array stores a list of ugly html codes, and the special charaters they represent (for changing them back)
    const String CHAR_CONVERSIONS[29][2] = {{"+"," "},{"%40","@"},{"%23","#"},{"%24","$"},{"%2B","+"},{"%21","!"},{"%7E","~"},
                                            {"%3A",":"},{"%3B",";"},{"%2C",","},{"%3F","?"},{"%2F","/"},{"%7C","|"},{"%5E","^"},
                                            {"%5C","\\"},{"%7B","{"},{"%7D","}"},{"%5B","["},{"%5D","]"},{"%3C","<"},{"%3E",">"},
                                            {"%28","("},{"%29",")"},{"%27","'"},{"%22","\""},{"%3F","?"},{"%26","&"},{"%3D","="},
                                            {"%25","%"}};
    
    URLstring = URLstring.replace("L1=","");     // remove the unecessary field variable names
    URLstring = URLstring.replace("&L2=","`");   // and turn this one into our delimiter character ` (The one on the ~ key)
    
    //A for loop that replaces all the html codes with the right symbols
    for (int i=0 ; i < 29; i++) {
      URLstring = URLstring.replace(CHAR_CONVERSIONS[i][0],CHAR_CONVERSIONS[i][1]);
    }
    
    indexOfDelim = URLstring.indexOf("`");       // find the index of that delimiter
    
    line1 = URLstring.substring(0,indexOfDelim); // set line1 and line 2 using that knowlege
    line1 += NULL;                               // but add a null terminator to each to avoid some odd bugs
    line2 = URLstring.substring(indexOfDelim+1,URLstring.length());
    line2 += NULL;
    
    return;
}
 
// Set up webserver functionality (from Kevin Haw)
void WebServerSetup() {
  Ethernet.begin(mac, ip);
  server.begin();
}
 
// Web server loop (modified from Kevin Haw's code)
void WebServerLoop() {  
  Client client = server.available();
  boolean bPendingHttpResponse = false; // True when we've received a whole HTTP request and need to output the webpage
  boolean bPreventSecondParse = false;  //Stops a second successive run of parseHttpHeader(), this prevents some nasty bugs.
  char c;  // For reading in HTTP request one character at a time
  
  if (client) {
    // Loop as long as there's a connection
    while (client.connected()) {
      // Do we have pending data (an HTTP request) and is this the first parse?    
      if (client.available() && !bPreventSecondParse) {
 
        // Indicate we need to respond to the HTTP request as soon as we're done processing it
        bPendingHttpResponse = true;
        bPreventSecondParse = true;
        
        ParseHttpHeader(client);  
      }
      else {
        // There's no data waiting to be read in on the client socket.  Do we have a pending HTTP request?
        if(bPendingHttpResponse) {
          // Yes, we have a pending request.  Clear the flag and then send the webpage to the client
          bPendingHttpResponse = false;
          bPreventSecondParse = false; 
 
          // send a standard http response header and HTML header
          HtmlHeader(client);
 
          // The following code Sends the relevant Html to the connected connected client to render the web page
          /* For readability, the Html sent to the client is as follows (the second and third lines are not stored in flash memory)
           *"<H2>Currently Displayed:</H2>"
           *"<b>Line 1:</b> " + line1 + "<br />"        
           *"<b>Line 2:</b> " + line2 + "<br /><br />" 
           *"<H2>Change it to something else!</H2>"
           *"<form action=\"/?\" method=get>"
           *"<b>Line 1: </b><input type=\"text\" name=\"L1\" maxlength=\"16\" size=\"16\" /><br />"
           *"<b>Line 2: </b><input type=\"text\" name=\"L2\" maxlength=\"16\" size=\"16\" /><br />"
           *"<input type=\"submit\" value=\"Submit\" /></form>"
           */
           
          char buffer[90]; //A character array to hold the strings from the flash mem
          
          strcpy_P(buffer, (char*)pgm_read_word(&(Body_table[0])));        // gets and prints the first line
          client.println( buffer );
          client.println("<b>Line 1:</b> " + line1 + "<br />");            // manually print the lines involving variables
          client.println("<b>Line 2:</b> " + line2 + "<br /><br />");
          for(int i = 1; i < 6; i++) {                                     // Use a for loop to print the remaining lines
            strcpy_P(buffer, (char*)pgm_read_word(&(Body_table[i])));
            client.println( buffer );
          }
          
          // send HTML footer
          HtmlFooter(client);
 
          // give the web browser time to receive the data
          delay(1);
          client.stop();
        }
      }
    }  // End while(connected)
  }
}


void setup(){
  // open the serial port at 9600 bps:
  Serial.begin(9600);
  lcd.begin(16,2);                    // columns, rows.  use 16,2 for a 16x2 LCD, etc.
  lcd.clear();                        // start with a blank screen
  WebServerSetup();
}
 
 
void loop(){
  WebServerLoop();
}
