/*
 * Web Server - multi-page.  5 Feb 2011.
 * Need an Ethernet Shield over Arduino.
 *
 * by Martyn Woerner extending the good work of
 * Alessandro Calzavara, alessandro(dot)calzavara(at)gmail(dot)com
 * and Alberto Capponi, bebbo(at)fast-labs net
 * for Arduino community! :-)
 * 
 * Pro:
 * - HTTP Requests GET & POST
 * - Switch page selection.
 * - HTML pages in flash memory.
 * - Button to turn LED on/off
 * - Favicon & png images
 *
 */
#include <SPI.h>
#include <Ethernet.h>
#include "WebServer.h"

#define USE_DHCP_FOR_IP_ADDRESS
#define USE_IMAGES
#define USE_ARDUINO_ICON

/**********************************************************************************************************************
*                                   MAC address and IP address.
***********************************************************************************************************************/
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

#if !defined USE_DHCP_FOR_IP_ADDRESS
// ip represents the fixed IP address to use if DHCP is disabled.
byte ip[] = { 192, 168, 1, 100 };
#endif

int ledPin = 9;
bool isLedOn = false;

// Http header token delimiters
char *pSpDelimiters = " \r\n";
char *pStxDelimiter = "\002";    // STX - ASCII start of text character

/**********************************************************************************************************************
*                                   Strings stored in flash of the HTML we will be transmitting
***********************************************************************************************************************/

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
PROGMEM prog_char content_page1[] = "<hr /><h3>Content of Page 1</h3><p>Nothing... yet.</p><br /><form action=\"/login\" method=\"POST\"><input type=\"text\" name=\"prova\"><input type=\"submit\" value=\"post\"></form>"
    "<br /><form action=\"/login\" method=\"GET\"><input type=\"text\" name=\"prova2\"><input type=\"submit\" value=\"get\"></form><form action=\"/login\" method=\"POST\"><button name=\"LedToggle\" value=\"myvalue\" type=\"submit\">greenery</button></form>";

// Page 2
PROGMEM prog_char http_uri2[] = "/page2";
PROGMEM prog_char content_title2[] = "<h2>Page 2</h2>";
PROGMEM prog_char content_page2[] = "<hr /><h3>Content of Page 2</h3><p>Nothing here. Except '\002' that.</p><p>Oh, and '\002' that.</p><p>Finally '\002' that.</p>";

// Page 3
PROGMEM prog_char http_uri3[] = "/page3";
PROGMEM prog_char content_title3[] = "<h2>Page 3</h2>";
PROGMEM prog_char content_page3[] = "<hr /><h3>Content of Page 3</h3><p><form action=\"/page3\" method=\"POST\"><button name=\"LedOn\" value=\"\002\" type=\"submit\">Turn led \002</button>"
#ifdef USE_IMAGES
    "<img src=\"led\002.png\" alt=\"Smiley face\" height=\"32\" width=\"32\" />"
#endif
    "</form></p>";

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
PGM_P contents_pages [] PROGMEM = { content_page1, content_page2, content_page3, content_page4, content_page5 }; // real content

#ifdef USE_IMAGES

/**********************************************************************************************************************
*                     Image strings and data stored in flash for the image UISs we will be transmitting
***********************************************************************************************************************/

// A Favicon is a little custom icon that appears next to a website's URL in the address bar of a web browser.
// They also show up in your bookmarked sites, on the tabs in tabbed browsers, and as the icon for Internet shortcuts
// on your desktop or other folders in Windows.
PROGMEM prog_char http_uri6[] = "/favicon.ico";    // favicon Request message
PROGMEM prog_char http_uri7[] = "/ledon.png";
PROGMEM prog_char http_uri8[] = "/ledoff.png";

PROGMEM prog_char content_image_header[] = "HTTP/1.1 200 OK\nServer: arduino\nContent-Length: \002\nContent-Type: image/\002\n\r\n";

#ifdef USE_ARDUINO_ICON

