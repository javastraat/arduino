MaxMatrix2
==========

Arduino Library for MAX7219 Serially Interfaced, 8-Digit, LED Display Drivers

It is very unfinished at the moment, but the basics should work

Based upon [MaxMatrix](https://code.google.com/p/arudino-maxmatrix-library/) by Oscar Kin-Chung Au


Motivation:
-----------
This is currently (and most likely will always be) a one-man project for my own education and general fooling around.

Obviously there are already numerous implementations and libraries to talk to the MAX7219 or MAX7221 LED-driver-ICs from Arduino (or similar) boards. 
However, none pleased me *personally* and the above linked one was the first library I got working by stripping it down to it's bare essentials.
I am now slowly re-adding the functionality in a way that makes sense to me. Hopefully this might be useful for someone stumbling across.


Todo:
-----
- Add support for 7-Segment displays as well as LED-Matrices
- Add more core functionality like Text display (see original Library)
- Make it all efficcient (Talk to all matricies instead of one at a time)

