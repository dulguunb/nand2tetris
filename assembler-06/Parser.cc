#include "Parser.h"
using namespace std;
Parser::Parser(string rawProgram):rawProgram(rawProgram){
  currentLetter = rawProgram.begin();
}
void Parser::advance(){
  // auto letter = rawProgram.begin();
  // bool isEnd = false;
  // while(!isEnd){
  //   isEnd = (letter == rawProgram.end());
  //   // if(isalnum(*letter) || *letter == '_' || *letter == '.' || *letter == '$' || *letter == ':'){ //
  //   //   string constantAndSymbols = "";
  //   //   constantAndSymbols+=*letter;
  //   //   letter++;
  //   //   while(isalnum(*letter)){
  //   //     constantAndSymbols+=*letter;
  //   //     letter++;
  //   //   }
  //   //   cout << "constantAndSymbols: " << constantAndSymbols << endl;
  //   // }
  //   if(isupper(*letter)){ //mnemonics
  //     string mnemonics = "";
  //     mnemonics+=*letter;
  //     letter++;
  //     while(isupper(*letter))
  //     {

  //     }
  //   }
  //   if(*letter == '@'){
  //     string constant = "";
  //     constant+=*letter;
  //     letter++;
  //     while(isdigit(*letter)){ // constant
  //       constant+=*letter;
  //       letter++;
  //     }
  //     while(isalnum(*letter)){ //variable?

  //     }
  //     cout << "constant: " << constant << endl;
  //   }
  //   auto next = (letter++);
  //   if(*letter == '/' && *next == '/'){
  //     string comment = "";
  //     while(*letter != '\n'){
  //       comment+=*letter;
  //       letter++;
  //     }
  //     cout << "comment: " << comment << endl;
  //   }
  //   while (isspace(*letter))
  //     letter++;
  // }
}
