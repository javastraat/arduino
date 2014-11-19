#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

#include "abstract_assets.h"

struct xy {
  int x, y;
};

#define MODE 2

#if MODE == 1
#define SPEED       4     // Wave propagation speed
#define RADIUS      400   // Blob drifting radius
#define DRIFT_RATE  60    // Blob drifting rate, furmans/frame
#define NSTARS      32

static xy bgstars[NSTARS];
#endif

#if MODE == 2
static xy pos[20] = {
{20,69},{60,53},{110,41},{174,36},{314,24},{393,41},{460,60},{245,30},{4,178},{65,126},{134,142},{198,97},{263,132},{336,100},{428,137},{526,135},{85,229},{198,217},{317,215},{418,243},
};
#endif

#if MODE == 3
#define NUMBLOBS 128
struct {
  int x, y;
  signed char dx, dy;
} blobs[NUMBLOBS];
static int v() {
  return random(64) - 32;
}
#endif

void setup()
{
  GD.begin();
  GD.load("abstract.gd2");

#if MODE == 1
  for (int i = 0; i < NSTARS; i++) {
    bgstars[i].x = random(16 * (480 - 64));
    bgstars[i].y = random(16 * (272 - 64));
  }
#endif

  // for (int i = 0; i < 20; i++) {
  //   pos[i].x = 30 + 60 * (i % 8);
  //   pos[i].y = 30 + 60 * (i / 8);
  // }
#if MODE == 3

  for (byte i=0; i<NUMBLOBS; ++i) {
    blobs[i].x = random(16 * 480);
    blobs[i].y = random(16 * 272);
    blobs[i].dx = v();
    blobs[i].dy = v();
  }
#endif

  Serial.begin(1000000);
}

uint16_t drift;
byte idx;
byte heights[128];
byte excitation;

static int t = 0;

#if MODE == 1
void loop()
{
  GD.get_inputs();
  GD.cmd_gradient(0, 0, 0x000000, 480, 272, 0x000040);

  // int touching = (GD.inputs.x != -32768);
  int touching = pgm_read_byte(beats + t);

  for (int i = 0; i < SPEED; i++) {
    heights[127 & (idx + i)] = excitation;
    if (touching)
      excitation = min(255, excitation + 15);
    else
      excitation = excitation * 63 / 64;
  }

  if (1) {
    uint32_t scale = 0xc000U + (excitation << 6);

    GD.cmd_translate(F16(32),F16(32));
    GD.cmd_scale(scale, scale);
    GD.cmd_translate(F16(-32),F16(-32));
    GD.cmd_setmatrix();

    GD.ColorA(70 + (excitation >> 4));
    GD.Begin(BITMAPS);
    for (int i = 0; i < NSTARS; i++) {
      uint16_t th = drift + (i << 11);
      uint32_t c = pgm_read_dword(saturated + (i << 2));
      GD.ColorRGB(c | 0xc0c0c0);
      GD.Vertex2f(bgstars[i].x + GD.rsin(RADIUS, th), bgstars[i].y + GD.rcos(RADIUS, th));
    }
  }
  drift += DRIFT_RATE + (excitation << 0);

  GD.BlendFunc(SRC_ALPHA, ONE);
  GD.ColorA(50);
  GD.Begin(LINES);
  byte attenuation = 128 + excitation / 2;
  for (int i = 0; i < 128; i++) {
    uint8_t j = ((idx + SPEED - 1) - i) & 127;
    byte h = heights[j];
    uint32_t c = pgm_read_dword(saturated + 2 * i);
    GD.ColorRGB(c);
    h = h * attenuation / 256;
    GD.LineWidth(h);
    GD.Vertex2ii(48 + 3 * i, 136 - h/2, 0, 0);
    GD.Vertex2ii(48 + 3 * i, 136 + h/2, 0, 0);
  }
  idx = (idx + SPEED) & 127;

  GD.swap();
  t++;
}
#endif

#if MODE == 2
static void gauge(uint32_t c, int x, int y, int r)
{
  GD.SaveContext();
  GD.ColorA(0x60);
  GD.ColorRGB(0x000000);
  GD.PointSize((r << 4) + 30);
  GD.Begin(POINTS);
  GD.Vertex2f(x << 4, y << 4);
  GD.RestoreContext();
  GD.ColorRGB(0x000000);
  GD.cmd_bgcolor(c);
  GD.cmd_gauge(x, y, r, OPT_FLAT | OPT_NOPOINTER, 4, 2, excitation, 255);
  GD.ColorRGB(0xd04040);
  GD.cmd_gauge(x, y, r, OPT_FLAT | OPT_NOBACK | OPT_NOTICKS, 4, 2, excitation, 255);
}

