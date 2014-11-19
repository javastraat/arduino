
void setup()
{
 Serial.begin(9600);
 Serial.println("System Up");
 Serial2.begin(9600);
}

void loop()
{
 if (Serial2.available())
   Serial.println(Serial2.read());

}
