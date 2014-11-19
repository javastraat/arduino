#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

#include "tiled_assets.h"

void setup()
{
  GD.begin();
  LOAD_ASSETS();
}

void loop()
{
  GD.Clear();
  GD.Begin(BITMAPS);
  GD.BitmapSize(BILINEAR, REPEAT, REPEAT, 480, 272);
  GD.cmd_rotate(3333);
  GD.cmd_setmatrix();
  GD.Vertex2ii(0, 0);
  GD.swap();
}
