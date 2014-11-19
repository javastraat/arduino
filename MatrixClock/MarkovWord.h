#ifndef markovword_h
#define markovword_h
#include <avr/pgmspace.h>

class CMarkovWord
{
public:
  static char* Word(char* Buffer, int BufferSize, int MinLength, int MaxLength);

private:
  static char NextChar(char First, char Second);
  static prog_uint16_t kMarkovTable[];
};

#endif

