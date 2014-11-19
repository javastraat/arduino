/*
	twelve animations
*/
int twelve() {
  byte animation1[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B10000000
  };

  byte animation2[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11000000
  };

  byte animation3[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11100000
  };

  byte animation4[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11110000
  };

  byte animation5[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111000
  };

  byte animation6[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111100
  };

  byte animation7[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110
  };
  byte animation8[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111111
  };
  byte animation9[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B10000000, B11111111
  };
  byte animation10[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11000000, B11111111
  };
  byte animation11[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11100000, B11111111
  };
  byte animation12[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111
  };

  bool skip = render(animation1, 500);
  if (skip) {
    return true;
  }
  skip = render(animation2, 500);
  if (skip) {
    return true;
  }
  skip = render(animation3, 500);
  if (skip) {
    return true;
  }
  skip = render(animation4, 500);
  if (skip) {
    return true;
  }
  skip = render(animation5, 500);
  if (skip) {
    return true;
  }
  skip = render(animation6, 500);
  if (skip) {
    return true;
  }
  skip = render(animation7, 500);
  if (skip) {
    return true;
  }
  skip = render(animation8, 500);
  if (skip) {
    return true;
  }
  skip = render(animation9, 500);
  if (skip) {
    return true;
  }
  skip = render(animation10, 500);
  if (skip) {
    return true;
  }
  skip = render(animation11, 500);
  if (skip) {
    return true;
  }
  skip = render(animation12, 500);
  if (skip) {
    return true;
  }
  return false;
}
