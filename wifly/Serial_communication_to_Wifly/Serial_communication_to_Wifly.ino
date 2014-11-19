void setup(){
  Serial1.begin(9600);
  Serial.begin(9600);
  
  while(!Serial){;}
}

void loop(){
  if(Serial1.available() > 0){ //if rx pin has received a message
    Serial.write(Serial1.read());
  }
  if(Serial.available() > 0){ //if usb has received a message
    Serial1.write(Serial.read());
  }
}

