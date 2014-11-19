/**
 * Matrix ausschalten
 */
void matrixOff() {
  if (matrix.done == false) {
    ColorFill(0, 0, 0);
    matrix.done = true;
  }
}



/**
 * Matrix Füllen
 */
void matrixFill() {
  if (matrix.done == false) {
    int bn = 0;
    bn = linearPWM((matrix.hsv.v > 100) ? 100 : matrix.hsv.v);
    ColorFill(bn&matrix.rgb.r, bn&matrix.rgb.g, bn&matrix.rgb.b);
    matrix.done = true;
  }
}
void matrixFillTest() {
  if (time+6 < millis()) {
    if (matrix.done == false) {
      unsigned int ready;
      
      ready = 0;
      PixelRGB *p = Colorduino.GetDrawPixel(0,0);
  
      for (char i = 0; i < 64; i++) {
        // Werte immer um 1 nähern
        (p->r == matrix.rgb.r) ? ready++ : ((p->r < matrix.rgb.r) ? p->r++ : p->r--);
        (p->g == matrix.rgb.g) ? ready++ : ((p->g < matrix.rgb.g) ? p->g++ : p->g--);
        (p->b == matrix.rgb.b) ? ready++ : ((p->b < matrix.rgb.b) ? p->b++ : p->b--);
          
        p++;
      }

      // Alle Werte geändert
      if (ready == 192) matrix.done = true;
    }
    time = millis();
  }
}



/**
 * Matrix Plasma
 */
void matrixPlasma() {
  plasma_morph();
}



/**
 * Matrix Aufblinken
 */
void matrixFlash() {
  if (time+matrix.speed < millis()) {
    flash(matrix.rgb.r, matrix.rgb.g, matrix.rgb.b);
    time = millis();
  }
}



/**
 * Matrix Regenbogen
 */
void matrixRainbow() {
  if (time+matrix.speed < millis()) {
    rainbow();
    time = millis();
  }
}
