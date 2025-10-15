#pragma once

#include "Tokenizer.hpp"
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct bytecode {
  std::bitset<8> inst = 0;
  std::bitset<8> addressing = 0; // 0: INST, 1: INT, 2: STR , 3: FLOAT , 4: MEM
  std::bitset<32> operand = 0;
};

void write_bytecode(std::ofstream &file, const bytecode &bc) {
  if (!file)
    return;

  std::uint8_t inst = bc.inst.to_ulong();
  std::uint8_t addressing = bc.addressing.to_ulong();
  std::uint32_t operand = bc.operand.to_ulong();

  file.write(reinterpret_cast<char *>(&inst), sizeof(inst));
  file.write(reinterpret_cast<char *>(&addressing), sizeof(addressing));
  file.write(reinterpret_cast<char *>(&operand), sizeof(operand));
}

void read_bytecode(std::ifstream &file, bytecode &bc) {
  if (!file)
    return;

  std::uint8_t inst = 0;
  std::uint8_t addressing = 0;
  std::uint32_t operand = 0;

  file.read(reinterpret_cast<char *>(&inst), sizeof(inst));
  file.read(reinterpret_cast<char *>(&addressing), sizeof(addressing));
  if (addressing != 0) {
    file.read(reinterpret_cast<char *>(&operand), sizeof(operand));
  }
}

class Compiler {
private:
  std::vector<Token> tokens;
  std::vector<bytecode> bin;
  std::size_t m_index = 0;

  std::optional<Token> peak() {
    if (m_index >= this->tokens.size()) {
      return {};
    }

    return this->tokens.at(m_index);
  }

  void consume() { this->m_index++; }

  template <std::uint32_t T> std::bitset<T> mask_bits(std::bitset<32> bits) {
    std::bitset<T> n_val = bits.to_ulong() & ((T * 2) - 1);
    return n_val;
  }

  Token::Token_Type get_type() { return peak().value().get_type(); }

  std::bitset<32> get_value() { return peak().value().get_value(); }

public:
  Compiler(std::string file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
      std::cerr << "ERROR: This file does exists: " << file_name << std::endl;
      std::exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto tokenizer = Tokenizer(buffer.str());
    tokens = tokenizer.get_tokens();
  }

  void compile(std::string binary_name = "out") {
    bytecode byt = {0};
    while (peak().has_value()) {
      if (get_type() == Token::Token_Type::INST) {
        byt.inst = mask_bits<8>(get_value());
        consume();
        if (peak().has_value() && get_type() != Token::Token_Type::INST) {
          byt.operand = get_value();
          switch (get_type()) {
          case Token::Token_Type::INT:
            byt.addressing = 1;
            break;
          case Token::Token_Type::STR:
            byt.addressing = 2;
            break;
          case Token::Token_Type::FLOAT:
            byt.addressing = 3;
            break;
          case Token::Token_Type::MEM:
            byt.addressing = 4;
            break;
          default:
            byt.addressing = 0;
            break;
          }
          consume();
        }
        bin.push_back(byt);
        byt = {0};
      }
    }

    std::ofstream file(binary_name, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "ERROR: could not create the binary file!" << std::endl;
      std::exit(1);
    }
    for (auto byt : bin) {
      write_bytecode(file, byt);
    }
  }
};
