COMPILER=g++
vmTranslator.o: vmTranslator.cc Parser.o
	${COMPILER} -g vmTranslator.cc Parser.o -o vmTranslator
Parser.o: Parser.cc
	${COMPILER} -c Parser.cc -o Parser.o
clean:
	rm *.o