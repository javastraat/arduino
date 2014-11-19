
#include <Wire.h>
#include <SPI.h>
#include <WiFlyHQ.h>

WiFly wifly;
// I2C values
#define TCSAdress 0x39
#define TMPAdress 0x40

// Matrix control
int LedEnable  = A2;
int LedLatch  = A3;

//#define RTCAdress B1010001 //compiler set read/write
#define RTCAdress B1010001
#define EEPROMAdress B1010000
#define RTCControl_1 0x00
#define RTCControl_2 0x01
#define RTCClockoutControl 0x0D
#define RTCTimerControl 0x0E
#define RTCTimer 0x0F
#define RTCMonths 0x07

// char TempCharS[3] = {'---'}; Not used yet
char TempChar[3] = {'---'};
char TimeCharM[3] = {'---'};
char TimeCharH[3] = {'---'};

unsigned int Seconds;
unsigned int Minutes;
unsigned int Hours;



// Wifi network
const char ssid[] = "TechInc";
const char pass[] = "itoldyoualready";



int counter = 0;
char Buffer[16];




float Tambient;




//unsigned int Value;

unsigned int Redhigh = 0;
unsigned int Redlow = 0;
unsigned int Greenhigh = 0;
unsigned int Greenlow = 0;
unsigned int Bluehigh = 0;
unsigned int Bluelow = 0;
unsigned int Clearhigh = 0;
unsigned int Clearlow = 0;



boolean Firsttime = true;
boolean StartPos = true;
unsigned int MsCount = 0;

unsigned int Checkwaarde = 0;
unsigned int Checkkleur = 0;


// will be 1 when checkresult is OK
byte CheckTMP = 0;
byte CheckTCS = 0;
byte CheckEEPROM = 0;
byte CheckRTC = 0;



//char Buf[15];
char Temp[150];
unsigned int Buflength = 15;
unsigned int TempPosition = 0;
unsigned int Scrollspeed = 65;
unsigned int Scrollspeed2 = 1000;
byte I = 0;
byte J = 0;
byte K = 0;




boolean UpdateIP = false;
boolean UpdateRgb = true;
boolean UpdateTemp = false;
boolean UpdateDelayAfterTemp = false;
boolean UpdateDelayAfterRTC = false;
boolean UpdateHeliosTime=false;

boolean ShowArrowAtTemp = false;
boolean ShowArrowAtRTC = false;


#define RESET_VALUE 65410 //1ms

//#define RESET_TIMER1 TCNT1 = RESET_VALUE100 //Timer1 reset value
#define RESET_TIMER3 TCNT3 = RESET_VALUE //Timer3 reset value

ISR(TIMER3_OVF_vect){
  RESET_TIMER3; //reset instellingen voor overflow elke ms
  MsCount++;
  ISR_ShowIP();
  ISR_ScrollRgb();
  ISR_Delay();
/* THIS IS NOW DONE IN ISR_Delay..   
  if(MsCount % 5500 == 0){
      if(UpdateDelayAfterTemp){
        UpdateDelayAfterTemp = false;
        UpdateHeliosTime = true;
        ShowArrowAtTemp = false;
        ShowArrowAtRTC = true;
      }
      if(UpdateDelayAfterRTC){
        UpdateDelayAfterRTC = false;
        UpdateIP = true;
        ShowArrowAtRTC = false;
      }
  }
*/ 
  if(MsCount % Scrollspeed == 0){
     // Scroll ledmatrix one row
    TempPosition++;
  } 
  if(MsCount % Scrollspeed2 == 0){
     // Scroll RGB by 8
   K=K+8;
  } 
};

