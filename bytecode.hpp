#pragma once
#include <bitset>

struct bytecode {
  std::bitset<8> inst = 0;       // 0: ERROR, 1: push, 2: pop, 3: add, 4: sub
  std::bitset<8> addressing = 0; // 0: INST, 1: INT, 2: STR , 3: FLOAT , 4: MEM
  std::bitset<64> operand = 0;
};
