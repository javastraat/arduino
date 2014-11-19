#include <BEURS_LEDS.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFlyHQ.h>

WiFly wifly;
BEURS_LEDS LED;

#define TCSAdress 0x39
#define TMPAdress 0x40

//#define RTCAdress B1010001 //compiler set read/write
#define RTCAdress B1010001
#define EEPROMAdress B1010000
#define RTCControl_1 0x00
#define RTCControl_2 0x01
#define RTCClockoutControl 0x0D
#define RTCTimerControl 0x0E
#define RTCTimer 0x0F
#define RTCMonths 0x07

unsigned int Checkwaarde = 0;
unsigned int Checkkleur = 0;

int Button  = 30;

float Tambient;
unsigned int Seconds;
unsigned int Minutes;
unsigned int Hours;
unsigned int Greenhigh = 0;
unsigned int Greenlow = 0;
byte CheckTMP = 0;
byte CheckTCS = 0;
byte CheckEEPROM = 0;
byte CheckRTC = 0;
byte CountGood =0;
char Buf[15];
byte K = 0;

boolean Firsttime = true;
boolean UpdateIP = true;
boolean UpdateRgb = false;
boolean UpdateTemp = false;
boolean UpdateDelay = false;
boolean UpdateDelay2 = false;
boolean UpdateHeliosTime=false;

boolean SCount = false;

unsigned int SecCount = 0;

#define RESET_VAL 3035 //500ms
#define RESET_T3 TCNT3 = RESET_VAL //Timer3 reset value

ISR(TIMER3_OVF_vect){
  RESET_T3; //reset instellingen voor overflow elke ms
  if(SCount){
    SecCount++;
    if(SecCount == 12){ //6seconden
      LED.SetRGB(1,0,0);
    }
    if(SecCount == 13){
        LED.SetRGB(0,0,0);
        SecCount = 11;
    }
  }
};



void setup()
{
  //TIMER3 INSTELLINGEN
  TCCR3B = B00000011;
  TCCR3A = B00000000;
  TIMSK3 |= (1<<TOIE3) | (0<<OCIE3A); //disabel T3OVF - TOIE3 = 0-
  LED.Init();
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  LED.ClearAll();
  pinMode(Button,INPUT);
  //RTC set alles op 0
  Wire.beginTransmission(RTCAdress);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
}
  
void loop()
{   
  if(digitalRead(Button)){
    while(digitalRead(Button)){  
      CheckLedMatrix();
    }
    LED.ClearAll();
    CheckRgb();
    CheckLeds();
    CheckWifly();
    CheckTcs();
    CheckTemp();
    CheckEeprom();
    CheckRtc();
    CheckHardware();
  }
  if(Firsttime){
    unsigned int millis1,millis2;
    millis1 = millis();
    millis2 = millis();
    while((millis2 - millis1)<1500){  
      millis2 = millis();
      Firsttime = false;
      CheckLedMatrix();
    }
    LED.ClearAll();
    CheckRgb();
    CheckLeds();
    CheckWifly();
    CheckTcs();
    CheckTemp();
    CheckEeprom();
    CheckRtc();
    CheckHardware();
  } 
}
 

void CheckLedMatrix()
{
  //delay(500);
  LED.LedkrantWaarde(0xFF);
  //delay(500);
  //LED.ClearAll();
}

void CheckRgb()
{
  LED.SetRGB(0,0,1);
  delay(500);
  LED.SetRGB(0,1,0);
  delay(500);
  LED.SetRGB(0,1,1);
  delay(500);
  LED.SetRGB(1,0,0);
  delay(500);
  LED.SetRGB(1,0,1);
  delay(500);
  LED.SetRGB(1,1,0);
  delay(500);
  LED.SetRGB(1,1,1);
  delay(500);
  LED.ClearAll();
}

void CheckLeds()
{
  LED.SetLEDS(1,1,1);
  delay(500);
  LED.ClearAll();
}

void CheckWifly()
{
  Checkwaarde = B10000000;
  LED.SetRGB(0,1,1);
  LED.LedkrantCheckLed(Checkwaarde);
  SCount = true;
  if(wifly.begin(&Serial1,&Serial))
  {
    Serial.println("Wifly available and started"); 
    LED.SetRGB(0,1,0);  
    CountGood = CountGood + 1; 
    SCount = false;
  }
  else
  {
    Serial.println("Can't start Wifly");
    LED.SetRGB(1,0,0);
    CountGood = CountGood + 0;
    SCount = false;
  }
  delay(500);
}