void loop()
{
  GD.get_inputs();

  int touching = pgm_read_byte(beats + t);
  // touching |= (t & 16);
  if (touching)
    excitation = min(255, excitation + 30);
  else
    excitation = excitation * 15 / 16;

  GD.ClearColorRGB(0x30303c);
  GD.Clear();
  GD.BitmapHandle(METAL_HANDLE);
  GD.BitmapSize(BILINEAR, REPEAT, REPEAT, 512, 512);
  GD.Begin(BITMAPS);
  GD.ColorMask(1, 0, 0, 0);
  int d = excitation >> 2;
  GD.Vertex2f(-d, -d);
  GD.ColorMask(0, 0, 1, 0);
  GD.Vertex2f(d, d);
  GD.ColorMask(0, 1, 0, 0);
  GD.BitmapSize(NEAREST, REPEAT, REPEAT, 480, 272);
  GD.Vertex2f(0, 0);

  GD.ColorMask(1, 1, 1, 1);

  for (int i = 0; i < 20; i++) {
    uint32_t c = pgm_read_dword(saturated + 30 + 2 * i);
    c = (c >> 2) & 0x3f3f3f;
    c = 4 * i + 170;
    c = c | (c << 8) | (c << 16);
    GD.Tag(1 + i);
    gauge(c, pos[i].x, pos[i].y, 20 + 2 * i);
  }

  // Frame counter
  if (0) {
    static uint32_t prev;
    uint32_t f = GD.rd32(REG_FRAMES);
    prev = f;
  }

  static byte prev_tag;
  static int down_x, down_y;
  if (GD.inputs.tag) {
    if (GD.inputs.tag == prev_tag) {
      byte i = GD.inputs.tag - 1;
      pos[i].x += GD.inputs.tag_x - down_x;
      pos[i].y += GD.inputs.tag_y - down_y;
      for (i = 0; i < 20; i++) {
        Serial.print("{");
        Serial.print(pos[i].x, DEC);
        Serial.print(",");
        Serial.print(pos[i].y, DEC);
        Serial.print("},");
      }
      Serial.println();
    }
    down_x = GD.inputs.tag_x;
    down_y = GD.inputs.tag_y;
  }
  prev_tag = GD.inputs.tag;


  GD.swap();
  t++;
}
#endif

#if MODE == 3

#define X0 (16 * (480 / 3))
#define X1 (16 * (2 * 480 / 3))
#define Y0 (16 * (272 / 3))
#define Y1 (16 * (2 * 272 / 3))

static void drawblobs(byte lo, byte hi)
{
  GD.SaveContext();
  GD.ColorRGB(0x000000);
  GD.ColorA(0x80);
  for (byte i = lo; i < hi; i++) {
    GD.PointSize(3 * i + 72);
    GD.Vertex2f(blobs[i].x, blobs[i].y);
  }
  GD.RestoreContext();
  for (byte i = lo; i < hi; i++) {
    GD.PointSize(3 * i);
    GD.Vertex2f(blobs[i].x, blobs[i].y);
  }
}

void loop()
{
  int touching = pgm_read_byte(beats + t);

  int centerx = (16 * (480 / 2));
  int centery = (16 * (272 / 2));

  int ACCEL = 1;
  int iters = touching ? 5 : 1;
  for (byte j = 0; j < iters; j++) {
    for (byte i = 0; i < NUMBLOBS; ++i) {
      centerx = (i & 32) ? X0 : X1;
      centery = (i & 64) ? Y0 : Y1;
      if (blobs[i].x < centerx)
        blobs[i].dx += ACCEL;
      else
        blobs[i].dx -= ACCEL;
      if (blobs[i].y < centery)
        blobs[i].dy += ACCEL;
      else
        blobs[i].dy -= ACCEL;
      blobs[i].x += blobs[i].dx << 0;
      blobs[i].y += blobs[i].dy << 0;
    }
  }
  blobs[random(NUMBLOBS)].dx = v();
  blobs[random(NUMBLOBS)].dy = v();

  // GD.ClearColorRGB(touching ? 0x000040 : 0);

  GD.BitmapHandle(GREEN_HANDLE);
  GD.BitmapSize(BILINEAR, REPEAT, REPEAT, 480, 272);

  GD.cmd_loadidentity();
  GD.cmd_translate(F16(240),F16(136));
  GD.cmd_rotate(t << 8);
  GD.cmd_translate(F16(-240),F16(-136));
  GD.cmd_setmatrix();

  GD.ColorRGB(0xd0d0d0);
  GD.Begin(BITMAPS);
  GD.Vertex2ii(0, 0, GREEN_HANDLE, 0);

  GD.cmd_loadidentity();
  GD.cmd_setmatrix();
  GD.ColorA(0x90);
  GD.ColorRGB(0x000000);
  GD.Vertex2ii(0, 0, VIGNETTE_HANDLE, 0);

  GD.Begin(POINTS);

  GD.ColorA(0xff);
  GD.ColorRGB(0xff0000);
  drawblobs(0, 32);

  GD.ColorRGB(0xff4040);
  drawblobs(32, 64);

  GD.ColorRGB(0xff8080);
  drawblobs(64, 96);

  GD.ColorRGB(0xffe0e0);
  drawblobs(96, 128);


  // Frame counter
  if (0) {
    static uint32_t prev;
    uint32_t f = GD.rd32(REG_FRAMES);
    GD.cmd_number(0, 0, 31, 0, f - prev);
    prev = f;
  }
  GD.swap();
  t++;
}
#endif
