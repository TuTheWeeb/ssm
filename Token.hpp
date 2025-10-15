#pragma once
#include <cstdint>
#include <print>
#include <unordered_map>
#include <variant>

class Token {
private:
  enum class Token_Type { INT, FLOAT, STR, INST };

  std::unordered_map<std::string, std::uint8_t> hash_table = {
      {"push", 1}, {"pop", 2}, {"add", 3}, {"sub", 4}};

  // Declare the type variable and the variant value
  Token_Type _type;

  std::variant<std::int32_t, float, std::uint8_t> value;

  std::int32_t get_int_value() {
    return std::holds_alternative<std::int32_t>(this->value)
               ? std::get<int32_t>(this->value)
               : 0;
  }

  std::uint8_t get_uint8_value() {
    return std::holds_alternative<std::uint8_t>(this->value)
               ? std::get<std::uint8_t>(this->value)
               : 0;
  }

  float get_float_value() {
    return std::holds_alternative<float>(this->value)
               ? std::get<float>(this->value)
               : 0.0;
  }

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

  void print_value() {
    if (get_type() == Token::Token_Type::INT) {
      std::println("int: {}", get_int_value());
    }
    if (get_type() == Token::Token_Type::STR) {
      std::println("str: {}", get_uint8_value());
    }
    if (get_type() == Token::Token_Type::INST) {
      std::println("inst: {}", get_uint8_value());
    }
    if (get_type() == Token::Token_Type::FLOAT) {
      std::println("float: {}", get_float_value());
    }
  }
};
