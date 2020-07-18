COMPILER=g++
debuglevel=0
debuglevel2=0
FLAG=-std=c++11 -Ddebug${debuglevel} -Ddebug${debuglevel2} -g 
VMTranslator: vmTranslator.cc Parser.o CodeWriter.o
	${COMPILER} ${FLAG} vmTranslator.cc Parser.o CodeWriter.o -o VMTranslator -lstdc++fs 
	chmod +x VMTranslator
Parser.o: Parser.cc
	${COMPILER} ${FLAG} -c Parser.cc -o Parser.o -lstdc++fs
CodeWriter.o: CodeWriter.cc
	${COMPILER} ${FLAG} -c CodeWriter.cc -o CodeWriter.o -lstdc++fs
clean:
	rm *.o VMTranslator
