/*
 * Web Server well structured.
 * Need a Ethernet Shield over Arduino.
 *
 *  by Alessandro Calzavara, alessandro(dot)calzavara(at)gmail(dot)com
 * and Alberto Capponi, bebbo(at)fast-labs net
 * for Arduino community! :-)
 * 
 * Pro:
 * - HTTP Request managed.
 * - Switch page selection.
 * - HTML pages on flash memory.
 *
 * Cons:
 * - not explicit parameters management.
 */


#include <Ethernet.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <SPI.h>
/**********************************************************************************************************************
*                                   MAC address and IP address.
***********************************************************************************************************************/
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 2, 177 };

/**********************************************************************************************************************
*                                   Memory space for string management and setup WebServer service
***********************************************************************************************************************/

// For sending HTML to the client
#define STRING_BUFFER_SIZE 128
char buffer[STRING_BUFFER_SIZE];

// to store data from HTTP request
#define STRING_LOAD_SIZE 128
char load[STRING_LOAD_SIZE];

// POST and GET variables
#define STRING_VARS_SIZE 128
char vars[STRING_VARS_SIZE];

/**********************************************************************************************************************
*                                   Strings stored in flash of the HTML we will be transmitting
***********************************************************************************************************************/

#define NUM_PAGES 5

// HTTP Request message
PROGMEM prog_char content_404[] = "HTTP/1.1 404 Not Found\nServer: arduino\nContent-Type: text/html\n\n<html><head><title>Arduino Web Server - Error 404</title></head><body><h1>Error 404: Sorry, that page cannot be found!</h1></body>";
PGM_P page_404[] PROGMEM = { content_404 }; // table with 404 page

// HTML Header for pages
PROGMEM prog_char content_main_header[] = "HTTP/1.0 200 OK\nServer: arduino\nCache-Control: no-store, no-cache, must-revalidate\nPragma: no-cache\nConnection: close\nContent-Type: text/html\n";
PROGMEM prog_char content_main_top[] = "<html><head><title>Arduino Web Server</title><style type=\"text/css\">table{border-collapse:collapse;}td{padding:0.25em 0.5em;border:0.5em solid #C8C8C8;}</style></head><body><h1>Arduino Web Server</h1>";
PROGMEM prog_char content_main_menu[] = "<table width=\"500\"><tr><td align=\"center\"><a href=\"/\">Page 1</a></td><td align=\"center\"><a href=\"page2\">Page 2</a></td><td align=\"center\"><a href=\"page3\">Page 3</a></td><td align=\"center\"><a href=\"page4\">Page 4</a></td></tr></table>";
PROGMEM prog_char content_main_footer[] = "</body></html>";
PGM_P contents_main[] PROGMEM = { content_main_header, content_main_top, content_main_menu, content_main_footer }; // table with 404 page
#define CONT_HEADER 0
#define CONT_TOP 1
#define CONT_MENU 2
#define CONT_FOOTER 3

// Page 1
PROGMEM prog_char http_uri1[] = "/";
PROGMEM prog_char content_title1[] = "<h2>Page 1</h2>";
PROGMEM prog_char content_page1[] = "<hr /><h3>Content of Page 1</h3><p>Nothing... yet.</p><br /><form action=\"/login\" method=\"POST\"><input type=\"text\" name=\"prova\"><input type=\"submit\" value=\"post\"></form>";

// Page 2
PROGMEM prog_char http_uri2[] = "/page2";
PROGMEM prog_char content_title2[] = "<h2>Page 2</h2>";
PROGMEM prog_char content_page2[] = "<hr /><h3>Content of Page 2</h3><p>Nothing here.</p>";

// Page 3
PROGMEM prog_char http_uri3[] = "/page3";
PROGMEM prog_char content_title3[] = "<h2>Page 3</h2>";
PROGMEM prog_char content_page3[] = "<hr /><h3>Content of Page 3</h3><p>No no no, white page here.</p>";

// Page 4
PROGMEM prog_char http_uri4[] = "/page4";
PROGMEM prog_char content_title4[] = "<h2>Page 4</h2>";
PROGMEM prog_char content_page4[] = "<hr /><h3>Content of Page 4</h3><p>Ehm... no, nothing.</p>";

// Page 5
PROGMEM prog_char http_uri5[] = "/login";
PROGMEM prog_char content_title5[] = "<h2>POST Page 5</h2>";
PROGMEM prog_char content_page5[] = "<hr /><h3>Content of Page 5</h3><p>received a POST request</p>";

// declare tables for the pages
PGM_P contents_titles[] PROGMEM = { content_title1, content_title2, content_title3, content_title4, content_title5 }; // titles
PGM_P http_uris[] PROGMEM = { http_uri1, http_uri2, http_uri3, http_uri4, http_uri5 }; // URIs
PGM_P contents_pages[] PROGMEM = { content_page1, content_page2, content_page3, content_page4, content_page5 }; // real content

/**********************************************************************************************************************
*                                        define HTTP return structure ID for parsing HTTP header request
***********************************************************************************************************************/
struct HTTP_DEF {
  int pages;
  char vars[20];
} ;

/**********************************************************************************************************************
*                                                        Shared variable
***********************************************************************************************************************/
Server server(80);

void setup() {
  Ethernet.begin(mac, ip);
  server.begin();

  Serial.begin(9600); // DEBUG
}

/**********************************************************************************************************************
*                                                           Main loop
***********************************************************************************************************************/

