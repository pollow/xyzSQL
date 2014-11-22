all : main 
main  : ./repl/repl.cpp ./repl/repl.h ./parser/parser.y ./parser/parser.l ./parser/stmt.h ./catalog/catalog.cpp ./catalog/catalog.h
	bison -d ./parser/parser.y
	flex ./parser/parser.l
	c++ -c -x c++ parser.tab.c lex.yy.c -ll -g -std=c++11
	c++ ./repl/repl.cpp ./catalog/catalog.cpp parser.tab.o lex.yy.o -std=c++11 -lreadline -ll -o main -g

clean:
	rm -rf parser.tab.c parser.tab.h lex.yy.c a.out* main* *.o data
