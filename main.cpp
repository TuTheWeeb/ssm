#include "./Tokenizer.hpp"
#include <fstream>
#include <sstream>

int main() {
  std::ifstream file("asm.s");
  std::stringstream buffer;
  buffer << file.rdbuf();

  auto tokens = Tokenizer(buffer.str()).get_tokens();

  for (auto token : tokens) {
    token.print_value();
  }

  return 0;
}
