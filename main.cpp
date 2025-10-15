#include "Compiler.hpp"

int main() {
  auto comp = Compiler("asm.s");
  comp.compile("asm.bin");

  return 0;
}
