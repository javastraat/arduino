
#include <EEPROM.h>
#include <Colorduino.h>
#include <String.h>
int debugmode = 0;

typedef struct
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} ColorRGB;

//a color with 3 components: h, s and v
typedef struct 
{
  unsigned char h;
  unsigned char s;
  unsigned char v;
} ColorHSV;

unsigned char plasma[ColorduinoScreenWidth][ColorduinoScreenHeight];
long paletteShift;
int addr = 0;
//PlotXY PlotData ;

void setup()
{
  randomSeed(millis());
  Serial.begin(9600);
  Colorduino.Init(); // initialize the board
  
  // compensate for relative intensity differences in R/G/B brightness
  // array of 6-bit base values for RGB (0~63)
  // whiteBalVal[0]=red
  // whiteBalVal[1]=green
  // whiteBalVal[2]=blue
  unsigned char whiteBalVal[3] = {36,63,63}; // for LEDSEE 6x6cm round matrix
  Colorduino.SetWhiteBal(whiteBalVal);
  
  
  // start with morphing plasma, but allow going to color cycling if desired.
  paletteShift=128000;
  unsigned char bcolor;

  //generate the plasma once
  for(unsigned char y = 0; y < ColorduinoScreenHeight; y++)
    for(unsigned char x = 0; x < ColorduinoScreenWidth; x++)
    {
      //the plasma buffer is a sum of sines
      bcolor = (unsigned char)
      (
            128.0 + (128.0 * sin(x*8.0 / 16.0))
          + 128.0 + (128.0 * sin(y*8.0 / 16.0))
      ) / 2;
      plasma[x][y] = bcolor;
    }
    
}

void loop()
{
  char getcmd ;
 if (Serial.available())
 {  

   getcmd = Serial.read() ;
     String tmps = String(getcmd) ;
   if (tmps.equalsIgnoreCase("@"))
         {
          VDraw(readHexByte(),readHexByte(),readHexByte(),readHexByte(),readHexByte()) ;
        if (debugmode == 1)
                Serial.println("Plot a point on RGB Screen");
                //P0302345678
                //P0102345678P0202345678P0302345678P0402345678P0502345678U
                //P0102345678P0202345638P0302FEDE23P0402234EFEP0502E323EEU
             }


     if (tmps.equalsIgnoreCase("X"))
        {
          plasma_morph() ;
          if (debugmode == 1)
                      Serial.println("Led plasma_morph");
              }
      
     if (tmps.equalsIgnoreCase("Z"))
         {
         RamdomRGBFill() ;
          if (debugmode == 1)
                Serial.println("Led RamdomRGBFill");
               }
      
     if (tmps.equalsIgnoreCase("L"))
         {
          LoadScreen() ;
          if (debugmode == 1)
                Serial.println("Led Screen  load from memory ");
               }
      
      if (tmps.equalsIgnoreCase("S"))
         {
          SaveScreen() ;
          if (debugmode == 1)
                Serial.println("Led Screen save into memory");
               }
      
       if (tmps.equalsIgnoreCase("C"))
         {
          ScrClear('B') ;
          if (debugmode == 1)
                Serial.println("Led Screen Clear");
               }
      
      if (tmps.equalsIgnoreCase("Q"))
         {
          debugShow() ;
          if (debugmode == 1)
                Serial.println("Debug data");
               }
   
      if (tmps.equalsIgnoreCase("M"))
         {
          OutputData() ;
          if (debugmode == 1)
                Serial.println("Output display data");
               }
   
      if (tmps.equalsIgnoreCase("N"))
         {
          OutputMemoryData() ;
          if (debugmode == 1)
                Serial.println("Output display Memory data");
               }


       if (tmps.equalsIgnoreCase("P"))
         {
          VDraw(readHexByte(),readHexByte(),readHexByte(),readHexByte(),readHexByte()) ;
        if (debugmode == 1)
                Serial.println("Plot a point on RGB Screen");
                //P0302345678
                //P0102345678P0202345678P0302345678P0402345678P0502345678U
             }
   
       if (tmps.equalsIgnoreCase("U"))
         {
            RefreshScreen() ;
          if (debugmode == 1)
                Serial.println("Update  RGB Screen");
                //P0302345678
                //P0302345678P0303345678P0102345678P0502345678P0304345678U
             }
   

 }
 delay(100);
}

// refresh RGB Led 
void RefreshScreen()
{
   Colorduino.FlipPage();

}

