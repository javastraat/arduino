/*
	three animations
*/
int three() {
  byte animation1[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B10000000
  };

  byte animation2[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11000000
  };

  byte animation3[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11100000
  };


  bool skip = render(animation1, 500);
  if (skip) {
    return true;
  }
  skip = render(animation2, 500);
  if (skip) {
    return true;
  } skip = render(animation3, 500);
  if (skip) {
    return true;
  }
  return false;
}
