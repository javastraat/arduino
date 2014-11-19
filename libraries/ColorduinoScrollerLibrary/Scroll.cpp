#include <Adafruit_GFX.h>
#include <Colorduino_GFX.h>
#include <Scroll.h>
ColorduinoPanel Colorduino; 




Scroll::Scroll(String text, int scrollspeed, int tr, int tg, int tb, int br, int bg, int bb, int displaynumber){

  int startdelay = scrollspeed*8*displaynumber;
  randomSeed(analogRead(0));

  Colorduino.init();


  Colorduino.setWhiteBalance(36, 63, 63); // Set white balance
  delay(startdelay);


  int numberofcharacters = text.length();
  for(int xv = 10; xv > -((numberofcharacters+1)*6) ; xv--){

    GFX_Color_t textcolor = Colorduino.color(tr, tg, tb);//Color of text to scroll Default: White
    GFX_Color_t textBG = Colorduino.color(br, bg, bb);//Color of text background Default: Black/Nothing

    char char1 = text.charAt(0);
    char char2 = text.charAt(1);
    char char3 = text.charAt(2);
    char char4 = text.charAt(3);
    char char5 = text.charAt(4);
    char char6 = text.charAt(5);
    char char7 = text.charAt(6);
    char char8 = text.charAt(7);
    char char9 = text.charAt(8);
    char char10 = text.charAt(9);
    char char11 = text.charAt(10);
    char char12 = text.charAt(11);
    char char13 = text.charAt(12);
    char char14 = text.charAt(13);
    char char15 = text.charAt(14);
    char char16 = text.charAt(15);
    char char17 = text.charAt(16);
    char char18 = text.charAt(17);
    char char19 = text.charAt(18);
    char char20 = text.charAt(19);
    

   
      Colorduino.drawChar(xv+6*1, 1, char1, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*2, 1, char2, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*3, 1, char3, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*4, 1, char4, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*5, 1, char5, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*6, 1, char6, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*7, 1, char7, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*8, 1, char8, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*9, 1, char9, textcolor, textBG, 1); 
      Colorduino.drawChar(xv+6*10, 1, char10, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*11, 1, char11, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*12, 1, char12, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*13, 1, char13, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*14, 1, char14, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*15, 1, char15, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*16, 1, char16, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*17, 1, char17, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*18, 1, char18, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*19, 1, char19, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*20, 1, char20, textcolor, textBG, 1);  
      Colorduino.drawChar(xv+6*20, 1, ' ', textcolor, textBG, 1); 
      
      Colorduino.swapBuffers(true);
    delay(scrollspeed); 

    }



  }


