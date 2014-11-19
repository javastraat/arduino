/*
  XBeeJoystick.h - Library for XBeeJoystick arduino code.
  Coded by Oidas Andrade.
*/
#include "Pong.h"
#include "GameModel.h"
#include <binary.h>
#include <XBeeJoystick.h>
#include <TimerOne.h>

#define DEFAULT_GAME_SPEED 300000
#define DECREMENT_GAME_SPEED 0.95
#define TOPO_TELA 6
#define TELA_DIREITA 6
#define TELA_ESQUERDA 1
#define BASE_TELA 1

#define COORD_X 0
#define COORD_Y 1

#define BEEP 7
#define BEEP_TIME 100

Pong *Pong::instance;

Pong::Pong()
{
  Pong::instance = this;
}

void Pong::loopApp()
{
  if (_running)
  {
    if (gameModel.gameOver)
    {
      gameOver();
    }
    else
    {
      gameLoop();
    }
  }
}

void Pong::gameLoop()
{
  loadScene();
  drawPlayer();
  drawBall();      
}

void Pong::gameOver()
{
  
  _ledMatrix->clearDisplay(0);
  for (int i = 0; i < 3; i++)
  {
    beep();
    delay(10);
  }
  
  String message = "  Game Over  Pontos: ";
  message += gameModel.turnsCount;  

  char startMessage[message.length() + 1];
  message.toCharArray(startMessage, message.length() + 1);

  _ledMatrix->printStringScroll(0, 0, startMessage , MESSAGE_SPEED, '<');
}

void Pong::setupApp(String appName, LedControl *ledMatrix)
{
  BaseApp::setupApp(appName, ledMatrix);
  pinMode(BEEP, OUTPUT);     
}

void Pong::beep()
{
  digitalWrite(BEEP, HIGH);
  delay(BEEP_TIME);
  digitalWrite(BEEP, LOW);
}

void Pong::startApp()
{    
  initializeGame();
  BaseApp::startApp();
}

void Pong::stopApp()
{
  BaseApp::stopApp();  
  Timer1.detachInterrupt();
}

void Pong::initializeGame()
{
  gameModel.playerPosition = 3;
  
  gameModel.ballPosition[COORD_X] = 3;
  gameModel.ballPosition[COORD_Y] = 4;
  gameModel.ballAngle = BALL_UP;
  
  gameModel.gameSpeed = DEFAULT_GAME_SPEED;
  gameModel.gameOver = false;
  gameModel.turnsCount = 0;
  
  Timer1.attachInterrupt(functionUpdateBallPosition);
  Timer1.initialize(gameModel.gameSpeed);
}

void Pong::loadScene()
{
  _ledMatrix->setRow(0,0,B11111111);
  _ledMatrix->setRow(0,1,B00000001);
  _ledMatrix->setRow(0,2,B00000001);
  _ledMatrix->setRow(0,3,B00000001);
  _ledMatrix->setRow(0,4,B00000001);
  _ledMatrix->setRow(0,5,B00000001);
  _ledMatrix->setRow(0,6,B00000001);
  _ledMatrix->setRow(0,7,B11111111);
}

void Pong::drawPlayer()
{
  _ledMatrix->setRow(0, gameModel.playerPosition, B10000000);
  _ledMatrix->setRow(0, gameModel.playerPosition + 1, B10000000);
}

void Pong::drawBall()
{
  _ledMatrix->setLed(0, gameModel.ballPosition[COORD_X], gameModel.ballPosition[COORD_Y], true);
}

void Pong::move(int side)
{
  if (BUTTON_LEFT == side && gameModel.playerPosition > 1) {
    gameModel.playerPosition -= 1;
  }
  if (BUTTON_RIGHT == side  && gameModel.playerPosition < 5) {
    gameModel.playerPosition += 1;
  }
}

void Pong::updateBallPosition()
{
  // Eixo Y subindo
  if (ballBottomUp() && gameModel.ballPosition[COORD_Y] < TOPO_TELA)
  {
    gameModel.ballPosition[COORD_Y] += 1;
  }
  else if (ballBottomUp() && gameModel.ballPosition[COORD_Y] == TOPO_TELA)
  {
    defineOpositeYTop();
    gameModel.ballPosition[COORD_Y] -= 1;
  }
  // Eixo Y descendo
  else if (ballTopDown() && gameModel.ballPosition[COORD_Y] > BASE_TELA)
  {
    gameModel.ballPosition[COORD_Y] -= 1;
  }
  else if (ballTopDown() && gameModel.ballPosition[COORD_Y] == BASE_TELA && ballMatchPlayer())
  {
    defineOpositeYPlayer();
    gameModel.ballPosition[COORD_Y] += 1;
  }   
  else if (ballTopDown())
  {
    gameModel.ballPosition[COORD_Y] -= 1;
  }
  
  // Eixo X direita
  if (ballLeftRight() && gameModel.ballPosition[COORD_X] < TELA_DIREITA)
  {
    gameModel.ballPosition[COORD_X] += 1;
  }
  else if (ballLeftRight() && gameModel.ballPosition[COORD_X] == TELA_DIREITA)
  {
    defineOpositeX();
    gameModel.ballPosition[COORD_X] -= 1;
  }
  // Eixo X esquerda
  else if (ballRightLeft() && gameModel.ballPosition[COORD_X] > TELA_ESQUERDA)
  {
    gameModel.ballPosition[COORD_X] -= 1;
  }
  else if (ballRightLeft() && gameModel.ballPosition[COORD_X] == TELA_ESQUERDA)
  {
    defineOpositeX();
    gameModel.ballPosition[COORD_X] += 1;
  }

  // Verificando se o jogo ja terminou
  if (gameModel.ballPosition[COORD_Y] < 0)
  {
    gameModel.gameOver = true;
  }
  
}