PROGMEM prog_char content_favicon_data[] = {
             0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x10, 0x00, 0x01, 0x00, 0x04, 0x00, 0x28, 0x01, 0x00, 0x00, 0x16, 0x00,
             0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x41, 0x00, 0x00, 0x14, 0x44, 0x44, 0x44, 0x41, 0x00, 0x00, 0x00, 0x00,
             0x14, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x40, 0x23,
             0x33, 0x20, 0x02, 0x33, 0x32, 0x04, 0x12, 0x30, 0x00, 0x32, 0x23, 0x00, 0x03, 0x21, 0x03, 0x00, 0x00, 0x03, 0x30, 0x03,
             0x00, 0x30, 0x03, 0x03, 0x33, 0x03, 0x30, 0x33, 0x30, 0x30, 0x03, 0x00, 0x00, 0x03, 0x30, 0x03, 0x00, 0x30, 0x02, 0x30,
             0x00, 0x32, 0x23, 0x00, 0x03, 0x20, 0x10, 0x23, 0x33, 0x20, 0x02, 0x33, 0x32, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x04, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x41,
             0x00, 0x00, 0x00, 0x00, 0x14, 0x44, 0x44, 0x44, 0x41, 0x00, 0x00, 0x14, 0x44, 0x44, 0xF8, 0x1F, 0x00, 0x00, 0xE0, 0x07,
             0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
             0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00
             };
#else

PROGMEM prog_char content_favicon_data[] = {
             0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x10, 0x00, 0x01, 0x00, 0x04, 0x00, 0x28, 0x01, 0x00, 0x00, 0x16, 0x00,
             0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x33, 0x66, 0x00, 0x33, 0x33, 0x66, 0x00, 0x66, 0x66, 0x99, 0x00, 0x99, 0x99,
             0x99, 0x00, 0x99, 0x99, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x62, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x26, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x50, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x07, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x76, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x77, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x37, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
             };
#endif  // USE_ARDUINO_ICON

PROGMEM prog_char content_led_on_png_data[] = {
             0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x0E,
             0x00, 0x00, 0x00, 0x0E, 0x04, 0x03, 0x00, 0x00, 0x00, 0xED, 0x66, 0x30, 0xE2, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47,
             0x42, 0x00, 0xAE, 0xCE, 0x1C, 0xE9, 0x00, 0x00, 0x00, 0x27, 0x50, 0x4C, 0x54, 0x45, 0x00, 0x00, 0x00, 0xFF, 0xCC, 0x00,
             0xCC, 0x66, 0x00, 0xFF, 0xFF, 0x33, 0xFF, 0x99, 0x00, 0xFF, 0xCC, 0x33, 0xCC, 0x99, 0x66, 0xFF, 0xFF, 0x99, 0xCC, 0x99,
             0x33, 0xCC, 0x99, 0x00, 0xFF, 0xCC, 0x66, 0x99, 0x66, 0x00, 0xFF, 0xCC, 0x99, 0x4A, 0x4E, 0xC9, 0x79, 0x00, 0x00, 0x00,
             0x01, 0x74, 0x52, 0x4E, 0x53, 0x00, 0x40, 0xE6, 0xD8, 0x66, 0x00, 0x00, 0x00, 0x01, 0x62, 0x4B, 0x47, 0x44, 0x00, 0x88,
             0x05, 0x1D, 0x48, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x0B, 0x12, 0x00, 0x00, 0x0B, 0x12, 0x01,
             0xD2, 0xDD, 0x7E, 0xFC, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4D, 0x45, 0x07, 0xDC, 0x01, 0x1D, 0x14, 0x23, 0x1C, 0x87,
             0xBC, 0xF0, 0x84, 0x00, 0x00, 0x00, 0x48, 0x49, 0x44, 0x41, 0x54, 0x08, 0xD7, 0x63, 0x60, 0x80, 0x03, 0x26, 0x25, 0x05,
             0x30, 0x95, 0x55, 0x3E, 0x0D, 0xC4, 0xD0, 0x31, 0x16, 0x14, 0x6C, 0x02, 0xD2, 0xC9, 0x86, 0x82, 0x82, 0xD2, 0x40, 0xE1,
             0xC5, 0x40, 0x3A, 0x44, 0x81, 0x81, 0xA9, 0x18, 0x46, 0x0B, 0x0A, 0x8A, 0xBA, 0x00, 0xE9, 0x60, 0x08, 0xCD, 0x90, 0x0C,
             0xA4, 0x9D, 0x80, 0xEA, 0x35, 0x80, 0xD2, 0x20, 0x9A, 0xA9, 0x53, 0xD0, 0x49, 0x01, 0xC9, 0x5C, 0x08, 0x00, 0x00, 0x9A,
             0x56, 0x0B, 0x58, 0x9D, 0x6A, 0x76, 0x99, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
             };

