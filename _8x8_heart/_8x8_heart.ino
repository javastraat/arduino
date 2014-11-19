int CLOCK = 11;
int LATCH = 12;
int DATA  = 10;
byte matrix[8];
int idx = 0;

void setup() {
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(DATA,  OUTPUT);
  digitalWrite(CLOCK, LOW);
  digitalWrite(LATCH, LOW);
  digitalWrite(DATA,  LOW);
  initLED();
  clearLED();
}

void loop() {
  for (int j=0;j<8;j++) {
    updateLED(idx, j, true);
  }
  refreshLED();
  delay(200);
  for (int j=0;j<8;j++) {
    updateLED(idx, j, false);
  }
  refreshLED();
  delay(100);
  idx++;
  idx %= 8;
}

void ledOut(int n) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, (n>>8));
  shiftOut(DATA, CLOCK, MSBFIRST, (n));
  digitalWrite(LATCH, HIGH);
  delay(1);
  digitalWrite(LATCH, LOW);
}

void initLED() {
  ledOut(0x0B07);
  ledOut(0x0A0C);
  ledOut(0x0900);
  ledOut(0x0C01);
}

void clearLED() {
  for (int i=0;i<8;i++) {
    matrix[i] = 0x00;
  }
  refreshLED();
}

void refreshLED() {
  int n1, n2, n3;
  for (int i=0;i<8;i++) {
    n1 = i+1;
    n2 = matrix[i];
    n3 = (n1<<8)+n2;
    ledOut(n3);
  }
}

void updateLED(int i, int j, boolean b) {
  int t = 1;
  int n = 0;
  int m = 0;
  if (j==0) {
    m = 7;
  }
  else {
    m = j-1;
  }
  n = t<<m;
  if (b) {
    matrix[i] = n | matrix[i];
  }
  else {
    n = ~n;
    matrix[i] = n & matrix[i];
  }
}

