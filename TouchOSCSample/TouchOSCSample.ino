//----------------------Arduino code-------------------------


char message = 0;     //  This will hold one byte of the serial message

int led9 = 9;

void setup() {
  Serial.begin(9600);  //set serial to 9600 baud rate

  pinMode(led9, OUTPUT);
}

void loop(){
  if (Serial.available() > 0) { //  Check if there is a new message
    message = Serial.read();    //  Put the serial input into the message

    if (message == 'Y'){  //  If a 'Y' is recieved
      digitalWrite(led9, 255);//255 tells light to go on 
    }
    if (message == 'N'){  //  If a N is received...
      analogWrite(led9, 0); //0 tells LED to go off
   
    }
  }
}



//----------------------------end Arduino code--------------------------------

