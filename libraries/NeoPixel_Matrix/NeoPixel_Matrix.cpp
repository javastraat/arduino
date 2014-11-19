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
#include "NeoPixel_Matrix.h"


static inline uint8_t downsample(uint8_t v, uint8_t bits)
{
    uint8_t tmp;
    tmp = v + (1 << (bits - 1));
    return ((tmp > v) ? tmp : v) >> bits;
}


NeoPixel_Matrix::NeoPixel_Matrix(int16_t w, int16_t h, uint8_t p, uint8_t t):
    Adafruit_GFX(w, h), pixels(w * h, p, t)
{
  pixels.begin();
}


uint16_t NeoPixel_Matrix::Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (downsample(r, 3) << 11 | downsample(g, 2) << 5 | downsample(b, 3));
}


uint16_t NeoPixel_Matrix::Color(uint32_t rgb)
{
  uint8_t r, g, b;
  r = (rgb >> 16) & 0xff;
  g = (rgb >> 8)  & 0xff;
  b = (rgb >> 0)  & 0xff;
  return Color(r, g, b);
}


uint32_t NeoPixel_Matrix::NativeColor(uint16_t color)
{
  uint8_t r, g, b;
  // extract red
  r = (color >> 11) & ((1 << 5) - 1);
  r <<= 3;
  // fill 3 LSb with copy of 3 MSb (for a smoother mapping)
  r |= r >> 5;
  // extract green
  g = (color >> 5) & ((1 << 6) - 1);
  g <<= 2;
  // fill 2 LSb with copy of 2 MSb (for a smoother mapping)
  g |= g >> 6;
  // extract blue
  b = (color) & ((1 << 5) - 1);
  b <<= 3;
  // fill 3 LSb with copy of 3 MSb (for a smoother mapping)
  b |= b >> 5;

  return pixels.Color(r, g, b);
}


void NeoPixel_Matrix::display()
{
  pixels.show();
}


void NeoPixel_Matrix::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
  {
    return;
  }

  // check rotation, move pixel around if necessary
  switch (getRotation())
  {
    case 0:
      break;
    case 1:
      swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      y = HEIGHT - y - 1;
      break;
  }

  pixels.setPixelColor(y * WIDTH + x, NativeColor(color));
}


void NeoPixel_Matrix::fillScreen(uint16_t color)
{
  uint32_t native_color = NativeColor(color);

  for (uint16_t i = 0; i < pixels.numPixels(); ++i)
  {
    pixels.setPixelColor(i, native_color);
  }  
}
