COMPILER=g++
debuglevel=8
FLAG=-std=c++11 -lstdc++fs -Ddebug${debuglevel} -g 
VMTranslator: vmTranslator.cc Parser.o CodeWriter.o
	${COMPILER} ${FLAG} vmTranslator.cc Parser.o CodeWriter.o -o VMTranslator 
	chmod +x VMTranslator
Parser.o: Parser.cc
	${COMPILER} ${FLAG}  -c Parser.cc -o Parser.o
CodeWriter.o: CodeWriter.cc
	${COMPILER} ${FLAG} -c CodeWriter.cc -o CodeWriter.o
clean:
	rm *.o VMTranslator
