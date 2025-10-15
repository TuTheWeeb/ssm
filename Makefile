FLAGS=-g -Wall -Werror -std=c++23

HEADERS=Tokenizer.hpp Token.hpp

main: main.cpp $(HEADERS)
	clang++ main.cpp $(FLAGS) -o main