PROGMEM prog_char content_led_off_png_data[] = {
             0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x0E,
             0x00, 0x00, 0x00, 0x0E, 0x08, 0x04, 0x00, 0x00, 0x00, 0xB5, 0x41, 0xE5, 0x5A, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47,
             0x42, 0x00, 0xAE, 0xCE, 0x1C, 0xE9, 0x00, 0x00, 0x00, 0x02, 0x62, 0x4B, 0x47, 0x44, 0x00, 0xFF, 0x87, 0x8F, 0xCC, 0xBF,
             0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x0B, 0x12, 0x00, 0x00, 0x0B, 0x12, 0x01, 0xD2, 0xDD, 0x7E,
             0xFC, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4D, 0x45, 0x07, 0xDC, 0x01, 0x1D, 0x14, 0x21, 0x10, 0xBC, 0x3C, 0xDE, 0x2D,
             0x00, 0x00, 0x00, 0xD2, 0x49, 0x44, 0x41, 0x54, 0x18, 0xD3, 0x75, 0xD0, 0xB1, 0x4A, 0x03, 0x51, 0x10, 0x05, 0xD0, 0xB3,
             0x21, 0x1F, 0x10, 0x04, 0x3B, 0x53, 0x89, 0x5A, 0x09, 0x9B, 0x58, 0xD9, 0x25, 0x5F, 0x60, 0x63, 0x9F, 0x87, 0x5F, 0xE0,
             0x27, 0x6D, 0x7A, 0x5B, 0xC1, 0x6E, 0xF3, 0x01, 0x92, 0x2C, 0x36, 0x56, 0x62, 0x61, 0x23, 0xA4, 0xB1, 0x12, 0xF2, 0x64,
             0xDF, 0x5A, 0xBC, 0x20, 0x5B, 0x98, 0x3B, 0xC5, 0xC0, 0xBD, 0x73, 0xEF, 0x30, 0x53, 0x74, 0x0E, 0x63, 0x48, 0x01, 0xC2,
             0xBD, 0x1B, 0x33, 0xAC, 0x2C, 0xAB, 0x0A, 0x3A, 0x45, 0xA7, 0x10, 0x46, 0xEA, 0x69, 0x39, 0x71, 0x21, 0xD9, 0x78, 0xF5,
             0xD2, 0x98, 0x57, 0x5F, 0x9D, 0x21, 0xA8, 0xEF, 0xCA, 0x53, 0xDF, 0xB6, 0xA2, 0x23, 0xD7, 0xC6, 0xE5, 0x63, 0x6D, 0xC2,
             0x80, 0x10, 0xA6, 0xE5, 0xB9, 0x9D, 0xA4, 0xD5, 0x4A, 0xA2, 0x91, 0xB3, 0x32, 0x04, 0x06, 0x58, 0x5C, 0x89, 0x92, 0x56,
             0xFA, 0xAB, 0x63, 0x16, 0x59, 0x9C, 0x5D, 0xF6, 0xA4, 0xDC, 0x4F, 0x98, 0xC9, 0x3B, 0x33, 0xD5, 0x77, 0x46, 0x64, 0xE7,
             0xAA, 0xE9, 0xD1, 0x79, 0x68, 0xCB, 0x2A, 0x8B, 0xCB, 0x0D, 0xBD, 0xC8, 0x24, 0xF9, 0x60, 0x69, 0x7F, 0xE7, 0xE6, 0xB6,
             0x1C, 0x8B, 0xA2, 0x1F, 0xD1, 0xCE, 0xBB, 0x75, 0x53, 0x4D, 0x3A, 0x03, 0x30, 0x7F, 0x68, 0x9E, 0xBC, 0x29, 0xB4, 0x3E,
             0x3D, 0x5B, 0x37, 0xE6, 0xEC, 0x9D, 0x10, 0x82, 0xC5, 0x3F, 0xEF, 0x3B, 0x8C, 0x5F, 0xC2, 0x84, 0x6A, 0x7B, 0xFB, 0x42,
             0x44, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
             };

