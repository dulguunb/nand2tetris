#include <iostream>
#include "Parser.h"
#include "CodeWriter.h"
using namespace std;
int main(int argc,char *argv[]){
  Parser parser(argv[1]);
  string outputFileName = string(argv[1]);
  CodeWriter code(outputFileName);

  while(parser.hasMoreCommands()){
    string arg1 = parser.arg1();
    int arg2 = parser.arg2();
    #ifdef debug7
    cout << "arg1:" << parser.arg1() << endl;
    cout << "arg2:"  << parser.arg2() << endl;
    #endif
    if(parser.commandType() == CommandType::C_ARITHMETIC){
      code.writeArithmetic(arg1);
    }
    else if(parser.commandType() == CommandType::C_POP ||
       parser.commandType() == CommandType::C_PUSH)
    {
      code.WritePushPop(parser.commandType(),arg1,arg2);
    }
    else if(parser.commandType() == CommandType::C_IF
     || parser.commandType() == CommandType::C_GOTO
     || parser.commandType() == CommandType::C_LABEL)
     {
       code.WriteBranching(parser.commandType(),arg1);
     }
    parser.advance();
  }
  return 0;
}