boolean Pong::ballMatchPlayer()
{
  boolean retorno = ((gameModel.ballPosition[COORD_X] == gameModel.playerPosition) ||
          (gameModel.ballPosition[COORD_X] == gameModel.playerPosition + 1) ||
          (gameModel.ballAngle == BALL_DOWN_LEFT && gameModel.ballPosition[COORD_X] == gameModel.playerPosition + 2) ||
          (gameModel.ballAngle == BALL_DOWN_RIGHT && gameModel.ballPosition[COORD_X] == gameModel.playerPosition -1));
          
  if (retorno)
  {
    gameModel.gameSpeed *= DECREMENT_GAME_SPEED;    
    Timer1.initialize(gameModel.gameSpeed);
    
    gameModel.turnsCount++;
    
    if (gameModel.turnsCount > 50)
    {
      initializeGame();
    }
    
    beep();
  }
  
  return retorno;    
}

void Pong::defineOpositeX()
{
  if (gameModel.ballAngle == BALL_UP_RIGHT)
  {
    gameModel.ballAngle = BALL_UP_LEFT;
  }
  else if (gameModel.ballAngle == BALL_UP_LEFT)
  {
    gameModel.ballAngle = BALL_UP_RIGHT;
  }
  else if (gameModel.ballAngle == BALL_DOWN_RIGHT)
  {
    gameModel.ballAngle = BALL_DOWN_LEFT;
  }
  else if (gameModel.ballAngle == BALL_DOWN_LEFT)
  {
    gameModel.ballAngle = BALL_DOWN_RIGHT;
  }
}

void Pong::defineOpositeYTop()
{
  if (gameModel.ballAngle == BALL_UP)
  {
    gameModel.ballAngle = BALL_DOWN;
  }
  else if (gameModel.ballAngle == BALL_UP_RIGHT)
  {
    gameModel.ballAngle = BALL_DOWN_RIGHT;
  }
  else if (gameModel.ballAngle == BALL_UP_LEFT)
  {
    gameModel.ballAngle = BALL_DOWN_LEFT;
  }
}

void Pong::defineOpositeYPlayer()
{
  if (gameModel.ballAngle == BALL_DOWN && gameModel.ballPosition[COORD_X] == gameModel.playerPosition)
  {
    gameModel.ballAngle = BALL_UP_LEFT;
  }
  else if (gameModel.ballAngle == BALL_DOWN && gameModel.ballPosition[COORD_X] == gameModel.playerPosition + 1)
  {
    gameModel.ballAngle = BALL_UP_RIGHT;
  }
  else if (gameModel.ballAngle == BALL_DOWN_LEFT && gameModel.ballPosition[COORD_X] == gameModel.playerPosition)
  {
    gameModel.ballAngle = BALL_UP_LEFT;
  }
  else if (gameModel.ballAngle == BALL_DOWN_LEFT && gameModel.ballPosition[COORD_X] == gameModel.playerPosition + 1)
  {
    gameModel.ballAngle = BALL_UP;
  }
  else if (gameModel.ballAngle == BALL_DOWN_LEFT && gameModel.ballPosition[COORD_X] == gameModel.playerPosition + 2)
  {
    gameModel.ballAngle = BALL_UP_RIGHT;
  }
  else if (gameModel.ballAngle == BALL_DOWN_RIGHT && gameModel.ballPosition[COORD_X] == gameModel.playerPosition - 1)
  {
    gameModel.ballAngle = BALL_UP_LEFT;
  }
  else if (gameModel.ballAngle == BALL_DOWN_RIGHT && gameModel.ballPosition[COORD_X] == gameModel.playerPosition)
  {
    gameModel.ballAngle = BALL_UP;
  }
  else if (gameModel.ballAngle == BALL_DOWN_RIGHT && gameModel.ballPosition[COORD_X] == gameModel.playerPosition + 1)
  {
    gameModel.ballAngle = BALL_UP_RIGHT;
  }
}

boolean Pong::ballLeftRight()
{
  return (gameModel.ballAngle == BALL_UP_RIGHT || gameModel.ballAngle == BALL_DOWN_RIGHT);
}

boolean Pong::ballRightLeft()
{
  return (gameModel.ballAngle == BALL_UP_LEFT || gameModel.ballAngle == BALL_DOWN_LEFT);
}

boolean Pong::ballBottomUp()
{
  return (gameModel.ballAngle == BALL_UP || gameModel.ballAngle == BALL_UP_RIGHT || gameModel.ballAngle == BALL_UP_LEFT);
}

boolean Pong::ballTopDown()
{
  return (gameModel.ballAngle == BALL_DOWN || gameModel.ballAngle == BALL_DOWN_RIGHT || gameModel.ballAngle == BALL_DOWN_LEFT);
}

void functionUpdateBallPosition()
{
  Pong::instance->updateBallPosition();
}
