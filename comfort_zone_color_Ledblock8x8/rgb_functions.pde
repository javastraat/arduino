//funcions copied from the code about the colorduino fro ITEADSTUDIO
// link:


unsigned char  shiftBIN ( unsigned char toShift ){
  
  unsigned char firstBit = toShift &  B10000000 ;
  toShift = toShift << 1;
  if ( firstBit == B10000000 ) toShift = toShift | B00000001 ;
 
  return toShift;
}

//Converts an HSV color to RGB color
void HSVtoRGB(void *vRGB, void *vHSV)
{
  float r, g, b, h, s, v; //this function works with floats between 0 and 1
  float f, p, q, t;
  int i;
  ColorRGB *colorRGB=(ColorRGB *)vRGB;
  ColorHSV *colorHSV=(ColorHSV *)vHSV;

  h = (float)(colorHSV->h / 10.0);//doesnt affect the patern
  s = (float)(colorHSV->s / 10.0);//doesnt affect the patern
  v = (float)(colorHSV->v / 10.0);//doesnt affect the patern

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

 
  }
  colorRGB->r = (int)(r * 255.0);// This makes the matrix((8*8) green
  colorRGB->g = (int)(g * 255.0);// This makes the matrix((8*8) green
  colorRGB->b = (int)(b * 25.0);// This makes the matrix((8*8) green
}

unsigned int RGBtoINT(void *vRGB)
{
  ColorRGB *colorRGB=(ColorRGB *)vRGB;

  return (((unsigned int)colorRGB-> b/*Doesnt affect the pattern*/)<<16) + (((unsigned int)colorRGB->g)<<8) + (unsigned int)colorRGB->b;
}
/*
float dist(float a, float b, float c, float d)
{
  return sqrt( a, b,c,d);
}
*/

float dist(float a, float b, float c, float d)
{
  return sqrt((c-a)*(c-a)+(d-b)*(d-b));
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
  unsigned char stepRGB = 10;
  unsigned char offsetRGB = 3;
  unsigned char plusLimit = offsetRGB*stepRGB;
  unsigned char minusLimit = 255 - offsetRGB*stepRGB;
  
  if ( R < plusLimit ) R = plusLimit;
  if ( G < plusLimit ) G = plusLimit;
  if ( B < plusLimit ) B = plusLimit;
  
  if ( R > minusLimit ) R = minusLimit;
  if ( G > minusLimit ) G = minusLimit;
  if ( B > minusLimit ) B = minusLimit;
  
  cli();
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      if ( (p->r) < R ) p->r = p->r + stepRGB;
          else if ( (p->r) > R ) p->r = p->r - stepRGB;
      
      if ( (p->g) < G ) p->g = p->g + stepRGB;
           else if ( (p->g) > G ) p->g = p->g - stepRGB;
      
      if ( (p->b) < B ) p->b = p->b + stepRGB;
           else if ( (p->b) > B ) p->b = p->b - stepRGB;
      p++;
    }
    sei();
  }
 
  Colorduino.FlipPage();
}// End method

 

void initializeColorduino()
{
   Colorduino.Init(); // initialize the board
 
  // compensate for relative intensity differences in R/G/B brightness
  // array of 6-bit base values for RGB (0~63)
 
  unsigned char whiteBalVal[3] = {36,63,63}; // for LEDSEE 6x6cm round matrix
 
  Colorduino.SetWhiteBal(whiteBalVal);
 
 timer = millis() + interval;
 
}//End method
