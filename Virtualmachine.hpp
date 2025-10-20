#pragma once

#include "bytecode.hpp"
#include <array>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

struct stack_val {
  std::uint8_t _type = 0;
  std::uint64_t value = 0;
};

class SSMVm {
public:
  SSMVm(std::string file_path) {
    read_entire_binary(file_path);
    // run();
    for (auto st : stack) {
      std::cout << st.value << std::endl;
    }
  }

private:
  std::size_t sp = 1;
  std::array<stack_val, 5> stack;
  stack_val A = {0};
  stack_val B = {0};

  std::vector<bytecode> bin;

  // Runing
  void run() {
    for (auto byt : bin) {
      std::uint8_t inst = byt.inst.to_ulong();
      switch (inst) {
      case 1:
        push(byt);
        break;
      case 2:
        pop(byt);
        break;
      case 3:
        break;
      case 4:
        break;
      default:
        throw std::runtime_error("ERROR: INSTRUCTION DOES NOT EXISTS!");
        break;
      }
    }
  }

  // Instructions
  void push(bytecode byt) {
    if (sp == stack.size()) {
      throw std::runtime_error("ERROR: stack overflow!");
    }

    stack[sp] = {._type = static_cast<std::uint8_t>(byt.addressing.to_ulong()),
                 .value = byt.operand.to_ullong()};
    sp += 1;
  }

  void pop(bytecode byt) {
    if (sp == 0) {
      throw std::runtime_error("ERROR: stack pointer already at the base");
    }
    B = stack[sp];
    sp -= 1;
  }

  void add() {
    if (sp - 2 < 0 or sp == stack.size()) {
      throw std::runtime_error("ERROR: wrongfull acess trying to add without 2 "
                               "values on stack or stack is full, sp: " +
                               std::to_string(sp));
    }
    /*
    stack_val val1 = stack[sp - 2];
    stack_val val2 = stack[sp - 1];
    if (val1._type == 3 and val2._type == 1) {
      float temp = convert<float>(val2.value);
      temp += static_cast<std::int32_t>(val2.value);
      std::uint64_t ret = convert<std::uint64_t>(temp);
    }*/
  }
  template <typename T, typename U> T convert(U val2) {
    T val1;
    std::memcpy(&val1, &val2, sizeof(val1));
    return val1;
  }

  // Reading
  void read_entire_binary(const std::string file_path) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);

    // get file size
    std::uintmax_t size = std::filesystem::file_size(file_path);
    file.seekg(0);

    // Alocate space and read the entire buffer
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
      throw std::runtime_error("ERROR while reading file: " + file_path);
    }

    std::size_t i = 0;

    // read the binary into bytecode
    while (i < size) {
      bytecode byt;
      byt.inst = static_cast<std::uint8_t>(buffer.at(i));
      byt.addressing = static_cast<std::uint8_t>(buffer.at(i + 1));
      i += 8;
      // std::memcpy(&byt.operand, &buffer[i + 2], sizeof(std::uint64_t));
      bin.push_back(byt);
      std::cout << "Bytecode: " << byt.inst << " " << byt.addressing << " "
                << byt.operand << std::endl;
    }
  }
};
