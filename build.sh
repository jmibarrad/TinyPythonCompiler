bison --defines=tokens.h -o expr_parser.cpp grammar.y
flex -o expr_lexer.cpp lexer.l
g++ -std=c++11 -c -o ast.o ast.cpp
g++ -std=c++11 -c -o expr_lexer.o expr_lexer.cpp
g++ -std=c++11 -c -o expr_parser.o expr_parser.cpp

g++ -std=c++11 -o expr expr_lexer.o expr_parser.o ast.o