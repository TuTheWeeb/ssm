#include "Assembler.hpp"
#include "Virtualmachine.hpp"

int main() {
  auto assm = Assembler("asm.s");
  assm.assemble("asm.bin");

  auto _ = SSMVm("asm.bin");

  return 0;
}
