COMPILER=g++
main.o: Parser.o Code.o SymbolTable.o
	${COMPILER} -g main.cc Parser.o Code.o SymbolTable.o -o main.o
Parser.o: Parser.cc
	${COMPILER} -c Parser.cc -o Parser.o
Code.o: Code.cc
	${COMPILER} -c Code.cc -o Code.o
SymbolTable.o: SymbolTable.cc
	${COMPILER} -c SymbolTable.cc -o SymbolTable.o
clean:
	rm main.o Parser.o Code.o SymbolTable.o