void loop() {
  Client client = server.available();
  if (client) { // now client is connected to arduino

    // read HTTP header request... so select what page client are looking for
    HTTP_DEF http_def = readHTTPRequest(client);

    if (http_def.pages > 0) {
      sendPage(client,http_def);
    } else {
      contentPrinter(client,(char*)pgm_read_word(&(page_404[0])));
    }

    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }

}

/**********************************************************************************************************************
*                                              Method for read HTTP Header Request from web client
***********************************************************************************************************************/
struct HTTP_DEF readHTTPRequest(Client client) {
  char c;
  int i;

  // use buffer, pay attention!
  int bufindex = 0; // reset buffer
  int loadindex = 0; // reset load

  int contentLength = 0; // reset POST content length
  char compare[50]; // page comparison (URI selection)

  HTTP_DEF http_def; // use the structure for multiple returns

  http_def.pages = 0; // default page selection... error

  // reading all rows of header
  if (client.connected() && client.available()) { // read a row
    buffer[0] = client.read();
    buffer[1] = client.read();
    bufindex = 2;
    // read the first line to determinate the request page
    while (buffer[bufindex-2] != '\r' && buffer[bufindex-1] != '\n') { // read full row and save it in buffer
      c = client.read();
      if (bufindex<STRING_BUFFER_SIZE) buffer[bufindex] = c;
      bufindex++;
    }

    // select the page from the buffer (GET and POST) [start]
    for(i = 0; i < NUM_PAGES; i++) {
      strcpy_P(load, (char*)pgm_read_word(&(http_uris[i])));
      // GET
      strcpy(compare,"GET ");
      strcat(compare,load);
      strcat(compare," ");
      Serial.print("GET compare: "); // DEBUG
      Serial.println(compare); // DEBUG
      if (strncmp(buffer,compare, strlen(load)+5)==0) {
        http_def.pages = i+1;
        break;
      }

      // POST
      strcpy(compare,"POST ");
      strcat(compare,load);
      strcat(compare," ");
      Serial.print("POST compare: "); // DEBUG
      Serial.println(compare); // DEBUG
      if (strncmp(buffer,compare, strlen(load)+6)==0) {
        http_def.pages = i+1;
        break;
      }

    }
    // select the page from the buffer (GET and POST) [stop]

    // read other stuff (for POST requests) [start]
    if (strncmp(buffer, "POST /", 5)==0) {
      processRequest:
      loadindex = 2; // reset load

      memset(load,0,STRING_LOAD_SIZE);
      load[0] = client.read();
      load[1] = client.read();
      while (load[loadindex-2] != '\r' && load[loadindex-1] != '\n') {
        c = client.read();
        if (loadindex<STRING_BUFFER_SIZE) load[loadindex] = c;
        loadindex++;
      }
      if (strncmp(load, "Content-Length: ",16)==0) {
        loadindex = 16;
        for(i = loadindex; i< strlen(load) ; i++) {
          if (load[i] != ' ' && load[i] != '\r' && load[i] != '\n') {
            vars[i-loadindex] = load[i];
          } else {
            break;
          }
        }

        contentLength = atoi(vars);
        memset(vars,0,STRING_VARS_SIZE);
        client.read(); client.read(); // read null line
        i = 0;
        while (i<contentLength) {
          c = client.read();
          vars[i] = c;
          ++i;
        }

      } else {
      	goto processRequest;
      }
    }
    // read other stuff (for POST requests) [stop]


    // clean buffer for next row
    bufindex = 0;
  }

//      delay(10); // removing this nothing work... if you understand why mail me a bebbo [at] bebbo [dot] it
  Serial.print("Grepped page: "); // DEBUG
  Serial.println(http_def.pages); // DEBUG

  strncpy(http_def.vars,vars,STRING_VARS_SIZE);

  return http_def;
}

/**********************************************************************************************************************
*                                                              Send Pages
***********************************************************************************************************************/
void sendPage(Client client,struct HTTP_DEF http_def) {

  // send HTML header
  // contentPrinter(client,(char*)pgm_read_word(&(contents_main[CONT_HEADER])));
  contentPrinter(client,(char*)pgm_read_word(&(contents_main[CONT_TOP])));

  // send menu
  contentPrinter(client,(char*)pgm_read_word(&(contents_main[CONT_MENU])));

  // send title
  contentPrinter(client,(char*)pgm_read_word(&(contents_titles[http_def.pages-1])));

  // send the body for the requested page
  sendContent(client,http_def.pages-1);

  client.print("<br />");
  // send POST variables
  client.print(vars);

  // send footer
  contentPrinter(client,(char*)pgm_read_word(&(contents_main[CONT_FOOTER])));
}

/**********************************************************************************************************************
*                                                              Send content split by buffer size
***********************************************************************************************************************/
void contentPrinter(Client client, char *realword) {
  int total = 0;
  int start = 0;
  char buffer[STRING_BUFFER_SIZE];
  int realLen = strlen_P(realword);

  memset(buffer,0,STRING_BUFFER_SIZE);

  while (total <= realLen) {
    // print content
    strncpy_P(buffer, realword+start, STRING_BUFFER_SIZE-1);
    client.print(buffer);

    // more content to print?
    total = start + STRING_BUFFER_SIZE-1;
    start = start + STRING_BUFFER_SIZE-1;

  }
}

/**********************************************************************************************************************
*                                                              Send real page content
***********************************************************************************************************************/
void sendContent(Client client, int pageId) {
  contentPrinter(client,(char*)pgm_read_word(&(contents_pages[pageId])));
}

/**********************************************************************************************************************
*                                                          END OF CODE! WELL DONE!
***********************************************************************************************************************/