void CheckTcs()
{
  int result;
  SCount = true;
  Wire.beginTransmission(0x39);
  Wire.write(0x80);  //cmd mode, control reg
  Wire.write(0x01); // power on
  result = Wire.endTransmission();
  if(result != 0){
    Serial.println("No Tcs found");
    Checkwaarde = B01000000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll();
    CountGood = CountGood + 0;
    return;
  }
  Wire.beginTransmission(0x39);
  Wire.write(0x81); //cmd mode, time reg
  Wire.write(0x01); //intergration time 
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.write(0x87); //cmd mode, gain reg
  Wire.write(0x00); //gain/pres 
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.write(0x80); //cmd mode, control reg
  Wire.write(0x03); // power on, adc enable
  Wire.endTransmission();
  
  //delay(10);
  
  unsigned int greenhigh;
  unsigned int greenlow;
  Wire.beginTransmission(0x39);
  Wire.write(0xB0); 
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.requestFrom(0x39,2);
  greenlow = Wire.read();
  greenhigh = Wire.read();
  result = Wire.endTransmission();
  if(result != 0){
    Serial.println("Error while TCS reading");
    Checkwaarde = B01000000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll();
    CountGood = CountGood + 0;
    return;
  }  
  greenhigh = (greenhigh*256)+greenlow;
  if(greenhigh >=0 & greenhigh <=60000){
    Serial.println("ColourSensor Ok");
    Checkwaarde = B01000000;
    LED.SetRGB(0,1,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll(); 
    CountGood = CountGood + 1;

  }
  else{
    Serial.println("Returned false TCS value");
    Checkwaarde = B01000000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll(); 
    CountGood = CountGood + 0;
  }
  SCount = false;
  SecCount = 0;
}

void CheckTemp()
{
  int result;
  SCount = true;
  unsigned int ambienth,ambientl;
  Wire.beginTransmission(TMPAdress);
  Wire.write(0x02); //write to congig register
  Wire.write(0x71); //config reg, MOD
  Wire.write(0x00); //config reg DRDY disabled
  result = Wire.endTransmission();
  if(result != 0){
    Serial.println("No TMP found");
    Checkwaarde = B00100000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll(); 
    CountGood = CountGood + 0;
    return;
  }
  
  Wire.beginTransmission(TMPAdress);
  Wire.write(0x01);
  Wire.endTransmission();
  
  delayMicroseconds(2);
  
  Wire.requestFrom(TMPAdress,2);
  while(Wire.available()){
    ambienth = Wire.read();
    ambientl = Wire.read();
  }
  result = Wire.endTransmission();
  if(result != 0){
    Serial.println("Error while reading TMP");
    Checkwaarde = B00100000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll(); 
    CountGood = CountGood + 0;
    return;
  }
  ambienth = (ambienth*256)+ambientl;
  float tambient=(ambienth)*(0.0078512);
  if(tambient>=10 & tambient <=50){    
      Serial.println("TempSensor Ok");
      Checkwaarde = B00100000;
      LED.SetRGB(0,1,0);
      LED.LedkrantCheckLed(Checkwaarde); 
      delay(500);
      LED.ClearAll();
      CountGood = CountGood + 1;
  }
  else{
    Serial.print("Returnded false TMP value");
    Checkwaarde = B00100000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll();
    CountGood = CountGood + 0;
  }
  SCount = false;
  SecCount = 0;
}

void CheckEeprom()
{
  int result;
  SCount = true;
  int readaddress;
  unsigned int value = 0x18;
  
  Wire.beginTransmission(EEPROMAdress);
  Wire.write(0x00);
  Wire.write(value);
  result = Wire.endTransmission();
  if(result!=0) 
  {
    Serial.println("No EEPROM found"); 
    Checkwaarde = B00010000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll(); 
    CountGood = CountGood + 0;
    return;
  }  
  
  delay(5);
  Wire.beginTransmission(EEPROMAdress);
  Wire.write(0x00);
  Wire.endTransmission();
  
  unsigned int x=0xab;
  Wire.requestFrom(EEPROMAdress,1);
  while(Wire.available()){
  x = Wire.read();
  }
  result=Wire.endTransmission();
  if(result!=0) 
  {
    Serial.print("Error while EEPROM reading "); 
    Serial.println(result);
     Checkwaarde = B00010000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll(); 
    CountGood = CountGood + 0;
    return;
  }
  if(x == value){
    Serial.println("EEPROM Ok");
    Checkwaarde = B00010000;
    LED.SetRGB(0,1,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll(); 
    CountGood = CountGood + 1;
  }
  else{
    Serial.print("Returned false EEPROM value");
    Serial.print(value,HEX);
    Serial.print("\t\tVS ");
    Serial.println(x,HEX);
    Checkwaarde = B00010000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll(); 
    CountGood = CountGood + 0;
  }
  SCount = false;
  SecCount = 0;
}

void CheckRtc()
{
  int result;
  SCount = true;
  unsigned int xh;
  unsigned int xl;
  Wire.beginTransmission(RTCAdress);
  Wire.write(0x02);
  result=Wire.endTransmission();
  
  if(result!=0) 
  {
    Serial.println("No RTC found"); 
    Checkwaarde = B00001000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll();  
    CountGood = CountGood + 0;
    return;
  }
  Wire.beginTransmission(RTCAdress);
  Wire.requestFrom(RTCAdress,1); 
  unsigned int x  = Wire.read(); 
  result = Wire.endTransmission();
  xh = (x>>4) & 0x7;
  xl = x & 0xF; 
  x = xh*10 + xl;
  if(result != 0){
    Serial.println("Error while reading RTC");
    Checkwaarde = B00001000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll();  
    CountGood = CountGood + 0; 
    return;
  }
  if(xh>=0 & xh<=61){
    Serial.println("RTC Ok");
    Checkwaarde = B00001000;
    LED.SetRGB(0,1,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll();  
    CountGood = CountGood + 1;
  }
  else{
    Serial.println("Returned false RTC value");
    Serial.print("Count: ");
    Serial.println(xh,DEC);
    Checkwaarde = B00001000;
    LED.SetRGB(1,0,0);
    LED.LedkrantCheckLed(Checkwaarde); 
    delay(500);
    LED.ClearAll();  
    CountGood = CountGood + 0;
  } 
  SCount = false;
  SecCount = 0;
}

void CheckHardware()
{
  if(CountGood == 5){
    LED.SetRGB(0,1,0);
    LED.LedkrantWrite("Hardware ok",60);
    while(LED.LedkrantBusy()){}
    LED.ClearAll();
  }
  else{
    LED.SetRGB(1,0,0);
    LED.LedkrantWrite("Hardware error!",60);
    while(LED.LedkrantBusy()){
      LED.SetRGB(1,0,0);
      delay(500);
      LED.SetRGB(0,0,0);
      delay(500);    
    }
    LED.ClearAll();
  }
  CountGood = 0;
}









