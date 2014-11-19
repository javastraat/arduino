/**
 * Ping Pong with 8x8 Led Dot Matrix on Arduino
 *
 * @author  Bruno Soares
 * @website www.bsoares.com.br
 */

#include "TimerOne.h"

#define PIN_LEFT 4
#define PIN_RIGHT 5

unsigned int left = 0;
unsigned int right = 0;
int angle = 0;
int radians;

byte rows[8] = {9, 14, 8, 12, 1, 7, 2, 5};
byte cols[8] = {13, 3, 4, 10, 6, 11, 15, 16};
byte pins[16] = {5, 4, 3, 2, 14, 15, 16, 17, 13, 12, 11, 10, 9, 8, 7, 6};
byte screen[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile byte screenRow = 0;
volatile byte screenCol = 0;

int _angle;
int _px;
int _py;
int _w = 7;
int _h = 7;
int _wall[] = {3, 3};
int _count = 0;
int _speed = 3;
int _countPoints = 0;


void setup() {
  Timer1.initialize(100);
  for (int i = 2; i <= 17; i++)
    pinMode(i, OUTPUT);
  Timer1.attachInterrupt(doubleBuffer);
  
  Serial.begin(9600);
  
  face();
  reset();
}

void doubleBuffer() {
  digitalWrite(translatePin(rows[screenRow]), LOW);
  digitalWrite(translatePin(cols[screenCol]), HIGH);
  
  screenCol++;
  if (screenCol >= 8) {
    screenCol = 0;
    screenRow++;
    if (screenRow >= 8) {
      screenRow = 0;
    }
  }
  
  if((screen[screenRow] >> screenCol) & B1 == B1) {
    digitalWrite(translatePin(rows[screenRow]), HIGH);
    digitalWrite(translatePin(cols[screenCol]), LOW);
  } else {
    digitalWrite(translatePin(rows[screenRow]), LOW);
    digitalWrite(translatePin(cols[screenCol]), HIGH);
  }
}

byte translatePin(byte original) {
  return pins[original - 1];
}

void allOFF() {
  for (int i = 0; i < 8; i++)
    screen[i] = 0;
}

void on(byte row, byte column) {
    screen[column-1] |= (B1 << (row - 1));
}

void off(byte row, byte column) {
    screen[column-1] &= ~(B1 << (row - 1));
}

void calcWall()
{
  left = analogRead(PIN_LEFT);
  right = analogRead(PIN_RIGHT);
  left = constrain(map(left, 223, 800, 0, 6), 0, 6);
  right = constrain(map(right, 223, 800, 6, 0), 0, 6);
  
  clearWall();
  
  on(1, left + 1);
  on(1, left + 2);
  on(8, right + 1);
  on(8, right + 2);
  
  _wall[0] = left;
  _wall[1] = right;
  show();
}

void clearWall()
{
  for (int i = 0; i < 8; i++)
    screen[i] &= B01111110;
}

void clearGame()
{
  for (int i = 0; i < 8; i++)
    screen[i] &= B10000001;
}

void loop() {
  calcWall();
  enterFrameHandler();
  delay(50);
}

void enterFrameHandler()
{
  if (_count++ < _speed)
    return;
  
  _count = 0;
  checkCollision();
  calcAngleIncrement();
  show();
}

void retorted(int angle)
{
  Serial.println(angle);
  _angle = angle;
  
  if (++_countPoints % 5 == 0 && _speed > 1)
    _speed--;
}

void resetAnim()
{
  for (int i = 0; i < 8; i++)
  {
    screen[i] = B11111111;
    delay(25);
  }
  for (int i = 0; i < 8; i++)
  {
    screen[i] = B00000000;
    delay(25);
  }
}

void face()
{
  on(1, 1);
  on(1, 2);
  on(2, 1);
  on(2, 2);
  on(7, 1);
  on(7, 2);
  on(8, 1);
  on(8, 2);
  on(1, 1);
  on(1, 2);
  on(4, 4);
  on(4, 5);
  on(5, 4);
  on(5, 5);
  on(2, 7);
  on(7, 7);
  on(3, 8);
  on(4, 8);
  on(5, 8);
  on(6, 8);
  delay(5000);
}

void reset()
{
  resetAnim();
  
  _px = random(3, 5);
  _py = random(3, 5);
  _angle = random(0, 2) == 0 ? 0 : 180;
  _speed = 5;
  _countPoints = 0;
  
  show();
  delay(500);
}

void show()
{
  clearGame();
  on(_px + 1, _py + 1);
}

void checkCollision()
{
  if (_px == _w - 1)
  {
    if (_angle == 315 || _angle == 0 || _angle == 45)
    {
      if (_py == _wall[1] || _py == _wall[1] + 1)
      {
        if (_angle == 0 && _py == _wall[1])
          retorted(225);
        else if (_angle == 0 && _py == _wall[1] + 1)
          retorted(135);
        else if (_angle == 45 && _py == _wall[1])
          retorted(135);
        else if (_angle == 45 && _py == _wall[1] + 1)
          retorted(180);
        else if (_angle == 315 && _py == _wall[1])
          retorted(180);
        else if (_angle == 315 && _py == _wall[1] + 1)
          retorted(225);
      }
    }
  }
  else if (_px == 1)
  {
    if (_angle == 225 || _angle == 180 || _angle == 135)
    {
      if (_py == _wall[0] || _py == _wall[0] + 1)
      {
        if (_angle == 180 && _py == _wall[0])
          retorted(315);
        else if (_angle == 180 && _py == _wall[0] + 1)
          retorted(45);
        else if (_angle == 135 && _py == _wall[0])
          retorted(45);
        else if (_angle == 135 && _py == _wall[0] + 1)
          retorted(0);
        else if (_angle == 225 && _py == _wall[0])
          retorted(0);
        else if (_angle == 225 && _py == _wall[0] + 1)
          retorted(315);
      }
    }
  }
  
  if (_px == _w)
  {
    reset();
  }
  else if (_px == 0)
  {
    reset();
  }
  else if (_py == _h)
  {
    if (_angle == 45)
      _angle = 315;
    else if (_angle == 135)
      _angle = 225;
  }
  else if (_py == 0)
  {
    if (_angle == 225)
      _angle = 135;
    else if (_angle == 315)
      _angle = 45;
  }
}

void calcAngleIncrement()
{
  if (_angle == 0 || _angle == 360)
  {
    _px += 1;
  }
  else if (_angle == 45)
  {
    _px += 1;
    _py += 1;
  }
  else if (_angle == 135)
  {
    _px -= 1;
    _py += 1;
  }
  else if (_angle == 180)
  {
    _px -= 1;
  }
  else if (_angle == 225)
  {
    _px -= 1;
    _py -= 1;
  }
  else if (_angle == 315)
  {
    _px += 1;
    _py -= 1;
  }
}
