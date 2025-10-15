#pragma once

#include "Tokenizer.hpp"
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using bytecode = struct {
  std::bitset<8> inst;
  std::bitset<32> operand;
  std::bitset<2> _type;
};

class Compiler {
private:
  std::vector<Token> tokens;
  std::vector<bytecode> bin;

public:
  Compiler(std::string file_name) {
    std::ifstream file(file_name);
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto tokenizer = Tokenizer(buffer.str());
    tokens = tokenizer.get_tokens();
  }

  void compile() {
    for (auto token : tokens) {
    }
  }
};
