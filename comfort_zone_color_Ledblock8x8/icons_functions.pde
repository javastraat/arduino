//******************************BEGIN SMILEY*****************************************************
void smiley(int r, int g, int b, unsigned char num)
{
 
 unsigned char x,y;
 //alternation of two smileys
  for(y = 0; y < ColorduinoScreenHeight; y++)
    for(x = 0; x < ColorduinoScreenWidth; x++)  
      {
        if ( num == 0){
            if ( (( smileyArray0[x] >> y ) & (B00000001)) == 1 )
      	      Colorduino.SetPixel(x,y, (millis()/103)% 200 + 54, (millis()/273)% 200 + 54, (millis()/73)% 200 + 54);
              else
               Colorduino.SetPixel( x,y, (millis()/33)% 10  , (millis()/ 73)% 10  , (millis()/11)% 10  );
              }
        else
        {
             if ( (( smileyArray1[x] >> y ) & (B00000001)) == 1 )
      	      Colorduino.SetPixel(x,y, (millis()/103)% 200 + 54, (millis()/273)% 200 + 54, (millis()/73)% 200 + 54);
              else
               Colorduino.SetPixel( x,y, (millis()/33)% 10  , (millis()/ 73)% 10  , (millis()/11)% 10  );
              }
          
        }
 
  Colorduino.FlipPage(); // swap screen buffers to show it
  
}//End method
//******************************END SMILEY*****************************************************

 

//**********************BEGIN HEART**************************************************************
void setHeart()
{
  heart( 255*sin(millis()/3000), 255*cos(millis()/3000), 255*tan(millis()/3000));
}

void heart(int r, int g, int b)
{

 unsigned char x,y;
 
 //outer side of heart
  for(y = 0; y < ColorduinoScreenHeight; y++)
    for(x = 0; x < ColorduinoScreenWidth; x++)  
      {
      if ( (( heartArray1[x] >> y ) & (B00000001)) == 1 )
	Colorduino.SetPixel(x,y, (millis()/103)% 200 + 54, (millis()/273)% 200 + 54, (millis()/73)% 200 + 54);
        else
         Colorduino.SetPixel( x,y, (millis()/33)% 10  , (millis()/ 73)% 10  , (millis()/11)% 10  );
   }
   
   //inner side of heart
    for(y = 0; y < ColorduinoScreenHeight; y++)
    for(x = 0; x < ColorduinoScreenWidth; x++)  
      {
      if ( (( heartArray2[x] >> y ) & (B00000001)) == 1 )
	Colorduino.SetPixel(x,y, (millis()/33)% 100  , (millis()/ 73)% 100  , (millis()/11)% 100  );
   }
 
  Colorduino.FlipPage(); // swap screen buffers to show it 
 
}//End method
//*********************END HEART***************************************


//*********************START RAIN***************************************
void rain(int r, int g, int b)
{

 unsigned char x,y;
 
  ColorRGB colorRGB;
  ColorHSV colorHSV;
  
  //shifting one position on the block of 8x8 LED's
  for(y = 0; y < ColorduinoScreenHeight; y++)
    for(x = 0; x < ColorduinoScreenWidth; x++)  
      {
      if ( (( rainArray[x] >> y ) & (B00000001)) == 1 )
	Colorduino.SetPixel(x,y, (millis()/103)% 200 + 54, (millis()/273)% 200 + 54, (millis()/73)% 200 + 54);
        else
         Colorduino.SetPixel( x,y, (millis()/33)% 10  , (millis()/ 73)% 10  , (millis()/11)% 10  );
   }

  Colorduino.FlipPage(); // swap screen buffers to show it 
 
}//End method


//**********************BEGIN NOISE 1**************************************************************
void setNoise0(int r, int g, int b)
{

   for ( int i = 0; i<8 ; i++ ) 
      for ( int j = 0; j<8 ; j++ ) 
    {
      if ( (( noiseArray0[i] >> j ) & (B00000001)) == 1 )
        Colorduino.SetPixel( 7 -i,  j, r,g,b);
        else
         Colorduino.SetPixel( 7 -i,  j, 0,0,0);
   }
 
  Colorduino.FlipPage(); // swap screen buffers to show it 
 
}//End method

//**********************BEGIN NOISE 2**************************************************************
void setNoise1(int r, int g, int b)
{
 

   for ( int i = 0; i<8 ; i++ ) 
      for ( int j = 0; j<8 ; j++ ) 
    {
      if ( (( noiseArray1[i] >> j ) & (B00000001)) == 1 )
        Colorduino.SetPixel( 7 -i,  j, r,g,b);
        else
         Colorduino.SetPixel( 7 -i,  j, 0,0,0);
   }
 
  Colorduino.FlipPage(); // swap screen buffers to show it 
 
}//End method
 
 
 void sun(int r, int g, int b, unsigned char num)
{
 
 unsigned char x,y;
 
  for(y = 0; y < ColorduinoScreenHeight; y++)
    for(x = 0; x < ColorduinoScreenWidth; x++)  
      {
        if ( num == 0){
            if ( (( sun0[x] >> y ) & (B00000001)) == 1 )
      	      Colorduino.SetPixel(x,y, (millis()/103)% 200 + 54, (millis()/273)% 200 + 54, (millis()/73)% 200 + 54);
              else
               Colorduino.SetPixel( x,y, (millis()/33)% 10  , (millis()/ 73)% 10  , (millis()/11)% 10  );
              }
        else
        {
             if ( (( sun1[x] >> y ) & (B00000001)) == 1 )
      	      Colorduino.SetPixel(x,y, (millis()/103)% 200 + 54, (millis()/273)% 200 + 54, (millis()/73)% 200 + 54);
              else
               Colorduino.SetPixel( x,y, (millis()/33)% 10  , (millis()/ 73)% 10  , (millis()/11)% 10  );
              }
          
        }
 
  Colorduino.FlipPage(); // swap screen buffers to show it
  
}
