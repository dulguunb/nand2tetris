#include "Parser.h"
using namespace std;
Parser::Parser(string rawProgram):rawProgram(rawProgram){
  currentLetter = rawProgram.begin();
}
bool Parser::hasMoreCommands(){
  return currentLetter != rawProgram.end();
}
Command Parser::commandType(){
    if(*currentLetter == '@'){
      return Command::A;
    }
    auto letter = currentLetter;
    if(isdigit(*letter) || *letter == '!' || *letter = '-' || isupper(*letter))
    {
      return Command::C;
    }
    if(*currentLetter == '('){
      return Command::L;
    }
}
string Parser::symbol(){
  auto currentCommandType = commandType();
  if(currentCommandType == Command::A){
      string result = "";
      // result+=*currentLetter;
      advance();
      while(isdigit(*currentLetter) || *currentLetter != '\n'){ // constant
        result+=*currentLetter;
        advance();
      }
    return result;
  }
  if (currentCommandType == Command::L){
      string result = "";
      // result+=*currentLetter;
      advance();
      while(*currentLetter != ')'){ // constant
        result+=*currentLetter;
        advance();
      }
  }
  return "";
}
string Parser::dest(){
  if (commandType() == Command::C){
    string result = "";
    while(*currentLetter != '='){
      result+=*currentLetter;
      advance();
    }
    advance();
    return result;
  }
  return "";
}
string Parser::comp(){
  if(commandType() == Command::C){
    string result;
    while(*currentLetter != ';' || *currentLetter != '\n'){
      result+=*currentLetter;
      advance();
    }
    advance();
    return result;
  }
  return "";
}
string Parser::jump(){
  if (commandType() == Command::C){

  }
}
void Parser::advance(){
  if (hasMoreCommands()){
    currentLetter++;
  }
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
