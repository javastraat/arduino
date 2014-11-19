int data=4;
int clock=8;
int latch=7;
int no=0;
int time;
int alp;

int a=B00000011; //0
int b=B10011111; //1
int c=B00100101; //2
int d=B00001101; //3
int e=B10011001; //4
int f=B01001001; //5
int g=B01000001; //6
int h=B00011111; //7
int i=B00000001; //8
int j=B00011001; //9
int k=B10011110; //1

void setup(){
  pinMode(data,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(latch,OUTPUT);
}

void loop(){
  time=millis()/1000;
  if(time++){
    no++;
  }
  if(no==11){
    no=0;
    delay(1000);
  }
  if(no==0){
    alp=a;
    delay(1000);
  }
  if(no==1){
    alp=b;
    delay(1000);
  }
  if(no==2){
    alp=c;
    delay(1000);
  }
  if(no==3){
    alp=d;
    delay(1000);
  }
  if(no==4){
    alp=e;
    delay(1000);
  }
  if(no==5){
    alp=f;
    delay(1000);
  }
  if(no==6){
    alp=g;
    delay(1000);
  }
  if(no==7){
    alp=h;
    delay(1000);
  }
  if(no==8){
    alp=i;
    delay(1000);
  }
  if(no==9){
    alp=j;
    delay(1000);
  }
  if(no==10){
    alp=k;
    delay(1000);
  }
  digitalWrite(latch,LOW);
  shiftOut(data,clock,LSBFIRST,alp);
  digitalWrite(latch,HIGH);
  delay(0);
}


