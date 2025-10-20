CXX=clang++

FLAGS=-g -Wall -Werror -std=c++23

HEADERS=Tokenizer.hpp Token.hpp Assembler.hpp Virtualmachine.hpp

main: main.cpp $(HEADERS)
	 $(CXX) main.cpp $(FLAGS) -o main
