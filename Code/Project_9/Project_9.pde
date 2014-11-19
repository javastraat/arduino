// Project 9 - LED Fire Effect

int ledPin1 = 10;
int ledPin2 = 9;
int ledPin3 = 11;

void setup()
{
pinMode(ledPin1, OUTPUT);
pinMode(ledPin2, OUTPUT);
pinMode(ledPin3, OUTPUT);
}

void loop() {
  int x=random(155);
  int y=random(155);
  int z=random(155);
analogWrite(ledPin1, random(x)+(255-x));
analogWrite(ledPin2, random(y)+(255-y));
analogWrite(ledPin3, random(z)+(255-z));
delay(random(150));
}


