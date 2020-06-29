COMPILER=g++
VMTranslator: vmTranslator.cc Parser.o CodeWriter.o
	${COMPILER} -std=c++11 -lstdc++fs -Ddebug7 -g vmTranslator.cc Parser.o CodeWriter.o -o VMTranslator 
	chmod +x VMTranslator
Parser.o: Parser.cc
	${COMPILER} -std=c++11 -lstdc++fs -Ddebug7  -c Parser.cc -o Parser.o
CodeWriter.o: CodeWriter.cc
	${COMPILER} -std=c++11 -lstdc++fs -Ddebug7 -c CodeWriter.cc -o CodeWriter.o
clean:
	rm *.o VMTranslator
