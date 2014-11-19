/*
  XBeeJoystick.h - Library for XBeeJoystick arduino code.
  Coded by Oidas Andrade.
*/
#include "Infravermelho.h"

const int _recebePino = 6;
IRrecv ir_recebe(_recebePino);
decode_results codigo_recebido;

void Infravermelho::loopApp()
{  
  if (_running && ir_recebe.decode(&codigo_recebido))
  {    
    switch(codigo_recebido.value)
    {
      case bt1:
        _ledMatrix->printStringScroll(0, 0, "Botao 1", 30, '<');
        break;
      case bt2:
        _ledMatrix->printStringScroll(0, 0, "Botao 2", 30, '<');
        break;
      case bt3:
        _ledMatrix->printStringScroll(0, 0, "Botao 3", 30, '<');
        break;
      case bt4:
        _ledMatrix->printStringScroll(0, 0, "Botao 4", 30, '<');
        break;
      case bt5:
        _ledMatrix->printStringScroll(0, 0, "Botao 5", 30, '<');
        break;
      case bt6:
        _ledMatrix->printStringScroll(0, 0, "Botao 6", 30, '<');
        break;
      case bt7:
        _ledMatrix->printStringScroll(0, 0, "Botao 7", 30, '<');
        break;
      case bt8:
        _ledMatrix->printStringScroll(0, 0, "Botao 8", 30, '<');
        break;
      case bt9:
        _ledMatrix->printStringScroll(0, 0, "Botao 9", 30, '<');
        break;
    }

    Serial.println(codigo_recebido.value);
    delay(1000);
    ir_recebe.resume();    
  }
}

void Infravermelho::setupApp(String appName, LedControl *ledMatrix)
{
  BaseApp::setupApp(appName, ledMatrix);
  ir_recebe.enableIRIn();
  ir_recebe.blink13(true);
}
