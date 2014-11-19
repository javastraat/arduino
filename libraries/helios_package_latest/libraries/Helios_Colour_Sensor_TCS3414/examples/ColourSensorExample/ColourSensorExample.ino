#include <Helios_EEPROM_24AA014.h>
#include <Wire.h>
#include <Helios_Colour_Sensor_TCS3414.h>

Helios_Colour_Sensor_TCS3414 csensor;

char Temp[100];

unsigned int Rood,Groen,Blauw,Wit,Lux,Cct;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  delay(2000);
}

void loop()
{
  csensor.Read(&Lux,&Cct);
  sprintf(Temp,"The illuminance = %d lux The CCT = %d K",Lux,Cct);
  Serial.println(Temp);
  Serial.println("");
  delay(1000);
  int inttime = 400;
  int gain = 1;
  int presc = 4;
  csensor.ReadLow(inttime,gain,presc,&Rood,&Groen,&Blauw,&Wit,&Lux,&Cct);
  for(int i = 0;i<6;i++){
    sprintf(Temp,"%s:",Parameter(i));
    Serial.print(Temp);
    sprintf(Temp," %d",Waarde(i));
    Serial.println(Temp);
  }
  Serial.println("");
  delay(5000);
}

char *Parameter(int i)
{
  switch(i){
    case 0: return "Red";
    case 1: return "Green";
    case 2: return "Blue";
    case 3: return "Clear";
    case 4: return "Illuminance";
    case 5: return "CCT";
  }
}

unsigned int Waarde(int i)
{
  switch(i){
    case 0: return Rood;
    case 1: return Groen;
    case 2: return Blauw;
    case 3: return Wit;
    case 4: return Lux;
    case 5: return Cct;
  }
}
  
    
  




