#pragma once
#include <optional>
#include <vector>

#include "Token.hpp"
#include <string>

class Tokenizer {
public:
  Tokenizer(std::string file_s) : m_source(std::move(file_s)) {}

  std::vector<Token> get_tokens() {
    std::vector<Token> tokens;
    std::string buff = "";

    while (this->peak().has_value()) {

      // Registers
      if (isalpha()) {
        while (peak().has_value() && isalnum()) {
          buff += peak().value();
          consume();
        }
      } // Numbers without sign
      else if (isnum()) {
        while (peak().has_value() && (isnum() or peak().value() == '.')) {
          buff += peak().value();
          consume();
        }
      } // Numbers with sign
      else if (issign()) {
        buff += peak().value();
        consume();
        while (peak().has_value() && (isnum() or peak().value() == '.')) {
          buff += peak().value();
          consume();
        }
      } else {
        consume();
      }

      if (buff != "")
        tokens.push_back(Token(buff));

      buff = "";
    }

    return tokens;
  }

private:
  const std::string m_source;
  size_t m_index = 0;

  std::optional<char> peak() {
    if (m_index >= this->m_source.length()) {
      return {};
    }

    return this->m_source.at(m_index);
  }

  void consume() { this->m_index++; }

  bool isalpha() {
    return (bool)std::isalpha(static_cast<unsigned char>(peak().value()));
  }

  bool isnum() {
    return (bool)std::isdigit(static_cast<unsigned char>(peak().value()));
  }

  bool isalnum() {
    return (bool)std::isalnum(static_cast<unsigned char>(peak().value()));
  }

  bool issign() { return peak().value() == '-' ? true : false; }

  bool isallowed() { return this->issign() || this->isalnum(); }
};