//Converts an HSV color to RGB color
void HSVtoRGB(void *vRGB, void *vHSV) 
{
  float r, g, b, h, s, v; //this function works with floats between 0 and 1
  float f, p, q, t;
  int i;
  ColorRGB *colorRGB=(ColorRGB *)vRGB;
  ColorHSV *colorHSV=(ColorHSV *)vHSV;

  h = (float)(colorHSV->h / 256.0);
  s = (float)(colorHSV->s / 256.0);
  v = (float)(colorHSV->v / 256.0);

  //if saturation is 0, the color is a shade of grey
  if(s == 0.0) {
    b = v;
    g = b;
    r = g;
  }
  //if saturation > 0, more complex calculations are needed
  else
  {
    h *= 6.0; //to bring hue to a number between 0 and 6, better for the calculations
    i = (int)(floor(h)); //e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999 becomes 4
    f = h - i;//the fractional part of h

    p = (float)(v * (1.0 - s));
    q = (float)(v * (1.0 - (s * f)));
    t = (float)(v * (1.0 - (s * (1.0 - f))));

    switch(i)
    {
      case 0: r=v; g=t; b=p; break;
      case 1: r=q; g=v; b=p; break;
      case 2: r=p; g=v; b=t; break;
      case 3: r=p; g=q; b=v; break;
      case 4: r=t; g=p; b=v; break;
      case 5: r=v; g=p; b=q; break;
      default: r = g = b = 0; break;
    }
  }
  colorRGB->r = (int)(r * 255.0);
  colorRGB->g = (int)(g * 255.0);
  colorRGB->b = (int)(b * 255.0);
}

float
dist(float a, float b, float c, float d) 
{
  return sqrt((c-a)*(c-a)+(d-b)*(d-b));
}




void plasma_morph()
{
  unsigned char x,y;
  float value;
  ColorRGB colorRGB;
  ColorHSV colorHSV;

  for(y = 0; y < ColorduinoScreenHeight; y++)
    for(x = 0; x < ColorduinoScreenWidth; x++) {
      {
	value = sin(dist(x + paletteShift, y, 128.0, 128.0) / 8.0)
	  + sin(dist(x, y, 64.0, 64.0) / 8.0)
	  + sin(dist(x, y + paletteShift / 7, 192.0, 64) / 7.0)
	  + sin(dist(x, y, 192.0, 100.0) / 8.0);
	colorHSV.h=(unsigned char)((value) * 128)&0xff;
	colorHSV.s=255; 
	colorHSV.v=255;
	HSVtoRGB(&colorRGB, &colorHSV);
	
	Colorduino.SetPixel(x, y, colorRGB.r, colorRGB.g, colorRGB.b);
      }
  }
  paletteShift++;

       RefreshScreen(); // swap screen buffers to show it
}

/********************************************************

Name: ColorFill
Function: Fill the frame with a color
Parameter:R: the value of RED.   Range:RED 0~255
          G: the value of GREEN. Range:RED 0~255
          B: the value of BLUE.  Range:RED 0~255
********************************************************/
void ColorFill(unsigned char R,unsigned char G,unsigned char B)
{
  PixelRGB *p = Colorduino.GetPixel(0,0);
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      p->r = R;
      p->g = G;
      p->b = B;
      p++;
    }
  }
  
       RefreshScreen();
}

void ScrClear(char cmd)
{
    PixelRGB *p = Colorduino.GetPixel(0,0);
    
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
        switch(cmd)
    {
      case 'B' :
        p->r = 0;
        p->g = 0;
        p->b = 0;
        break ;
        
      case 'W' :
        p->r = 255;
        p->g = 255;
        p->b = 255;
        break ;
        
    }   
      p++;
    }
  }
  
       RefreshScreen();
}

void Draw(int x, int y, unsigned char R,unsigned char G,unsigned char B)
{ 
  PixelRGB *p = Colorduino.GetPixel(x,y);
      p->r = R;
      p->g = G;
      p->b = B;
  
     RefreshScreen()  ;
}

void VDraw(int x, int y, unsigned char R,unsigned char G,unsigned char B)
{ 
  PixelRGB *p = Colorduino.GetPixel(x,y);
      p->r = R;
      p->g = G;
      p->b = B;
  
}

void DrawLine(int line, unsigned char R,unsigned char G,unsigned char B)
{
  PixelRGB *p = Colorduino.GetPixel(0,line);
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      p->r = R;
      p->g = G;
      p->b = B;
      p++;
    }
  
       RefreshScreen();
}

void DrawColumn(int column, unsigned char R,unsigned char G,unsigned char B)
{
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
      PixelRGB *p = Colorduino.GetPixel(column,y);
      p->r = R;
      p->g = G;
      p->b = B;
      p++;
  }
  
      RefreshScreen();
}
void RamdomRGBFill()
{
   PixelRGB *p = Colorduino.GetPixel(0,0);
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      RamdomRGB(p) ;
      p++;
    }
  }
  
       RefreshScreen(); 
  
}
void  RamdomRGB(PixelRGB *p)
{
     p->r = (byte)random(0,255);
      p->g = (byte)random(0,255);
      p->b = (byte)random(0,255);

}


