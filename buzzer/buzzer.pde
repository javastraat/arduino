#define BUZZER 3

void setup()                    
 {
  pinMode(BUZZER, OUTPUT);
 }
void loop()                    
 {
    for(int x = 0; x < 100; x++)
    {
      digitalWrite(BUZZER, HIGH);
      delay(1);
      digitalWrite(BUZZER, LOW);
      delay(1);
    }
  }
