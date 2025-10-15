#pragma once
#include <bitset>
#include <cstdint>
#include <print>
#include <unordered_map>

class Token {
private:
  enum class Token_Type { INT, FLOAT, STR, INST };

  std::unordered_map<std::string, std::uint8_t> hash_table = {
      {"push", 1}, {"pop", 2}, {"add", 3}, {"sub", 4}};

  // Declare the type variable and the variant value
  Token_Type _type;

  std::bitset<32> value;

public:
  Token(std::string minemonic) {
    if (std::isdigit(static_cast<unsigned char>(minemonic.at(0))) or
        minemonic.at(0) == '-') {
      if (minemonic.find(".") != std::string::npos) {
        _type = Token_Type::FLOAT;
        value = std::stof(minemonic);
      } else {
        _type = Token_Type::INT;
        value = std::stoi(minemonic);
      }
    } else if (hash_table.find(minemonic) != hash_table.end()) {
      _type = Token_Type::INST;
      value = hash_table.find(minemonic)->second;
    }
  }

  Token_Type get_type() { return this->_type; }

  std::uint32_t get_value() { return this->value.to_ulong(); }

  void print_value() {
    if (get_type() == Token::Token_Type::INT) {
      std::println("int: {}", (std::int32_t)get_value());
    }
    if (get_type() == Token::Token_Type::STR) {
      std::println("str: {}", (std::uint8_t)get_value());
    }
    if (get_type() == Token::Token_Type::INST) {
      std::println("inst: {}", (std::uint8_t)get_value());
    }
    if (get_type() == Token::Token_Type::FLOAT) {
      std::println("float: {}", (float)get_value());
    }
  }
};