PixelRGB  customcolor(char cmd)
{
    PixelRGB *p ;
    
  switch(cmd)
    {
      case 'B' :
        p->r = 0;
        p->g = 0;
        p->b = 0;
        break ;
        
      case 'W' :
        p->r = 255;
        p->g = 255;
        p->b = 255;
        break ;
      }   
 return *p;
}

void SaveScreen()
{
  int memaddr = 0 ;
   PixelRGB *p  = Colorduino.GetPixel(0,0); ;
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
    //  *p  = Colorduino.GetPixel(x,y);  
      EEPROM.write(memaddr,p->r)  ; 
      if (debugmode == 1)
      {
          Serial.print("Memory :("); 
          Serial.print(memaddr);
          Serial.print(")/("); 
          Serial.print(p->r,HEX);
          Serial.print(")/("); 
         Serial.print((byte)EEPROM.read(memaddr),HEX);
          Serial.println(")END"); 
      }
        
        memaddr ++ ;
        EEPROM.write(memaddr,p->g)  ;
      if (debugmode == 1)
      {
          Serial.print("Memory :("); 
          Serial.print(memaddr);
          Serial.print(")/("); 
          Serial.print(p->g,HEX);
          Serial.print(")/("); 
         Serial.print((byte)EEPROM.read(memaddr),HEX);
          Serial.println(")END");
      } 
        memaddr ++ ;
        EEPROM.write(memaddr,p->b)  ;
      if (debugmode == 1)
      {
          Serial.print("Memory :("); 
          Serial.print(memaddr);
          Serial.print(")/("); 
          Serial.print(p->b,HEX);
          Serial.print(")/("); 
         Serial.print((byte)EEPROM.read(memaddr),HEX);
          Serial.println(")END"); 
      }
        memaddr ++ ;
        p++;
    }
  }
  
}

void LoadScreen()
{
  int memaddr = 0 ;
   PixelRGB *p = Colorduino.GetPixel(0,0);
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
        p->r = (byte)EEPROM.read(memaddr)  ;
              if (debugmode == 1)
      {
          Serial.print("Memory :("); 
          Serial.print(memaddr);
          Serial.print(")/("); 
          Serial.print(p->r,HEX);
          Serial.print(")/("); 
         Serial.print((byte)EEPROM.read(memaddr),HEX);
          Serial.println(")END"); 
      }

        memaddr ++ ;
         p->g = (byte)EEPROM.read(memaddr)  ;
      if (debugmode == 1)
      {
          Serial.print("Memory :("); 
          Serial.print(memaddr);
          Serial.print(")/("); 
          Serial.print(p->g,HEX);
          Serial.print(")/("); 
         Serial.print((byte)EEPROM.read(memaddr),HEX);
          Serial.println(")END");
      } 
        memaddr ++ ;
         p->b = (byte)EEPROM.read(memaddr)  ;
      if (debugmode == 1)
      {
          Serial.print("Memory :("); 
          Serial.print(memaddr);
          Serial.print(")/("); 
          Serial.print(p->b,HEX);
          Serial.print(")/("); 
         Serial.print((byte)EEPROM.read(memaddr),HEX);
          Serial.println(")END"); 
      }
        memaddr ++ ;
      p++;
    }
  }
  
     RefreshScreen(); 
  
}

void OutputData()
{
   PixelRGB *p = Colorduino.GetPixel(0,0);
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
          Serial.print(strzero(p->r,2));
          Serial.print(strzero(p->g,2));
          Serial.print(strzero(p->b,2));
          p++;
    }
  }
  
}

void OutputMemoryData()
{
  int memaddr = 0 ;
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
         Serial.print(strzero((byte)EEPROM.read(memaddr),2));
          memaddr++;
         Serial.print(strzero((byte)EEPROM.read(memaddr),2));
          memaddr++;
         Serial.print(strzero((byte)EEPROM.read(memaddr),2));
          memaddr++;
    }
  }
  
}

