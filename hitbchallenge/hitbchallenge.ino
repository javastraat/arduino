void setup()
{
                                  //the code this replaces is below
}
void loop()                     // run over and over again
{

  for(int i = 10; i < 13; i++){ 
  digitalWrite(i, HIGH);  //Turns on LED #1 (connected to pin 3 )
  delay(10);                //waits delayTime milliseconds
  digitalWrite(i, LOW);  //Turns on LED #3 (connected to pin 5 )
  delay(10);                //waits delayTime milliseconds
 }
}

