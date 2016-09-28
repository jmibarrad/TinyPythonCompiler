
all: py

py: py_ast.o py_parser.o py_lexer.o
	g++ -std=c++11 -o $@ $^

ast.o: ast.cpp ast.h
	g++ -std=c++11 -c -o $@ $<

py_lexer.cpp: lexer.l ast.h
	flex -o $@ $<

py_parser.cpp: grammar.y ast.h
	bison --defines=tokens.h -o $@ $<

py_ast.o: ast.cpp
	g++ -std=c++11 -c -o $@ $<

py_lexer.o: py_lexer.cpp
	g++ -std=c++11 -c -o $@ $<

py_parser.o: py_parser.cpp
	g++ -c -o $@ $<

clean:
	rm -f *.o
	rm -f py_lexer.cpp
	rm -f py_parser.cpp
	rm -f tokens.h
	rm -f py

