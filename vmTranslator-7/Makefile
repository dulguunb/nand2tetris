COMPILER=g++
vmTranslator.o: vmTranslator.cc Parser.o CodeWriter.o
	${COMPILER} -g vmTranslator.cc Parser.o CodeWriter.o -o vmTranslator
Parser.o: Parser.cc
	${COMPILER} -c Parser.cc -o Parser.o
CodeWriter.o: CodeWriter.cc
	${COMPILER} -c CodeWriter.cc -o CodeWriter.o
clean:
	rm *.o