static int ATeken[60]=
{
          B00000000,//46
          B01100000,
          B01100000,
          B00000000,
          B00000000,
   
          B00100000,//47
          B00010000,
          B00001000,
          B00000100,
          B00000010,
   
          B00111110,//48
          B01010001,
          B01001001,
          B01000101,
          B00111110,
   
          B00000000,//49
          B01000010,
          B01111111,
          B01000000,
          B00000000,
   
          B01000010,//50
          B01100001,
          B01010001,
          B01001001,
          B01000110,
   
          B00100001,//51
          B01000001,
          B01000101,
          B01001011,
          B00110001,
   
          B00011000,//52
          B00010100,
          B00010010,
          B01111111,
          B00010000,
   
          B00100111,//53
          B01000101,
          B01000101,
          B01000101,
          B00111001,
   
          B00111100,//54
          B01001010,
          B01001001,
          B01001001,
          B00110000,
   
          B00000001,//55
          B01110001,
          B00001001,
          B00000101,
          B00000011,
   
          B00110110,//56
          B01001001,
          B01001001,
          B01001001,
          B00110110,
   
          B00000110,//57
          B01001001,
          B01001001,
          B00101001,
          B00011110
};

void setup()
{
  pinMode(LedEnable,OUTPUT);
  pinMode(LedLatch,OUTPUT);
  digitalWrite(LedEnable,HIGH);
  digitalWrite(LedLatch,LOW);
  ConfigTimer();
  Serial.begin(9600);
  SPI.begin();
  Wire.begin();
  Serial1.begin(9600); // Wifly
  delay(6000);

  Serial.print("Free memory: ");
  Serial.println(wifly.getFreeMemory(),DEC);
   
  if (!wifly.begin(&Serial1, &Serial))
  {
    Serial.println("Failed to start wifly");
   //T   wifly.terminal();
  }
 
  if (!wifly.isAssociated())
  {
    wifly.setSSID(ssid);
    wifly.setPassphrase(pass);
    wifly.enableDHCP();
    wifly.save();
   if (wifly.join()) {
     Serial.println("Joined wifi network");
   } else {
     Serial.println("Failed to join wifi network");
     //T wifly.terminal();
   }
  } else {
      Serial.println("Already joined network");
  }
  wifly.setBroadcastInterval(0);   // Turn off UPD broadcast

  Serial.println(wifly.getIP(Buffer,sizeof(Buffer)));

  //T wifly.terminal();
  //T    Serial.print(F("MAC: "));
  //T    Serial.println(wifly.getMAC(buf, sizeof(buf)));
  //T    Serial.print(F("IP: "));
  //T    Serial.println(wifly.getIP(buf, sizeof(buf)));

  wifly.setDeviceID("Helios-WebServer");

  if (wifly.isConnected()) {
    Serial.println("Old connection active. Closing");
   wifly.close();
  }

  wifly.setProtocol(WIFLY_PROTOCOL_TCP);
  if (wifly.getPort() != 80) {
    wifly.setPort(80);
   /* local port does not take effect until the WiFly has rebooted (2.32) */
   wifly.save();
   Serial.println("Set port to 80, rebooting to make it work");
   //  Moved outside the if
   //   wifly.reboot();
   //   delay(3000);
  }
  Serial.println("Reboot wifly");
  wifly.reboot();
  delay(3000);
  Serial.println("Ready");
   
  digitalWrite(LedEnable,LOW);
}

void ConfigTimer()
{   
   //TIMER3 INSTELLINGEN
   TCCR3B = B00000011;
   TCCR3A = B00000000;
   TIMSK3 |= (1<<TOIE3) | (0<<OCIE3A);
   
   SREG =   B10000000;  //enable global interrupt
}

void loop()
{
   
  if(Firsttime){
    CheckLedMatrix();
    CheckRgb();
    CheckLeds();
    CheckTcs();
    CheckTemp();
    CheckEeprom();
    CheckRtc();
    ClearSPI();
  }
  ReadAmbientTemp();
  HeliosTime();
  char buf[80];
  char *bbuf;
  bbuf = &buf[0];
  if (wifly.available() > 0) {
     if (wifly.gets(buf, sizeof(buf))) {
     // Skip the *OPEN* message that occasionally occurs when a new connection is made
     if (strncmp_P(bbuf, PSTR("*OPEN*"), 6) == 0) {
            Serial.println("skipping *OPEN*");
            bbuf = &buf[6]; 
     }
       if (strncmp_P(bbuf, PSTR("GET / "), 6) == 0) {
        while (wifly.gets(bbuf, sizeof(buf-6)) > 0) { /* Skip rest of request */   }
        sendIndex();                  
        Serial.println("sending index");
      } else if (strncmp_P(bbuf, PSTR("GET /A "), 7) == 0) {
        while (wifly.gets(bbuf, sizeof(buf-6)) > 0) { /* Skip rest of request */   }
        Serial.println("sending AJAX");
        sendStatus(TempChar,TimeCharM, TimeCharH);
      } else {
        /* Unexpected request */
      Serial.print("Unexpected: ");
      Serial.println(bbuf);
      wifly.flushRx();      // discard rest of input
      Serial.println("Sending 404");
      send404();
      }      
    }
  }
}


 

