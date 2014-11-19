    int bt_key = -1;
    const int upkey = 70 ;
    const int leftkey = 76 ;
    const int downkey = 66 ;
    const int rightkey = 82 ;
    const int bt1 = 68 ;
    const int bt2 = 83 ;
    const int bt3 = 88 ;
    const int bt4 = 79 ;
    #define btLED 11 
    
  void setup()
  {
     pinMode(btLED,OUTPUT);
    Serial.begin(9600);
   Serial.println("System Up");
   Serial2.begin(9600);
   digitalWrite(btLED,LOW) ;
  }
  
  void loop()
  {
    int btdata = 0;
    btdata = BTcheck();
    if (btdata != 0)
    {
        Serial.println(btdata);
         digitalWrite(btLED,HIGH) ;
    }
     delay(200);
         digitalWrite(btLED,LOW) ;
  
  }
  
  int  BTcheck()
  {
  int tmp = 0 ;
  int readkey = 0 ;
   if (Serial2.available())
   {
     readkey = Serial2.read() ;
    tmp = BTencode(readkey) ;
    }
    else
    {
      tmp = 0  ;
  
    }
      return (tmp);
  } 
  
  int BTencode(int cc)
  {
  // Serial.print("the bt receive data is:");
  //Serial.println(cc); 
    int encodeno = 0 ;
    switch (cc)
    {
     case upkey :   // for button "up" code
       encodeno = 1 ;   
         break ;
         
     case leftkey :    // for button "left" code
       encodeno = 2 ;
         break ;
         
     case downkey :    // for button "down" code
       encodeno = 3 ;
         break ;
         
     case rightkey :    // for button "right" code
       encodeno = 4 ;
         break ;
         
     case bt1 :    // for button "1" code
       encodeno = 11 ;
         break ;
     case bt2 :    // for button "2" code
       encodeno = 12 ;
         break ;
     case bt3 :    // for button "3" code
       encodeno = 13 ;
         break ;
     case bt4 :    // for button "4" code
       encodeno = 14 ;
         break ;
    default:
       encodeno = cc ;
    
    }
    return (encodeno) ;
    
  } 