// declare tables for the images
PGM_P image_header PROGMEM = content_image_header;
PGM_P data_for_images [] PROGMEM = { content_favicon_data, content_led_on_png_data, content_led_off_png_data }; // real data
int   size_for_images [] PROGMEM = { sizeof(content_favicon_data), sizeof(content_led_on_png_data), sizeof(content_led_off_png_data) };
// declare table for all URIs
PGM_P http_uris[] PROGMEM = { http_uri1, http_uri2, http_uri3, http_uri4, http_uri5, http_uri6, http_uri7, http_uri8 }; // URIs


#else
PGM_P image_header PROGMEM = NULL;
PGM_P data_for_images [] PROGMEM = { }; // real data
int   size_for_images [] PROGMEM = { };
// declare table for all URIs
PGM_P http_uris[] PROGMEM = { http_uri1, http_uri2, http_uri3, http_uri4, http_uri5 }; // URIs

#endif  // USE_IMAGES

#define NUM_PAGES  sizeof(contents_pages)  / sizeof(PGM_P)
#define NUM_IMAGES sizeof(data_for_images) / sizeof(PGM_P)    // favicon or png format
#define NUM_URIS  NUM_PAGES + NUM_IMAGES  // Pages URIs + favicon URI, etc

/**********************************************************************************************************************
*                                                 Shared variable and Setup()
***********************************************************************************************************************/
EthernetServer server(80);

void setup()
{
#ifdef USE_DHCP_FOR_IP_ADDRESS
    Ethernet.begin(mac);  // Use DHCP to get an IP address
#else
    Ethernet.begin(mac, ip);
#endif

  server.begin();

  Serial.begin(9600); // DEBUG
  pinMode(ledPin, OUTPUT);
  setLed(true);
}

/**********************************************************************************************************************
*                                                           Main loop
***********************************************************************************************************************/