void CheckLedMatrix()
{
  char buf[100];
  unsigned int x = 0;
  buf[18] = 0;
  buf[17] = 0;
  buf[16] = 0;
  buf[15] = 0;
  buf[14] = 0;
  buf[13] = 0;
  buf[12] =B00011000;
  buf[11] =B00100100;
  buf[10] =B01000010;
  buf[9] = B10000001;
  buf[8] = B10000001;
  buf[7] = B01000010;
  buf[6] = B00100100;
  buf[5] = B00011000;
  buf[4] = 0;
  buf[3] = 0;
  buf[2] = 0;
  buf[1] = 0;
  buf[0] = 0;
  for(unsigned int j=0; j<13; j++){
    while(x<40){
      x++;
        for(unsigned int i = 0; i<5;i++){
        SPI.transfer(1<<(i));
        SPI.transfer(buf[i+j]);
        digitalWrite(LedLatch,HIGH);
        digitalWrite(LedLatch,LOW);
        delay(1);
        }
    }
    x=0;
  }
}

void CheckRgb()
{
  unsigned int s = 0;
  for(int i = 8; i<64; i=i+8){
    while(s<500){
      s++;
      SPI.transfer(1<<(5));
      SPI.transfer(i);
      digitalWrite(LedLatch,HIGH);
      digitalWrite(LedLatch,LOW);
      delay(1);
    }
    s = 0;
  }
}

void CheckLeds()
{
  unsigned int s = 0;
  while(s<500){
    s++;
    SPI.transfer(1<<(5));
    SPI.transfer(7);
    digitalWrite(LedLatch,HIGH);
    digitalWrite(LedLatch,LOW);
    delay(1);
  }
    SPI.transfer(1<<(5));
    SPI.transfer(0);
    digitalWrite(LedLatch,HIGH);
    digitalWrite(LedLatch,LOW);
    delay(1);
}

