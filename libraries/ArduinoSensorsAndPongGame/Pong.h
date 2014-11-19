/*
  XBeeJoystick.h - Library for XBeeJoystick arduino code.
  Coded by Oidas Andrade.
*/
#ifndef Pong_h
#define Pong_h

#include <LedControl.h>
#include "BaseApp.h"
#include "GameModel.h"

class Pong: public BaseApp
{
  public:
    Pong();
    // Methods
    void startApp();
    void stopApp();
    void loopApp();
    void setupApp(String appName, LedControl *ledMatrix);
    void move(int side);
    void updateBallPosition();
    
    // Properties
    static Pong *instance;
    
  private:
    //Methods
    void initializeGame();
    void loadScene();
    void drawPlayer();
    void drawBall();
    void gameLoop();
    void gameOver();
    void gameWin();
    boolean ballMatchPlayer();
    boolean ballBottomUp();
    boolean ballTopDown();
    boolean ballLeftRight();
    boolean ballRightLeft();
    void defineOpositeX();
    void defineOpositeYTop();
    void defineOpositeYPlayer();
    void beep();
    
    // Properties
    GameModel gameModel;
};

// Foi declarado como funçao pois a API de Timer precisa de um ponteiro para funçao
void functionUpdateBallPosition();

#endif
