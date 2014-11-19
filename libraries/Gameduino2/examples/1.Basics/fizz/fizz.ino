#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

void setup()
{
  GD.begin();
}

void loop()
{
  GD.Clear();
  GD.Begin(POINTS);
  for (int i = 0; i < 100; i++) {
    GD.PointSize(GD.random(16 * 50));
    GD.ColorRGB(GD.random(256),
                GD.random(256),
                GD.random(256));
    GD.ColorA(GD.random(256));
    GD.Vertex2ii(GD.random(480), GD.random(272));
  }
  GD.swap();
}
