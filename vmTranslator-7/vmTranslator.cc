#include <iostream>
#include "Parser.h"
using namespace std;
int main(int argc,char *argv[]){
  Parser parser(argv[1]);
  while(parser.hasMoreCommands()){
    cout << parser.getCurrentLine() << endl;
    cout << "arg1: " << parser.arg1() << endl;
    cout << "arg2:" << parser.arg2() << endl;
    parser.advance();
  }
  return 0;
}