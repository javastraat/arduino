#include <MaxMatrix.h>

// создаем спрайт снежинки
byte snowstar[] = {
  9, 8, B00000000, B01001001, B00101010, B00010100, B01101011, B00010100, B00101010, B01001001, B00000000};
// создаем снегопада
byte snow[] = {
  16,  8, B10000010, B00000001, B10000010, B00100000, B01110000, B00100001, B10000011, B00101001, B00010000, B00101000, B10000000,0B11000001, B10000000, B00000100, B00001110, B00000100};

// описание подключения
// DIN к 8
// CS к 9
// CLK к 10
// 2 модуля
MaxMatrix m(10, 11, 12, 2);

void setup(){
  m.init();
  m.setIntensity(0); // яркость 0 из 15
}

void loop(){

  // двигаем снежинку из-за левой границы за правую через цикл
  for (int i=-8; i<16; i++)
  {
    delay(100);
   m.writeSprite(i, 0, snowstar);
  }

//  delay(100); // пауза
  // печатаем снегопад в 6 этажей
  m.writeSprite(0, 0, snow);
  m.writeSprite(0, -8, snow);
  m.writeSprite(0, -16, snow);
  m.writeSprite(0, -24, snow);
  m.writeSprite(0, -32, snow);
  m.writeSprite(0, -40, snow);

  // снегопад ... УРА !
  for (int i=0; i<32; i++)
  {
    m.shiftDown(true);
    delay(100);
  }
  // ветер сдувает снежинки, красота !
  for (int i=0; i<16; i++)
  {
    m.shiftLeft(false);
    delay(100);
  }
  delay(2000);

}
