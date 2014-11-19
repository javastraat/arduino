////////////////////////////////////////////////
// Character Display
// Displays characters one at a time on the 8x8 Matrix
// This code may be freely used and copied.
//
// Gareth Davies - June 2012
//
////////////////////////////////////////////////

int comRows[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // pins for common Anode Rows
int colRed[8] = {8, 9, 10, 11, 12, 13, A0, A1}; // pins for Red column
int idxCol = 0, idxRow = 0;
int delay1 = 2; // delay for each column in ms
  int count = 0; // used for cycling through characters
  long last = 0;
  long now;

long interval = 500; // delay between each character in ms
const int numChars = 2; //number of characters in array and to display

// The following 2-dimensional array defines the characters in binary format. '1' e LED on, '0' = LED off.
// Each line represents one row of LEDs, each group of 8 lines inside {braces} is a character
// Each character is separated by a comma
byte text[numChars][8] = {{
  B00111100,
  B01000010,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B01000010,
  B00111100}, {
  B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001}};  

void setup() {                
  // initialize all pins as outputs and disable everything
  for (int i=0; i< 8; i++)
  {
    pinMode(comRows[i], OUTPUT);
    pinMode(colRed[i], OUTPUT);
    digitalWrite(comRows[i], LOW);   // switch ROW off (Anode)
    digitalWrite(colRed[i], HIGH);    // switch the column off (Cathode)
  }
  last = millis();
}

void loop()
{
  for (int i=0; i<8; i++)
  {
    for (int j=0; j<8; j++)
    {
      if(bitRead(text[count][i], j) == 1)
        digitalWrite(colRed[j], LOW);
      else
        digitalWrite(colRed[j], HIGH);
    }
    digitalWrite(comRows[i], HIGH); // enable row
    delay(delay1);
    digitalWrite(comRows[i], LOW); // disable row    
  }
  now = millis();
  if ((now - last) > interval)
  {
    last = now;
    count++;
    if (count > (numChars - 1))
      count=0;
  }
}


