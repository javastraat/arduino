/*
 * This file is part of the NeoPixel_Matrix library.
 * Copyright 2013 Justin R. Cutler <justin.r.cutler@gmail.com>
 *
 * NeoPixel_Matrix is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * NeoPixel_Matrix is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with NeoPixel_Matrix.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <NeoPixel_Matrix.h>


// Adafruit NeoPixel Shield uses default pin / type
NeoPixel_Matrix shield = NeoPixel_Matrix(8, 5);


void setup()
{
  // set normal size font
  shield.setTextSize(1);
  // set text color to 1/8 white on black
  shield.setTextColor(shield.Color(32, 32, 32), 0);
  // turn of text wrapping
  shield.setTextWrap(false);
  // Initialize all pixels to 'off'
  shield.fillScreen(0);
  shield.display();
}


void marquee(NeoPixel_Matrix &m, const char *s, uint16_t ms)
{
  int16_t width = 0;

  // calculate printed width
  for (const char *c = s; *c; ++c)
  {
    width += 6;
  }

  // clear screen
  m.fillScreen(0);

  // scroll message
  for (int16_t x = m.width(); x >= -width; --x)
  {
    m.setCursor(x, 0);
    m.print(s);
    m.display();
    delay(ms);
  }
}


void loop()
{
  // rotate shield 90 deg CCW to display one character
  shield.setRotation(1);
  // display message
  marquee(shield, "Hello, world!", 100);
  delay(500);

  // fill red
  shield.fillScreen(shield.Color(255,0,0));
  shield.display();
  delay(500);
  // fill green
  shield.fillScreen(shield.Color(0,255,0));
  shield.display();
  delay(500);
  // fill blue
  shield.fillScreen(shield.Color(0,0,255));
  shield.display();
  delay(500);
  // fill black
  shield.fillScreen(shield.Color(0,0,0));
  shield.display();
  delay(500);

  for (uint8_t rotation = 0; rotation < 4; ++rotation)
  {
    int16_t min_xy = min(shield.width(), shield.height());
    shield.setRotation(rotation);
    shield.fillScreen(0);
    shield.fillTriangle(0, 0, shield.width()-1, shield.height()-1, 0, shield.height()-1, shield.Color(64,0,0));
    shield.drawCircle(min_xy >> 1, min_xy >> 1, min_xy >> 1, shield.Color(0,64,0));
    shield.drawRect(shield.width()>>1, shield.height()>>1, 3, 3, shield.Color(0,0,64));
    shield.display();
    delay(1000);
  }
}
