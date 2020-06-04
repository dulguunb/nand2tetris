#include "Parser.h"
using namespace std;
Parser::Parser(const string &rawProgram){
  this->rawProgram = rawProgram;
  currentLetter = this->rawProgram.begin();
}
bool Parser::hasMoreCommands(){
  return letterCnt != this->rawProgram.size() - 1;
}
Command Parser::commandType(){
    if(*currentLetter == '@'){
      return Command::A;
    }
    if(isdigit(*currentLetter) || *currentLetter == '!' || *currentLetter == '-' || *currentLetter == 'D' || *currentLetter == 'A' || *currentLetter == 'M')
    {
      return Command::C;
    }
    if(*currentLetter == '('){
      return Command::L;
    }
    if(*currentLetter == '/' && *(currentLetter++) == '/'){
      return Command::Comment;
    }
    if(isspace(*currentLetter)){
      return Command::Space;
    }
}
void Parser::skipComment(){
  if(commandType() == Command::Comment){
    string result = "";
    while(*currentLetter != '\n'){
      result+=*currentLetter;
      currentLetter++;
    }
    cout << "comment: " << result << endl;
  }
}
string Parser::symbol(){
  auto currentCommandType = commandType();
  if(currentCommandType == Command::A){
      string result = "";
      // result+=*currentLetter;
      advance();
      while(isdigit(*currentLetter) && *currentLetter != '\n'){ // constant
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
    while(*currentLetter != ';' && *currentLetter != '\n' && *currentLetter != EOF){
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
    string result;
    while(*currentLetter != '\n' && *currentLetter != EOF){
      result+=*currentLetter;
      advance();
    }
    advance();
    return result;
  }
  return "";
}
void Parser::advance(){
  if (hasMoreCommands()){
    letterCnt++;
    currentLetter++;
  }
}
