COMPILER=g++
main.o: Parser.o
	${COMPILER} -g main.cc Parser.o -o main.o
Parser.o: Parser.cc
	${COMPILER} -c Parser.cc -o Parser.o
clean:
	rm main.o Parser.o