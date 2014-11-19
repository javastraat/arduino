#include <Helios_Temperature_Sensor_TMP006.h>
#include <BEURS_LEDS.h>
#include <Helios_Colour_Sensor_TCS3414.h>
#include <Helios_EEPROM_24AA014.h>
#include <Wire.h>
#include <SPI.h>

Helios_Temperature_Sensor_TMP006 ts;
Helios_EEPROM_24AA014 ee;
Helios_Colour_Sensor_TCS3414 ct;
BEURS_LEDS bl;

char Data1[30];

int ReadCounter = 0;
int NoSignalCounter = 0;
char RxBuffer[50];
int RxIn=0;

static boolean ButtonPressed = false;

unsigned long CheckTimer = 0;
unsigned long HeartBeatTimer = 0;
boolean Connection =false;
boolean SetId = false;

void setup()
{ 
  bl.Init();
  bl.ClearAll();  
  Wire.begin();
  Serial.begin(9600); // Serial monitor
  bl.SetRGB(0,1,0);
  if(strlen(ee.StringRead(0)) == 0){
    bl.LedkrantWrite("No Id",70);
    while(bl.LedkrantBusy()){}
    LuxMeting();
  }
  else{
    sprintf(Data1,"Hallo %s",ee.StringRead(0));
    bl.LedkrantWrite(Data1,70);
    while(bl.LedkrantBusy()){}
    LuxMeting(); 
  } 
}

void loop()
{
  UpKeepButton();  
}


void LuxMeting()
{
  bl.LedkrantClear();
  unsigned int lux,cct;
  ct.Read(&lux,&cct);
  if(cct < 3500) bl.SetLEDS(1,0,0);
  if(cct>3500 && cct<5000) bl.SetLEDS(0,1,0);
  if(cct > 5000) bl.SetLEDS(0,0,1);
  int object = ts.ReadObject();
  sprintf(Data1,"%d lux %d K %d C",lux,cct,object);
  bl.LedkrantWrite(Data1,70);
  while(bl.LedkrantBusy()){}
  bl.SetLEDS(0,0,0);
}
    

void UpKeepButton()
{
  if(bl.Button()){
    LuxMeting();
    bl.ResetButton();
  }
}
