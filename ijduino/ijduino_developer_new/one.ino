/*
	one animation
*/
int one() {
  byte animation1[8] = {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B10000000
  };



  bool skip = render(animation1, 500);
  if (skip) {
    return true;
  }

  return false;
}
