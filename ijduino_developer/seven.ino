/*
	seven animations
*/
int seven() {
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
  return false;
}
