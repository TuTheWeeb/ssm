#pragma once

#include "Tokenizer.hpp"
#include "bytecode.hpp"
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Assembler {
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

  template <std::uint32_t T> std::bitset<T> mask_bits(std::bitset<64> bits) {
    std::bitset<T> n_val = bits.to_ulong() & ((T * 2) - 1);
    return n_val;
  }

  Token::Token_Type get_type() { return peak().value().get_type(); }

  std::bitset<64> get_value() { return peak().value().get_value(); }

  void write_entire_buffer(const std::string file_path) {
    std::ofstream file(file_path, std::ios::binary | std::ios::trunc);

    // Multiplies by 10 so that it has 80 bits
    std::vector<std::uint8_t> buffer(bin.size() * 10);

    for (const auto &k : bin) {
      // Copy the instruction and the addressing to buffer
      buffer[0] = static_cast<std::uint8_t>(k.inst.to_ulong());
      buffer[1] = static_cast<std::uint8_t>(k.addressing.to_ulong());

      // Copy the operand to buffer
      std::uint64_t operand = k.operand.to_ullong();
      std::memcpy(&buffer[2], &operand, sizeof(operand));
    }

    // Write the entire buffer
    file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
  }

public:
  Assembler(std::string file_name) {
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

  void assemble(std::string binary_name = "out") {
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

    write_entire_buffer(binary_name);
  }
};
