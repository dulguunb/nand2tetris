COMPILER=g++
debuglevel=8
VMTranslator: vmTranslator.cc Parser.o CodeWriter.o
	${COMPILER} -std=c++11 -lstdc++fs -Ddebug${debuglevel} -g vmTranslator.cc Parser.o CodeWriter.o -o VMTranslator 
	chmod +x VMTranslator
Parser.o: Parser.cc
	${COMPILER} -std=c++11 -lstdc++fs -Ddebug${debuglevel}  -c Parser.cc -o Parser.o
CodeWriter.o: CodeWriter.cc
	${COMPILER} -std=c++11 -lstdc++fs -Ddebug${debuglevel} -c CodeWriter.cc -o CodeWriter.o
clean:
	rm *.o VMTranslator
