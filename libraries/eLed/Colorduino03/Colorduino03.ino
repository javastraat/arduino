
#include <EEPROM.h>
#include <Colorduino.h>

#define userfontmax 13 

byte userfont[][8] =
{{B00000000,
B01100110,
B10011001,
B10000001,
B10000001,
B01000010,
B00100100,
B00011000},
// heart icon
{B00011000,
B00011000,
B00011000,
B11111111,
B00011000,
B00011000,
B00011000,
B00011000}
//T shape
,
{B00011000,
B00100100,
B01000010,
B10000001,
B10000001,
B11111111,
B10000001,
B10000001}
// A 
,
{B11111110,
B10000001,
B10000001,
B10000010,
B11111110,
B10000010,
B10000001,
B11111110}
//  B
,
{B00111100,
B01000010,
B10000000,
B10000000,
B10000000,
B10000001,
B01000010,
B00111100}
//  c
,
{B00111100,
B01000010,
B10000000,
B10000000,
B10001110,
B10000010,
B01000110,
B00111010}
//  G
,
{B11111100,
B10000010,
B10000010,
B10000010,
B11111100,
B10001000,
B10000100,
B10000010}
// R
,
{B10000001,
B01000010,
B00100100,
B00011000,
B00011000,
B00100100,
B01000010,
B10000001}
// X
,
{B10000001,
B01000010,
B00100100,
B00011000,
B00011000,
B00011000,
B00011000,
B00011000}
// Y
,
{B11111110,
B00000010,
B00000100,
B00001000,
B00010000,
B00100000,
B01000000,
B11111111}
//  z
,
{B11111000,
B10000100,
B10000010,
B10000010,
B10000010,
B10000010,
B10000100,
B11111000}
// D
,
{B11111110,
B10000000,
B10000000,
B11111110,
B11111110,
B10000000,
B10000000,
B11111110}
//  E
,
{B11111110,
B10000000,
B10000000,
B11111100,
B10000000,
B10000000,
B10000000,
B10000000}
//F
} ;
  

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
    
 // to adjust white balance you can uncomment this line
 // and comment out the plasma_morph() in loop()
 // and then experiment with whiteBalVal above
 // ColorFill(255,255,255);
}

void loop()
{
  char getcmd ;
 if (Serial.available())
 {  
     getcmd = Serial.read() ;
     
     switch (getcmd)
     {
         case 'X' :
          plasma_morph() ;
          Serial.println("Led plasma_morph");
          break ;
      
         case 'Z' :
          RamdomRGBFill() ;
          Serial.println("Led RamdomRGBFill");
          break ;
      
         case 'C' :
          ScrClear('B') ;
          Serial.println("Led Screen Clear");
          break ;
      
         case 'U' :
          UsermChar(random(0,userfontmax)) ;
          Serial.println("Led USER FNT
          break ;
      
       
     }
 }
  delay(1000);
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

  Colorduino.FlipPage(); // swap screen buffers to show it
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
  
  Colorduino.FlipPage();
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
  
  Colorduino.FlipPage();
}

void Draw(int x, int y, unsigned char R,unsigned char G,unsigned char B)
{ 
  PixelRGB *p = Colorduino.GetPixel(x,y);
      p->r = R;
      p->g = G;
      p->b = B;
  
    Colorduino.FlipPage();
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
  
  Colorduino.FlipPage();
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
  
  Colorduino.FlipPage();
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
  
  Colorduino.FlipPage(); 
  
}
void  RamdomRGB(PixelRGB *p)
{
     p->r = (byte)random(0,255);
      p->g = (byte)random(0,255);
      p->b = (byte)random(0,255);

}

void UsermChar(int usrf)
{
   PixelRGB *p = Colorduino.GetPixel(0,0);
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      if (bitRead(userfont[usrf][y],x) == 1)
            {
                RamdomRGB(p) ;
            }
            else
            {
               *p= customcolor('B') ;
            }
            
      p++;
    }
  }

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





