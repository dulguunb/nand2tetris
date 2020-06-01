#include "Parser.h"
using namespace std;
Parser::Parser(string rawProgram):rawProgram(rawProgram){}
void Parser::scan(){
  auto letter = rawProgram.begin();
  while(isspace(*letter)){
      auto lastChar = letter;
      // cout << "lastChar: " << lastChar << endl;
      if(isdigit(*lastChar)){ //
        auto constantAndSymbols = lastChar;
        // while(isalnum(*(lastChar = letter++))){
        //   *constantAndSymbols+=*lastChar;
        // }
        
        // cout << "constantAndSymbols: " << constantAndSymbols << endl;
      }
    }
}