void loop()
{
  EthernetClient client = server.available();

  if (client)
  {
    // now client is connected to arduino we need to extract the
    // following fields from the HTTP request.
    int    nUriIndex;  // Gives the index into table of recognized URIs or -1 for not found.
    BUFFER requestContent;    // Request content as a null-terminated string.
    MethodType eMethod = readHttpRequest(client, nUriIndex, requestContent);

    Serial.print("Read Request type: ");
    Serial.print(eMethod);
    Serial.print(" Uri index: ");
    Serial.print(nUriIndex);
    Serial.print(" content: ");
    Serial.println(requestContent);

    if (nUriIndex < 0)
    {
      // URI not found
      sendProgMemAsString(client, (char*)pgm_read_word(&(page_404[0])));
    }
    else if (nUriIndex < NUM_PAGES)
    {
      // Normal page request, may depend on content of the request
      sendPage(client, nUriIndex, requestContent);
    }
    else
    {
      // Image request
      sendImage(client, nUriIndex, requestContent);
    }

    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}

/**********************************************************************************************************************
*                                              Method for read HTTP Header Request from web client
*
* The HTTP request format is defined at http://www.w3.org/Protocols/HTTP/1.0/spec.html#Message-Types
* and shows the following structure:
*  Full-Request and Full-Response use the generic message format of RFC 822 [7] for transferring entities. Both messages may include optional header fields
*  (also known as "headers") and an entity body. The entity body is separated from the headers by a null line (i.e., a line with nothing preceding the CRLF).
*      Full-Request   = Request-Line       
*                       *( General-Header 
*                        | Request-Header  
*                        | Entity-Header ) 
*                       CRLF
*                       [ Entity-Body ]    
*
* The Request-Line begins with a method token, followed by the Request-URI and the protocol version, and ending with CRLF. The elements are separated by SP characters.
* No CR or LF are allowed except in the final CRLF sequence.
*      Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
* HTTP header fields, which include General-Header, Request-Header, Response-Header, and Entity-Header fields, follow the same generic format.
* Each header field consists of a name followed immediately by a colon (":"), a single space (SP) character, and the field value.
* Field names are case-insensitive. Header fields can be extended over multiple lines by preceding each extra line with at least one SP or HT, though this is not recommended.     
*      HTTP-header    = field-name ":" [ field-value ] CRLF
***********************************************************************************************************************/
// Read HTTP request, setting Uri Index, the requestContent and returning the method type.
MethodType readHttpRequest(EthernetClient & client, int & nUriIndex, BUFFER & requestContent)
{
  BUFFER readBuffer;    // Just a work buffer into which we can read records
  int nContentLength = 0;
  bool bIsUrlEncoded;

  requestContent[0] = 0;    // Initialize as an empty string
  // Read the first line: Request-Line setting Uri Index and returning the method type.
  MethodType eMethod = readRequestLine(client, readBuffer, nUriIndex, requestContent);
  // Read any following, non-empty headers setting content length.
  readRequestHeaders(client, readBuffer, nContentLength, bIsUrlEncoded);

  if (nContentLength > 0)
  {
  // If there is some content then read it and do an elementary decode.
    readEntityBody(client, nContentLength, requestContent);
    if (bIsUrlEncoded)
    {
    // The '+' encodes for a space, so decode it within the string
    for (char * pChar = requestContent; (pChar = strchr(pChar, '+')) != NULL; )
      *pChar = ' ';    // Found a '+' so replace with a space
    }
  }

  return eMethod;
}

// Read the first line of the HTTP request, setting Uri Index and returning the method type.
// If it is a GET method then we set the requestContent to whatever follows the '?'. For a other
// methods there is no content except it may get set later, after the headers for a POST method.
MethodType readRequestLine(EthernetClient & client, BUFFER & readBuffer, int & nUriIndex, BUFFER & requestContent)
{
  MethodType eMethod;
  // Get first line of request:
  // Request-Line = Method SP Request-URI SP HTTP-Version CRLF
  getNextHttpLine(client, readBuffer);
  // Split it into the 3 tokens
  char * pMethod  = strtok(readBuffer, pSpDelimiters);
  char * pUri     = strtok(NULL, pSpDelimiters);
  char * pVersion = strtok(NULL, pSpDelimiters);
  // URI may optionally comprise the URI of a queryable object a '?' and a query
  // see http://www.ietf.org/rfc/rfc1630.txt
  strtok(pUri, "?");
  char * pQuery   = strtok(NULL, "?");
  if (pQuery != NULL)
  {
    strcpy(requestContent, pQuery);
    // The '+' encodes for a space, so decode it within the string
    for (pQuery = requestContent; (pQuery = strchr(pQuery, '+')) != NULL; )
      *pQuery = ' ';    // Found a '+' so replace with a space

//    Serial.print("Get query string: ");
//    Serial.println(requestContent);
  }
  if (strcmp(pMethod, "GET") == 0)
    eMethod = MethodGet;
  else if (strcmp(pMethod, "POST") == 0)
    eMethod = MethodPost;
  else if (strcmp(pMethod, "HEAD") == 0)
    eMethod = MethodHead;
  else
    eMethod = MethodUnknown;

  // See if we recognize the URI and get its index
  nUriIndex = GetUriIndex(pUri);

  return eMethod;
}

// Read each header of the request till we get the terminating CRLF
void readRequestHeaders(EthernetClient & client, BUFFER & readBuffer, int & nContentLength, bool & bIsUrlEncoded)
{
  nContentLength = 0;      // Default is zero in cate there is no content length.
  bIsUrlEncoded  = true;   // Default encoding
  // Read various headers, each terminated by CRLF.
  // The CRLF gets removed and the buffer holds each header as a string.
  // An empty header of zero length terminates the list.
  do
  {
    getNextHttpLine(client, readBuffer);
//    Serial.println(readBuffer); // DEBUG
    // Process a header. We only need to extract the (optionl) content
    // length for the binary content that follows all these headers.
    // General-Header = Date | Pragma
    // Request-Header = Authorization | From | If-Modified-Since | Referer | User-Agent
    // Entity-Header  = Allow | Content-Encoding | Content-Length | Content-Type
    //                | Expires | Last-Modified | extension-header
    // extension-header = HTTP-header
    //       HTTP-header    = field-name ":" [ field-value ] CRLF
    //       field-name     = token
    //       field-value    = *( field-content | LWS )
    //       field-content  = <the OCTETs making up the field-value
    //                        and consisting of either *TEXT or combinations
    //                        of token, tspecials, and quoted-string>
    char * pFieldName  = strtok(readBuffer, pSpDelimiters);
    char * pFieldValue = strtok(NULL, pSpDelimiters);

    if (strcmp(pFieldName, "Content-Length:") == 0)
    {
      nContentLength = atoi(pFieldValue);
    }
    else if (strcmp(pFieldName, "Content-Type:") == 0)
    {
      if (strcmp(pFieldValue, "application/x-www-form-urlencoded") != 0)
        bIsUrlEncoded = false;
    }
  } while (strlen(readBuffer) > 0);    // empty string terminates
}

// Read the entity body of given length (after all the headers) into the buffer.
void readEntityBody(EthernetClient & client, int nContentLength, BUFFER & content)
{
  int i;
  char c;

  if (nContentLength >= sizeof(content))
    nContentLength = sizeof(content) - 1;  // Should never happen!

  for (i = 0; i < nContentLength; ++i)
  {
    c = client.read();
//    Serial.print(c); // DEBUG
    content[i] = c;
  }

  content[nContentLength] = 0;  // Null string terminator

//  Serial.print("Content: ");
//  Serial.println(content);
}

// See if we recognize the URI and get its index; or -1 if we don't recognize it.
int GetUriIndex(char * pUri)
{
  int nUriIndex = -1;

  // select the page from the buffer (GET and POST) [start]
  for (int i = 0; i < NUM_URIS; i++)
  {
    if (strcmp_P(pUri, (PGM_P)pgm_read_word(&(http_uris[i]))) == 0)
    {
      nUriIndex = i;
      Serial.print("URI: ");
      Serial.println(pUri);
      break;
    }
  }
//  Serial.print("URI: ");
//  Serial.print(pUri);
//  Serial.print(" Page: ");
//  Serial.println(nUriIndex);

  return nUriIndex;
}

/**********************************************************************************************************************
* Read the next HTTP header record which is CRLF delimited.  We replace CRLF with string terminating null.
***********************************************************************************************************************/
void getNextHttpLine(EthernetClient & client, BUFFER & readBuffer)
{
  char c;
  int bufindex = 0; // reset buffer

  // reading next header of HTTP request
  if (client.connected() && client.available())
  {
    // read a line terminated by CRLF
    readBuffer[0] = client.read();
    readBuffer[1] = client.read();
    bufindex = 2;
    for (int i = 2; readBuffer[i - 2] != '\r' && readBuffer[i - 1] != '\n'; ++i)
    {
      // read full line and save it in buffer, up to the buffer size
      c = client.read();
      if (bufindex < sizeof(readBuffer))
        readBuffer[bufindex++] = c;
    }
    readBuffer[bufindex - 2] = 0;  // Null string terminator overwrites '\r'
  }
}

/**********************************************************************************************************************
*                                                              Send Pages
       Full-Response  = Status-Line         
                        *( General-Header   
                         | Response-Header 
                         | Entity-Header ) 
                        CRLF
                        [ Entity-Body ]   

       Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
       General-Header = Date | Pragma
       Response-Header = Location | Server | WWW-Authenticate
       Entity-Header  = Allow | Content-Encoding | Content-Length | Content-Type
                      | Expires | Last-Modified | extension-header
*
***********************************************************************************************************************/
void sendPage(EthernetClient & client, int nUriIndex, BUFFER & requestContent)
{
  // send HTML header
  // sendProgMemAsString(client,(char*)pgm_read_word(&(contents_main[CONT_HEADER])));
  sendProgMemAsString(client, (char*)pgm_read_word(&(contents_main[CONT_TOP])));

  // send menu
  sendProgMemAsString(client, (char*)pgm_read_word(&(contents_main[CONT_MENU])));

  // send title
  sendProgMemAsString(client, (char*)pgm_read_word(&(contents_titles[nUriIndex])));

  // send the body for the requested page
  sendUriContentByIndex(client, nUriIndex, requestContent);

  // Append the data sent in the original HTTP request
  client.print("<br />");
  // send POST variables
  client.print(requestContent);

  // send footer
  sendProgMemAsString(client,(char*)pgm_read_word(&(contents_main[CONT_FOOTER])));
}

/**********************************************************************************************************************
*                                                              Send Images
***********************************************************************************************************************/
void sendImage(EthernetClient & client, int nUriIndex, BUFFER & requestContent)
{
  int nImageIndex = nUriIndex - NUM_PAGES;

  // send the header for the requested image
  sendUriContentByIndex(client, nUriIndex, requestContent);

  // send the image data
  sendProgMemAsBinary(client, (char *)pgm_read_word(&(data_for_images[nImageIndex])), (int)pgm_read_word(&(size_for_images[nImageIndex])));
}

/**********************************************************************************************************************
*                                                              Send content split by buffer size
***********************************************************************************************************************/
// If we provide string data then we don't need specify an explicit size and can do a string copy
void sendProgMemAsString(EthernetClient & client, const char *realword)
{
  sendProgMemAsBinary(client, realword, strlen_P(realword));
}

// Non-string data needs to provide an explicit size
void sendProgMemAsBinary(EthernetClient & client, const char* realword, int realLen)
{
  int remaining = realLen;
  const char * offsetPtr = realword;
  int nSize = sizeof(BUFFER);
  BUFFER buffer;

  while (remaining > 0)
  {
    // print content
    if (nSize > remaining)
      nSize = remaining;      // Partial buffer left to send

    memcpy_P(buffer, offsetPtr, nSize);

    if (client.write((const uint8_t *)buffer, nSize) != nSize)
      Serial.println("Failed to send data");

    // more content to print?
    remaining -= nSize;
    offsetPtr += nSize;
  }
}

/**********************************************************************************************************************
*                                                              Send real page content
***********************************************************************************************************************/
// This method takes the contents page identified by nUriIndex, divides it up into buffer-sized
// strings, passes it on for STX substitution and finally sending to the client.
void sendUriContentByIndex(EthernetClient client, int nUriIndex, BUFFER & requestContent)
{
  // Locate the page data for the URI and prepare to process in buffer-sized chunks.
  const char * offsetPtr;               // Pointer to offset within URI for data to be copied to buffer and sent.
  char *pNextString;
  int nSubstituteIndex = -1;            // Count of substitutions so far for this URI
  int remaining;                        // Total bytes (of URI) remaining to be sent
  int nSize = sizeof(BUFFER) - 1;       // Effective size of buffer allowing last char as string terminator
  BUFFER buffer;

  if (nUriIndex < NUM_PAGES)
    offsetPtr = (char*)pgm_read_word(&(contents_pages[nUriIndex]));
  else
    offsetPtr = (char*)pgm_read_word(&(image_header));

  buffer[nSize] = 0;  // ensure there is always a string terminator
  remaining = strlen_P(offsetPtr);  // Set total bytes of URI remaining

  while (remaining > 0)
  {
    // print content
    if (nSize > remaining)
    {
      // Set whole buffer to string terminator before copying remainder.
      memset(buffer, 0, STRING_BUFFER_SIZE);
      nSize = remaining;      // Partial buffer left to send
    }
    memcpy_P(buffer, offsetPtr, nSize);
    offsetPtr += nSize;
    // We have a buffer's worth of page to check for substitution markers/delimiters.
    // Scan the buffer for markers, dividing it up into separate strings.
    if (buffer[0] == *pStxDelimiter)    // First char is delimiter
    {
      sendSubstitute(client, nUriIndex, ++nSubstituteIndex, requestContent);
      --remaining;
    }
    // First string is either terminated by the null at the end of the buffer
    // or by a substitution delimiter.  So simply send it to the client.
    pNextString = strtok(buffer, pStxDelimiter);
    client.print(pNextString);
    remaining -= strlen(pNextString);
    // Scan for strings between delimiters
    for (pNextString = strtok(NULL, pStxDelimiter); pNextString != NULL && remaining > 0; pNextString = strtok(NULL, pStxDelimiter))
    {
      // pNextString is pointing to the next string AFTER a delimiter
      sendSubstitute(client, nUriIndex, ++nSubstituteIndex, requestContent);
      --remaining;
      client.print(pNextString);
      remaining -= strlen(pNextString);
    }
  }
}

// Call this method in response to finding a substitution character '\002' within some
// URI content to send the appropriate replacement text, depending on the URI index and
// the substitution index within the content.
void sendSubstitute(EthernetClient client, int nUriIndex, int nSubstituteIndex, BUFFER & requestContent)
{
  if (nUriIndex < NUM_PAGES)
  {
    // Page request
    switch (nUriIndex)
    {
      case 1:  // page 2
        client.print("<b>Insert #");
        client.print(nSubstituteIndex);
        client.print("</b>");
        break;
      case 2:  // page 3
        switch (nSubstituteIndex)
        {
          case 0:  // LedOn button send value
            if (strncmp(requestContent, "LedOn=", 6) == 0)
              setLed(strncmp(&requestContent[6], "true", 4) == 0);

            client.print(isLedOn ? "false" : "true");
            break;
          case 1:  // LedOn button legend
            client.print(isLedOn ? "Off" : "On");
            break;
          case 2:  // LedOn partial image name
            client.print(isLedOn ? "on" : "off");
            break;
          default:
            break;
        }
        break;
    }
  }
  else
  {
    // Image request
    int nImageIndex = nUriIndex - NUM_PAGES;

    switch (nSubstituteIndex)
    {
      case 0:
        // Content-Length value - ie. image size
        char strSize[6];  // Up to 5 digits plus null terminator
        itoa((int)pgm_read_word(&(size_for_images[nImageIndex])), strSize, 10);
        Serial.println(strSize);    // Debug
        client.print(strSize);
        break;
      case 1:
        // Content-Type partial value
        switch (nImageIndex)
        {
          case 0:  // favicon
            client.print("x-icon");
            break;
          case 1:  // led on image
          case 2:  // led off image
            client.print("png");
            break;
        }
    }
  }
}

void setLed(bool isOn)
{
  isLedOn = isOn;
  digitalWrite(ledPin, isLedOn ? HIGH : LOW);
}

/**********************************************************************************************************************
*                                                          END OF CODE! WELL DONE!
***********************************************************************************************************************/

