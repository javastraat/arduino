Colorduino GFX Library for Arduino
==================================

This library depends on a slightly modified version of the Adafruit GFX library and provides advanced drawing functions(lines, circles, text, ...) for the Colorduino or the Colors Shield.

Installation
------------

The Colorduino GFX library depends on the Adafruit GFX library. Use the original Adafruit GFX library or the 8-bit mode of the slightly modified version for a smaller code size. Download and install the dependencies first.

Download the source code and rename the uncompressed folder to Colorduino\_GFX. The folder should at least contain the files Colorduino\_GFX.cpp and Colorduino\_GFX.h. Place the folder in your `<Arduino-Sketch-Folder>/libraries/` folder and restart the Arduino IDE.

Usage
-----

Include all required libraries first.

    #include <Adafruit_GFX.h>
    #include <Colorduino_GFX.h>

Create a new Colorduino instance.

    ColorduinoPanel Colorduino;

Initialize the Colorduino and set the white-balance in the setup() function.

    void setup() {
      Colorduino.init();
      Colorduino.setWhiteBalance(36, 63, 63);
    }

Now the Colorduino is ready to use.

    void loop() {
        GFX_Color_t color = Colorduino.color(255, 0, 0);
        Colorduino.drawLine(0, 0, 7, 7, color);
        Colorduino.swapBuffers(true);
    }


Licenses
========

Library (LGPL)
--------------

The library is licensed under the LGPL 2.1 or a later version of the license.

    The source code is based on C code by zzy@iteadstudio and Sam C. Lin
      Copyright (c) 2010 zzy@IteadStudio.  All right reserved.
      Copyright (c) 2011-2012 Sam C. Lin <lincomatic@hotmail.com>

Examples (LGPL)
---------------

The examples are licensed under the LGPL 2.1 or a later version of the license.

Additional resources
====================

* [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
* [Colorduino GFX Library](https://github.com/DinoTools/Colorduino_GFX) (Sources and Bug Tracker)
* [Colorduino GFX Library project page](https://github.com/DinoTools/Colorduino_GFX)
* [Modified version of the Adafruit GFX Library](https://github.com/dinotools/Adafruit-GFX-Library) (optional)
* [Original Colorduino Library](https://github.com/lincomatic/Colorduino)
