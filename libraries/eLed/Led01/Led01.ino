/* Pin Mapping for Common Cathode 5x7 LED Matrix 
  LED Matrix    Arduino(x = not connect)    Designator
      1            6                           Row4     
      2            8                           Row6
      3           12                           Column1
      4           11                           Column2
      5            x  
      6            9                           Column4
      7            7                           Row5
      8            4                           Row2
      9            2                           Row0
     10           10                           Column3
     11            x  
     12            5                           Row3
     13           13                           Column0
     14            3                           Row1

==================================
  LED Matrix    Arduino(x = not connect)    Designator
      9            2                           Row0
     14            3                           Row1
      8            4                           Row2
     12            5                           Row3
      1            6                           Row4     
      7            7                           Row5
      2            8                           Row6
     13           13                           Column0
      3           12                           Column1
      4           11                           Column2
     10           10                           Column3
      6            9                           Column4
      5            x  
     11            x  


====================================
    Arduino pin2~pin8 = row0~row6, so rowX +2 = pin number
    Arduino pin13~pin9 = column0~column4, 13 - columnY = pin number    
*/

// 5x7 LED Matrix
const byte COLS = 5; // 5 Columns
const byte ROWS = 7; // 7 Rows

void clearLEDs() {
  for (int r = 0; r < ROWS; r++) {
    digitalWrite( r + 2, LOW);
  }   
  
  for (int c = 0; c < COLS; c++) {
      digitalWrite( 13 - c, LOW);  // 共陰極，每一行預設均為接地(Low = 接地，High = 熄燈)
  }
}  

void setup() {
  // 5x7 LED Matrix 接在 pin 2 ~ pin 13
  // 把腳位設置為 output pin
  for (int i = 2; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  clearLEDs();
  
  // 由下而下，一排一排(Row)打開
  for (int r = 0; r < ROWS; r++) {
    digitalWrite( r + 2, HIGH);
    delay(300);
  }
  /*
  // 由左至右，一行一行(Column)關掉
  for (int c = 0; c < COLS; c++) {
    digitalWrite(13 - c, HIGH);
    delay(300);
  }  
*/

  // 暫停 1 秒鐘
  delay(1000);
}