void debugShow()
{
  int memaddr = 0 ;
   PixelRGB *p = Colorduino.GetPixel(0,0);
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
          Serial.print("Memory :("); 
          Serial.print(memaddr);
          Serial.print(")/in Led ("); 
          Serial.print(strzero(p->r,2));
          Serial.print(")/in Memory("); 
         Serial.print(strzero((byte)EEPROM.read(memaddr),2));
          Serial.println(")ENDD"); 

        memaddr ++ ;

          Serial.print("Memory :("); 
          Serial.print(memaddr);
          Serial.print(")/in Led("); 
          Serial.print(strzero(p->g,2));
          Serial.print(")/in Memory("); 
         Serial.print(strzero((byte)EEPROM.read(memaddr),2));
          Serial.println(")ENDD");

        memaddr ++ ;
 
          Serial.print("Memory :("); 
          Serial.print(memaddr);
          Serial.print(")/in Led("); 
          Serial.print(strzero(p->b,2));
          Serial.print(")/in Memory("); 
         Serial.print(strzero((byte)EEPROM.read(memaddr),2));
          Serial.println(")ENDD"); 

        memaddr ++ ;
        p++;
    }
  }
  
       RefreshScreen(); 
  
}


unsigned int readHexByte()
{
     String hexcode = String("0123456789ABCDEF") ;
 
  byte tmp1 = 0  ;
  byte tmp2 = 0  ;
  byte data1 = 0  ;
  byte data2 = 0  ;
   if (Serial.available())
      {
         tmp1 = Serial.read() ;
      }
      delay(50);
   if (Serial.available())
      {
         tmp2 = Serial.read() ;
      }
   if (tmp1 > 0)
      {
       data1 = hexcode.indexOf(tmp1) ;
      } 
   if (tmp2 > 0)
      {
       data2 = hexcode.indexOf(tmp2) ;
      } 
   if (debugmode == 1)
        {    
                 Serial.print("Read  byte is:(") ;
                 Serial.print(tmp1) ;
                 Serial.print(")/(") ;
                 Serial.print(tmp2) ;
                 Serial.print(")/(") ;
                 Serial.print(data1) ;
                 Serial.print(")/(") ;
                 Serial.print(data2) ;
                 Serial.print(")/(") ;
                 Serial.print(data1 * 16 + data2) ;
                 Serial.print(")") ;
                 Serial.println("") ;
         }    
  
  return (data1 * 16 + data2) ;
}



String strzero(long num, int len)
{
  String retstring = String("");
  int ln = 0 ;
    int i = 0 ;
    char tmp[10] ;
    long tmpnum ;
    int tmpchr = 0 ;
    char hexcode[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'} ;
    while (ln < len)
    {
       tmpnum = (long)(num/(long)pow(16,ln)) ;
        tmpchr = (int)(tmpnum % 16) ;
        tmp[ln] = hexcode[tmpchr] ;
/*
        Serial.print("tran :(");
        Serial.print(ln);
        Serial.print(")/(");
        Serial.print(hexcode[tmpchr] );
        Serial.print(")/(");
        Serial.print(tmpchr);
        Serial.println(")");
        */
        
                ln++ ;
    }
    for (i = len-1; i >= 0 ; i --)
      {
          retstring.concat(tmp[i]);
      }
    
  return retstring;
}

unsigned long unstrzero(String hexstr)
{
  String chkstring  ;
  int len = hexstr.length() ;
  if (debugmode == 1)
      {
          Serial.print("String ");
          Serial.println(hexstr);
          Serial.print("len:");
          Serial.println(len);
      }
    unsigned int i = 0 ;
    unsigned int tmp = 0 ;
    unsigned int tmp1 = 0 ;
    unsigned long tmpnum = 0 ;
    String hexcode = String("0123456789ABCDEF") ;
    for (i = 0 ; i < (len ) ; i++)
    {
  //     chkstring= hexstr.substring(i,i) ; 
      hexstr.toUpperCase() ;
           tmp = hexstr.charAt(i) ;   // give i th char and return this char
           tmp1 = hexcode.indexOf(tmp) ;
      tmpnum = tmpnum + tmp1* POW(16,(len -i -1) )  ;
 
      if (debugmode == 1)
      {
            Serial.print("char:( ");
          Serial.print(i);
            Serial.print(")/(");
          Serial.print(hexstr);
            Serial.print(")/(");
          Serial.print(tmpnum);
            Serial.print(")/(");
          Serial.print((long)pow(16,(len -i -1)));
            Serial.print(")/(");
          Serial.print(pow(16,(len -i -1) ));
            Serial.print(")/(");
          Serial.print((char)tmp);
            Serial.print(")/(");
          Serial.print(tmp1);
            Serial.print(")");
            Serial.println("");
      }
    }
  return tmpnum;
}

long POW(long num, int expo)
{
  long tmp =1 ;
  if (expo > 0)
  { 
        for(int i = 0 ; i< expo ; i++)
          tmp = tmp * num ;
         return tmp ; 
  } 
  else
  {
   return tmp ; 
  }
}
