COMPILER=g++
main.o: Parser.o Code.o
	${COMPILER} -g main.cc Parser.o Code.o -o main.o
Parser.o: Parser.cc
	${COMPILER} -c Parser.cc -o Parser.o
Code.o: Code.cc
	${COMPILER} -c Code.cc -o Code.o
clean:
	rm main.o Parser.o Code.o