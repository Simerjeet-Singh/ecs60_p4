#ifndef cpuH
#define cpuH

#include "CPURunner.h"

class CPU
{
public:
  CPU();
  bool a, b;
  int tableSize;
  int tempAddr;
  int mybuffer1, mybuffer2, index1, index2, index3;  
  int last;
  typedef struct cacheBlock
  {
    cacheBlock():tag(0),value(0),isValid(0){ }
    unsigned tag;
    unsigned value;
    bool isValid;
  } cacheBlock;
  cacheBlock arr[5329];
  void operation(OpCode &opCode, Instruction &instruction, int &buffer);
};
#endif
