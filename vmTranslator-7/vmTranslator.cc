#include <iostream>
#include "Parser.h"
#include "CodeWriter.h"
using namespace std;
int main(int argc,char *argv[]){
  Parser parser(argv[1]);
  string outputFileName = string(argv[1])+".asm";
  CodeWriter code(outputFileName);
  while(parser.hasMoreCommands()){
    string arg1 = parser.arg1();
    int arg2 = parser.arg2();
    cout << "arg1:" << parser.arg1() << endl;
    cout << "arg2:"  << parser.arg2() << endl;
    code.WritePushPop(CommandType::C_POP,arg1,arg2);
    parser.advance();
  }
  return 0;
}