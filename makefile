all : main 

main : src/parser/parser.y src/parser/parser.l src/buffer/Block.cpp src/parser/stmt.cpp src/buffer/BufferManager.cpp src/buffer/File.cpp src/catalog/catalog.cpp src/evaluator/evaluator.cpp src/evaluator/main.cpp src/index/IndexManager.cpp src/record/RecordManager.cpp src/record/record.cpp parser.tab.c lex.yy.c
	bison -d src/parser/parser.y
	flex src/parser/parser.l
	clang++ -std=c++11 -g -x c++ src/parser/stmt.cpp src/buffer/Block.cpp src/buffer/BufferManager.cpp src/buffer/File.cpp src/catalog/catalog.cpp src/evaluator/evaluator.cpp src/evaluator/main.cpp src/index/IndexManager.cpp src/record/RecordManager.cpp src/record/record.cpp parser.tab.c lex.yy.c -o main -lfl -lreadline -luuid

clean:
	rm -rf parser.tab.c parser.tab.h lex.yy.c a.out* main* *.o data
