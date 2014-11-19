int linearPWM(int percentage){
  // coefficients
  double a = 9.7758463166360387E-01;
  double b = 5.5498961535023345E-02;
  
  return floor((a * exp(b*percentage)+.5))-1;
}


void flash(byte r, byte g, byte b) {
  int bn = 0;
  
  if (matrix.number > 0) {
    bn = (matrix.number%2) ? linearPWM((int)100-counter) : linearPWM((int)counter);

    ColorFill(bn&r, bn&g, bn&b);
    
    if (counter == 100) {
      counter = 0;
      matrix.number--;
    }
  } else {
    matrix = matrixTemp;
  }
  
  counter++;
}


void rainbow() {
  ColorRGB colorRGB;
  ColorHSV colorHSV;
  
  colorHSV.s = matrix.hsv.s;
  colorHSV.v = matrix.hsv.v;
  
  PixelRGB *p = Colorduino.GetPixel(0,0);
  for (byte i = 0; i < 8; i++) {
    colorHSV.h = counter + i*matrix.effect;
    HSVtoRGB(&colorRGB, &colorHSV);
    
    for (byte j = 0; j < 8; j++) {
      p->r = colorRGB.r;
      p->g = colorRGB.g;
      p->b = colorRGB.b;
      p++;
    }
  }
  
  Colorduino.FlipPage();
  counter++;
}
