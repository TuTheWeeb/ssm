#pragma once
#include <bitset>
#include <cstdint>
#include <cstring>
#include <print>
#include <stdfloat>
#include <unordered_map>

class Token {
public:
  enum class Token_Type { INT, FLOAT, STR, INST, MEM };

private:
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
        float tempf = std::stof(minemonic);
        std::memcpy(&value, &tempf, sizeof(float));
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

  std::bitset<32> get_value() { return this->value; }
};
