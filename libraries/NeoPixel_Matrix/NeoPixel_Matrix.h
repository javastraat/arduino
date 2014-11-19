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
#ifndef NEOPIXEL_MATRIX_H
#define NEOPIXEL_MATRIX_H

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>

class NeoPixel_Matrix : public Adafruit_GFX
{
public:
  // Constructor (provides same default pin/type values as Adafruit_NeoPixel)
  NeoPixel_Matrix(int16_t w, int16_t h, uint8_t p=6, uint8_t t=NEO_GRB + NEO_KHZ800);

  // Adafruit_GFX methods
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void fillScreen(uint16_t color);

  // Apply changes
  void display();

  // Create a 16-bit color from packed 24-bit RGB
  static uint16_t Color(uint32_t rgb);
  // Create a 16-bit color from unpacked 24-bit RGB
  static uint16_t Color(uint8_t r, uint8_t g, uint8_t b);

  // Map a 16-bit color for Adafruit_GFX into a native packed 24-bit color
  uint32_t NativeColor(uint16_t color);

protected:
  // actual pixels
  Adafruit_NeoPixel pixels;
};

#endif // NEOPIXEL_MATRIX_H
