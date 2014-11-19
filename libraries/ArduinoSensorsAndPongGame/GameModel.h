/*
  XBeeJoystick.h - Library for XBeeJoystick arduino code.
  Coded by Oidas Andrade.
*/
#ifndef GameModel_h
#define GameModel_h

#define BALL_UP 0
#define BALL_UP_RIGHT 1
#define BALL_UP_LEFT 2
#define BALL_DOWN 3
#define BALL_DOWN_RIGHT 4
#define BALL_DOWN_LEFT 5

class GameModel
{    
  public:
    int playerPosition;
    int ballPosition[2];
    int ballAngle;
    long gameSpeed;
    boolean gameOver;
    long turnsCount;
};

#endif