void CheckTcs()
{
  int result;
  Wire.beginTransmission(0x39);
  Wire.write(0x80);  //cmd mode, control reg
  Wire.write(0x01); // power on
  result = Wire.endTransmission();
  if(result != 0){
    Serial.println("No Tcs found");
    Checkwaarde = B10000000;
    Checkkleur = 32;
    CheckHardware();
    CheckTCS = 0;
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
 
  //unsigned int greenhigh;
  //unsigned int greenlow;
  Wire.beginTransmission(0x39);
  Wire.write(0xB0);
  Wire.endTransmission();
 
  Wire.beginTransmission(0x39);
  Wire.requestFrom(0x39,8);
  Greenlow = Wire.read();
  Greenhigh = Wire.read();
  Redlow = Wire.read();
  Redhigh = Wire.read();
  Bluelow = Wire.read();
  Bluehigh = Wire.read();
  Clearlow = Wire.read();
  Clearhigh = Wire.read();
  result = Wire.endTransmission();
  if(result != 0){
    Serial.println("Error while TCS reading");
    Checkwaarde = B10000000;
    Checkkleur = 32;
    CheckTCS = 0;
    CheckHardware();
    return;
  } 
  bool AllOk = true;
  Greenhigh = (Greenhigh*256)+Greenlow;
  if(Greenhigh >=0 & Greenhigh <=60000){
    Serial.print("Green value = ");
    Serial.println(Greenhigh);
  } else {
   AllOk = false;
  }
  Redhigh = (Redhigh*256)+Redlow;
  if(Redhigh >=0 & Redhigh <=60000){
    Serial.print("Red value = ");
    Serial.println(Redhigh);
  } else {
   AllOk = false;
  }
  Bluehigh = (Bluehigh*256)+Bluelow;
  if(Bluehigh >=0 & Bluehigh <=60000){
    Serial.print("Blue value = ");
    Serial.println(Bluehigh);
  } else {
   AllOk = false;
  }
  Clearhigh = (Clearhigh*256)+Clearlow;
  if(Clearhigh >=0 & Clearhigh <=60000){
    Serial.print("Clear value = ");
    Serial.println(Clearhigh);
  } else {
   AllOk = false;
  }
  if(AllOk){
    Checkwaarde = B10000000;
    Checkkleur = 16;
    CheckTCS = 1;
    CheckHardware();
  } else {
    Serial.println("Returned false TCS value");
    Checkwaarde = B10000000;
    Checkkleur = 32;
    CheckTCS = 0;
    CheckHardware();
  }
}

void CheckTemp()
{
  int result;
  unsigned int ambienth,ambientl;
  Wire.beginTransmission(TMPAdress);
  Wire.write(0x02); //write to congig register
  Wire.write(0x71); //config reg, MOD
  Wire.write(0x00); //config reg DRDY disabled
  result = Wire.endTransmission();
  if(result != 0){
    Serial.println("No TMP found");
    Checkwaarde = B01000000;
    Checkkleur = 32;
    CheckTMP = 0;
    CheckHardware();
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
    Checkwaarde = B01000000;
    Checkkleur = 32;
    CheckTMP = 0;
    CheckHardware();
    return;
  }
  ambienth = (ambienth*256)+ambientl;
  float tambient=(ambienth)*(0.0078512);
  if(tambient>=10 & tambient <=50){   
      Serial.print("TMP waarde = ");
      Serial.println(tambient);
      Checkwaarde = B01000000;
      CheckTMP = 1;
      Checkkleur = 16;
      CheckHardware();
  }
  else{
    Serial.print("Returnded false TMP value");
    Checkwaarde = B01000000;
    Checkkleur = 32;
    CheckTMP = 0;
    CheckHardware();
  }

}

void CheckEeprom()
{
  int result;
  int readaddress;
  unsigned int value = 0x18;
 
  Wire.beginTransmission(EEPROMAdress);
  Wire.write(0x00);
  Wire.write(value);
  result = Wire.endTransmission();
  if(result!=0)
  {
    Serial.println("No EEPROM found");
    Checkwaarde = B00100000;
    Checkkleur = 32;
    CheckEEPROM = 0;
    CheckHardware();
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
    Checkwaarde = B00100000;
    CheckEEPROM = 0;
    Checkkleur = 32;
    CheckHardware();
    return;
  }
  if(x == value){
    Serial.print("Returned matching EEPROM value");
    Serial.print("\t EEPROM value  = ");
    Serial.println(x,HEX);
    Checkwaarde = B00100000;
    Checkkleur = 16;
    CheckEEPROM = 1;
    CheckHardware();
  }
  else{
    Serial.print("Returned false EEPROM value");
    Serial.print(value,HEX);
    Serial.print("\t\tVS ");
    Serial.println(x,HEX);
    Checkwaarde = B00100000;
    Checkkleur = 32;
    CheckEEPROM = 0;
    CheckHardware();
  }
}

void CheckRtc()
{
  int result;
  unsigned int xh;
  unsigned int xl;
  Wire.beginTransmission(RTCAdress);
  Wire.write(0x02);
  result=Wire.endTransmission();
 
  if(result!=0)
  {
    Serial.println("No RTC found");
    Checkwaarde = B00010000;
    Checkkleur = 32;
    CheckRTC = 0;
    CheckHardware();
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
    Checkwaarde = B00010000;
    Checkkleur = 32;
    CheckRTC = 0;
    CheckHardware();
    return;
  }
  if(xh>=0 & xh<=61){
    Serial.print("Count: ");
    Serial.println(xh,DEC);
    Checkwaarde = B00010000;
    Checkkleur = 16;
    CheckRTC = 1;
    CheckHardware();
  }
  else{
    Serial.println("Returned false RTC value");
    Serial.print("Count: ");
    Serial.println(xh,DEC);
    Checkwaarde = B00010000;
    Checkkleur = 32;
    CheckRTC = 0;
    CheckHardware();
  }
 Firsttime = false;
}

void CheckHardware()
{
  unsigned int x = 0;
  while(x <500){
    x++;
    for(int i = 0; i<2;i++){
      if(i==0){
        SPI.transfer(1<<(4));
        SPI.transfer(Checkwaarde);
        digitalWrite(LedLatch,HIGH);
        digitalWrite(LedLatch,LOW);
        delay(1);
      }
      if(i==1){
        SPI.transfer(1<<(5));
        SPI.transfer(Checkkleur);
        digitalWrite(LedLatch,HIGH);
        digitalWrite(LedLatch,LOW);
        delay(1);
      }
    }
  }
}

void ClearSPI()
{
      for(int i = 0; i<2;i++){
      if(i==0){
        SPI.transfer(1<<(4));
        SPI.transfer(0);
        digitalWrite(LedLatch,HIGH);
        digitalWrite(LedLatch,LOW);
        delay(1);
      }
      if(i==1){
        SPI.transfer(1<<(5));
        SPI.transfer(0);
        digitalWrite(LedLatch,HIGH);
        digitalWrite(LedLatch,LOW);
        delay(1);
      }
    }
}

void GetTekens()
{
  unsigned int x;
  for(int i = 0; i<20; i++){
      x = Buffer[i];
      if(x == 0){
        Buflength = i;
        break;
      }
  }
  for(int i = 0;i<Buflength;i++){
    for(int j=0;j<6;j++){
      if(j<5){
       Temp[(i*6+5)+j] = ATeken[((Buffer[i]-46)*5)+j]; //begin positie+5 om eerste character beeld in te laten scrollen
      }
      if(j==5){ //ruimte tussen characters toevoegen
        Temp[(i*6+5)+j] = 0;
      }
    }
  }
}


// called during ISR to switch on the next row of LEDS in the Matrix
void ISR_ShowIP() //functie om naar ledmatrix te schrijven
{
//  if(!Firsttime & UpdateIP){
  if(UpdateIP){
    if(StartPos){
        StartPos = false;
        //sprintf(Buffer,"255.255.255.255");
        ClearTemp();
        GetTekens();
        I=0;
        TempPosition = 0;
    }
    if(TempPosition>Buflength*6+9){
         // Reset startposition, and then update RGB
        StartPos = true; UpdateIP = false; UpdateRgb = true; return;
    } //reset alle variablen

     
    J = TempPosition; // j is de posite van de tekst waarde in temp[]
    SPI.transfer(1<<(I));
    SPI.transfer((Temp[J+I]));
    digitalWrite(LedLatch,HIGH);
    digitalWrite(LedLatch,LOW); 
    I++; //bepaalt naar welke van de 5 lijnen wordt geschreven
    if(I>4){I=0;} //begin weer te schrijven bij lijn 0
     
  }
}

void ClearTemp()
{
  //for(int i =0; i<(Buflength+5); i++){
  //  for(int j = 0; j<6; j++){
  //    Temp[(i*6+5)+j] = 0;
  //  }
  //}
  for(int i =0; i<150; i++){
      Temp[i] = 0;
  }
}


// called during ISR to switch on color in the RGB led
void ISR_ScrollRgb()
{
  if(UpdateRgb){
    if(StartPos){
      StartPos = false;
      // K is updated (k=k+8) every Scrollspeed2 miliseconds
      K=8;
    }
    if(K > 64){
       // Stop updating the RGB, start updating temperature
       UpdateRgb = false;UpdateIP=false;StartPos=true; UpdateTemp = true;ShowArrowAtTemp = true;
       return;
    }
    SPI.transfer(1<<(5));
    SPI.transfer(K);
    digitalWrite(LedLatch,HIGH);
    digitalWrite(LedLatch,LOW);
  }
}

// called during ISR to delay
void ISR_Delay()
{
  if(UpdateDelayAfterTemp){
    if(StartPos){
      StartPos = false;
      // K is updated (k=k+8) every Scrollspeed2 miliseconds so init here
      K=8;
    }
    if(K > 64){
      UpdateDelayAfterTemp = false;
      UpdateHeliosTime = true;
      ShowArrowAtTemp = false;
      ShowArrowAtRTC = true;
    }
  }
  if(UpdateDelayAfterRTC){
    if(StartPos){
      StartPos = false;
      // K is updated (k=k+8) every Scrollspeed2 miliseconds so init here
      K=8;
    }
    if(K > 64){
     UpdateDelayAfterRTC = false;
      UpdateIP = true;
     StartPos = true;
      ShowArrowAtRTC = false;
   }
  }   
}

void ReadAmbientTemp()
{
  if(UpdateTemp){
     // only update the temp when needed
    UpdateTemp = false;
    unsigned int ambienth,ambientl;
    Wire.beginTransmission(TMPAdress);
    Wire.write(0x02); //write to congig register
    Wire.write(0x71); //config reg, MOD
    Wire.write(0x00); //config reg DRDY disabled
    Wire.endTransmission();

 
    Wire.beginTransmission(TMPAdress);
    Wire.write(0x01);
    Wire.endTransmission();
 
    delayMicroseconds(2);
 
    Wire.requestFrom(TMPAdress,2);
    while(Wire.available()){
    ambienth = Wire.read();
    ambientl = Wire.read();
    }
   Wire.endTransmission();
   ambienth = (ambienth*256)+ambientl;
   Tambient=(ambienth)*(0.0078512);
   
   TempChar[3];
   itoa(Tambient,TempChar,10);
   
   // and after updating the temp, delay
   UpdateDelayAfterTemp  = true; StartPos = true;
  }
}

void HeliosTime()
{
  if(UpdateHeliosTime){
   // Only when needed
    UpdateHeliosTime = false;
    unsigned int sh,sl,mh,ml,hh,hl;
    Wire.beginTransmission(RTCAdress);
    Wire.write(0x02);
    Wire.endTransmission();
 
    Wire.beginTransmission(RTCAdress);
    Wire.requestFrom(RTCAdress,3);
    unsigned int s = Wire.read();
    unsigned int m = Wire.read();
    unsigned int h = Wire.read();
    Wire.endTransmission();
 
    sh = (s>>4) & 0x7;
    sl = s & 0xF;
    Seconds = sh*10 + sl;
 
    mh = (m>>4) & 0x7;
    ml = m & 0xF;
    Minutes = mh*10 + ml;
   
 
    hh = (h>>4) & 0x3;
    hl = h & 0xF;
    Hours = hh*10 + hl;
    if(Minutes < 10) {
     // sprintf uses 1500 bytes, so fix it like this
     TimeCharM[0] = '0';
     TimeCharM[1] = 48 + Minutes;
     TimeCharM[2] = 0;
   } else {
      itoa(Minutes,TimeCharM,10);
   } 
    itoa(Hours,TimeCharH,10);
    //sprintf(TimeCharM,"%02d",Seconds);
   
    // And after updating the time, delay
    UpdateDelayAfterRTC = true; StartPos = true;
   
  } 
}

void sendIndex()
{
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();
    //wifly.sendChunkln(F("<html><head>"));
    wifly.sendChunkln(F("<html><head><title>Helios status page</title>"));
    wifly.sendChunkln(F("<script src=\"http://heliosgadget.nl/demoapp/js/jquery.min.js\"></script>"));
    wifly.sendChunkln(F("<script src=\"http://heliosgadget.nl/demoapp/js/script.js\"></script>"));
    wifly.sendChunkln(F("<link rel=\"stylesheet\" type=\"text/css\" href=\"http://heliosgadget.nl/demoapp/demo.css\" >")); 
 
  //  wifly.sendChunkln(F("<script src=\"http://localhost/js/jquery.min.js\"></script>"));
  //  wifly.sendChunkln(F("<script src=\"http://localhost/js/script.js\"></script>"));
  //  wifly.sendChunkln(F("<link rel=\"stylesheet\" type=\"text/css\" href=\"http://localhost/demo.css\" >")); 
 
 
    //wifly.sendChunkln(F("</head>"));
    //wifly.sendChunkln(F("<body>"));
    wifly.sendChunkln(F("</head><body><h1>Status Helios</h1><div class=\"initStatus\"><h2>Init status</h2><ul>"));
    //wifly.sendChunkln(F("<div class=\"initStatus\">")); 
    //wifly.sendChunkln(F("<h2>Init status</h2><ul>")); 
         
    //wifly.sendChunkln(F("<ul>")); 
         
    if(CheckTCS == 1){   wifly.sendChunk(F("<li class='good'>Color sensor</li>"));}
    else{            wifly.sendChunk(F("<li>Color sensor</li>"));}
    //wifly.sendChunkln(F("Color sensor</li>")); 
           
    if(CheckTMP == 1){   wifly.sendChunk(F("<li class='good'>Temperatuur sensor</li>"));}
    else{            wifly.sendChunk(F("<li>Temperatuur sensor</li>"));}
    //wifly.sendChunkln(F("Temperatuur sensor</li>")); 
           
    if(CheckEEPROM == 1){   wifly.sendChunk(F("<li class='good'>EEPROM</li>"));}
    else{               wifly.sendChunk(F("<li>EEPROM</li>"));}
    //wifly.sendChunkln(F("EEPROM</li>")); 
           
    if(CheckRTC == 1){   wifly.sendChunk(F("<li class='good'>Real Time Clock</li>"));}
    else{            wifly.sendChunk(F("<li>Real Time Clock</li>"));}
    //wifly.sendChunkln(F("Real Time Clock</li>")); 
    //wifly.sendChunkln(F("</ul>")); 
    //wifly.sendChunkln(F("</div>"));

    wifly.sendChunkln(F("</ul></div><div class='dynamicStatus'></div></body></html>"));
     
    //wifly.sendChunkln(F("</body>"));
    //wifly.sendChunkln(F("</html>")); 
    wifly.sendChunkln();
}

void sendStatus(char *tp, char *timeM, char *timeH )
{   
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
   
    wifly.sendChunkln();
    wifly.sendChunkln(F("<h2>Loop status</h2><ul>"));
    //wifly.sendChunkln(F("<ul>"));
   
    if(UpdateIP){    wifly.sendChunk(F("<li class='active'>LED Matrix</li>")); }
    else{          wifly.sendChunk(F("<li>LED Matrix</li>")); }
    //wifly.sendChunkln(F("LED Matrix</li>")); 
     
    if(UpdateRgb){   wifly.sendChunk(F("<li class='active'>RGB LED</li>"));}
    else{         wifly.sendChunk(F("<li>RGB LED</li>"));}
    //wifly.sendChunkln(F("RGB LED</li>")); 
     
    if(ShowArrowAtTemp){   
                wifly.sendChunk(F("<li class='active'>Temperatuur Sensor</li>"));}
    else{         wifly.sendChunk(F("<li>Temperatuur Sensor</li>"));}
                 
    wifly.sendChunk(F("<li class='temperature'>temperatuur is: "));
    //wifly.sendChunk(F("temperatuur is: "));
    wifly.sendChunk(tp);
    wifly.sendChunkln(F("</li>"));

    if(ShowArrowAtRTC){
                wifly.sendChunk(F("<li class='active'>Real time clock</li>"));}
    else{         wifly.sendChunk(F("<li>Real time clock</li>"));}
    //wifly.sendChunkln(F("Real time clock</li>")); 
     
    wifly.sendChunk(F("<li class='temperature'>RTC Time: "));
    //wifly.sendChunk(F("RTC Time: "));
    wifly.sendChunk(timeH);
    wifly.sendChunk(F(":"));
    wifly.sendChunk(timeM);
    wifly.sendChunkln(F("</li></ul>"));
     
    //wifly.sendChunkln(F("</ul>"));
    wifly.sendChunkln();
}


/** Send a 404 error */
void send404()
{
    wifly.println(F("HTTP/1.1 404 Not Found"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();
    wifly.sendChunkln(F("<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><hr></body></html>"));
    //wifly.sendChunkln(F("<title>404 Not Found</title>"));
    //wifly.sendChunkln(F("</head><body>"));
    //wifly.sendChunkln(F("<h1>Not Found</h1>"));
    //wifly.sendChunkln(F("<hr>"));
    //wifly.sendChunkln(F("</body></html>"));
    wifly.sendChunkln();
}
