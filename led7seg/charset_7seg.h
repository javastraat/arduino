// Character set data for 7-segment display
// Eli McIlveen
// 22 July 2007
// http://www.forgeryleague.com/
//
// This code is hereby placed in the public domain.

// Uses ASCII character map except for control characters. Can easily be cut down 
// to save space in memory.
//
// The first 16 characters are hex digits 0-F, to simplify the display of numerals.
// Characters 16-23 and 24-31 provide an animated "figure-8" cycle with one and two
// segments lit at a time, respectively.
//
// For obvious reasons, some letters and punctuation are rather hard to decipher. 
// Rather than duplicate shapes, alternative versions of some characters are offered 
// (e.g. 6 7 9, upper/lowercase letters).

// Bit order: DP A B C D E F G
// Segments A-F run clockwise from the top; G is the middle bar; DP = decimal point.
//
// Pins to connect to common-cathode LED display via a 74HC595:
// DP-15, A-1, B-2, C-3, D-4, E-5, F-6, G-7 (shiftOut using LSBFIRST)
// Or:
// DP-7, A-6, B-5, C-4, D-3, E-2, F-1, G-15 (shiftOut using MSBFIRST)



const byte ledCharSet[128] = {
	// 00-0F: Hex digits
	B01111110, B00110000, B01101101, B01111001,	// 0123
	B00110011, B01011011, B01011111, B01110000,	// 4567
	B01111111, B01111011, B01110111, B00011111,	// 89AB
	B01001110, B00111101, B01001111, B01000111,	// CDEF

	// 10-1F: Figure-8 drawing (8-character cycle)
	B01000000, B00100000, B00000001, B00000100,	// 1-segment
	B00001000, B00010000, B00000001, B00000010,

	B01100000, B00100001, B00000101, B00001100,	// 2-segment
	B00011000, B00010001, B00000011, B01000010,

	// 20-2F: Punctuation (barely recognizable!)
	B00000000, B10100000, B00100010, B00111111,	//  !"#
	B01011010, B01001001, B00000111, B00000010,	// $%&'
	B01001110, B01111000, B01100011, B00110001,	// ()*+
	B00010000, B00000001, B10000000, B00100101,	// ,-./

	// 30-3F: Decimal digits (alternate) and more punctuation
	B01111110, B00110000, B01101101, B01111001,	// 0123
	B00110011, B01011011, B00011111, B01110010,	// 4567
	B01111111, B01110011, B01001000, B01010000,	// 89:;
	B00001101, B00001001, B00011001, B11100000,	// <=>?

	// 40-5F: Capital letters and punctuation
	B01101110, B01110111, B00011111, B01001110,	// @ABC
	B01111100, B01001111, B01000111, B01011110,	// DEFG
	B00110111, B00000110, B00111100, B01010111,	// HIJK
	B00001110, B01110110, B00010101, B01111110,	// LMNO

	B01100111, B01110011, B01000110, B01011011,	// PQRS
	B01110000, B00111110, B00111110, B00101011,	// TUVW
	B01110101, B00111011, B01101101, B01001110,	// XYZ[
	B00010011, B01111000, B01100010, B00001000,	// \]^_

	// 60-7F: Lowercase letters and punctuation
	B01100000, B01111101, B00011111, B00001101,	// `abc
	B00111101, B01101111, B01000111, B01111011,	// defg
	B00010111, B00010000, B00011000, B00101111,	// hijk
	B00001100, B01010101, B01101010, B00011101,	// lmno

	B01100111, B01110011, B00000101, B00010011,	// pqrs
	B00001111, B00011100, B00100011, B01011101,	// tuvw
	B01101100, B00111011, B00100101, B01000011,	// xyz{
	B00110110, B01100001, B01000000, B11111111	// |}~
};
