#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

#define NBLOBS      128
#define OFFSCREEN   -16384

struct xy {
  int x, y;
} blobs[NBLOBS];

void setup()
{
  GD.begin();

  for (int i = 0; i < NBLOBS; i++) {
    blobs[i].x = OFFSCREEN;
    blobs[i].y = OFFSCREEN;
  }
}

void loop()
{
  static byte blob_i;
  GD.get_inputs();
  if (GD.inputs.x != -32768) {
    blobs[blob_i].x = GD.inputs.x << 4;
    blobs[blob_i].y = GD.inputs.y << 4;
  } else {
    blobs[blob_i].x = OFFSCREEN;
    blobs[blob_i].y = OFFSCREEN;
  }
  blob_i = (blob_i + 1) & (NBLOBS - 1);

  GD.ClearColorRGB(0xe0e0e0);
  GD.Clear();

  GD.Begin(POINTS);
  for (int i = 0; i < NBLOBS; i++) {
    // Blobs fade away and swell as they age
    GD.ColorA(i << 1);
    GD.PointSize((1024 + 16) - (i << 3));

    // Random color for each blob, keyed from (blob_i + i)
    uint8_t j = (blob_i + i) & (NBLOBS - 1);
    byte r = j * 17;
    byte g = j * 23;
    byte b = j * 147;
    GD.ColorRGB(r, g, b);

    // Draw it!
    GD.Vertex2f(blobs[j].x, blobs[j].y);
  }
  GD.swap();
}
