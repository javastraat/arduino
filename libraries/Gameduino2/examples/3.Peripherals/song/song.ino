#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

#define MUSICFILE   "mesmeriz.ima"

static Streamer stream;

void setup()
{
  GD.begin();
  stream.begin(MUSICFILE);
}

void loop()
{
  GD.cmd_gradient(0, 40, 0x282830,
                  0, 272, 0x606040);
  GD.cmd_text(240, 100, 31, OPT_CENTER, MUSICFILE);
  uint16_t val, range;
  stream.progress(val, range);
  GD.cmd_slider(30, 160, 420, 8, 0, val, range);
  GD.swap();
  GD.finish();
  stream.feed();
}
