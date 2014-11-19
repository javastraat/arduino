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
  //Helios_EEPROM_24AA014 ee;
  //ee.StringWrite(0, "Jolle");
  //ee.StringWrite(1, "Einstein");
  //ee.StringWrite(2, "Javastraat.Com");

  bl.Init();
  bl.ClearAll();  
  Wire.begin();
  Serial.begin(9600); // Serial monitor

  if(strlen(ee.StringRead(0)) == 0){
    bl.SetLEDS(1,1,1);
    bl.LedkrantWrite("No Id",70);
    while(bl.LedkrantBusy()){
    }
    LuxMeting();
  }
  else{
    bl.SetLEDS(0,0,1);
    sprintf(Data1,"Hallo %s",ee.StringRead(1));
    bl.LedkrantWrite(Data1,70);
    while(bl.LedkrantBusy()){
    }
    LuxMeting(); 
  } 
}

void loop()
{
  UpKeepButton();  
}


void LuxMeting()
{
    bl.SetLEDS(0,1,0);
bl.LedkrantClear();
  unsigned int lux,cct;
  ct.Read(&lux,&cct);
  int object = ts.ReadAmbient();
  if(object < 15) bl.SetRGB(0,0,1);
  if(object>15 && object<=20) bl.SetRGB(0,1,0);
  if(object>20 && object<=25) bl.SetRGB(1,1,0);
  if(object>25 && object<=30) bl.SetRGB(1,0,1);
 if(object > 30) bl.SetRGB(1,0,0);
  sprintf(Data1,"%d lux %d K %d C",lux,cct,object);
  bl.LedkrantWrite(Data1,70);
  while(bl.LedkrantBusy()){
  }
  bl.SetRGB(0,0,0);
  bl.SetLEDS(0,0,0);
}


void UpKeepButton()
{
  if(bl.Button()){
    LuxMeting();
    bl.ResetButton();
    }
}


