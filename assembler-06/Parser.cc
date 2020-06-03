#include "Parser.h"
using namespace std;
Parser::Parser(const string &rawProgram){
  this->rawProgram = rawProgram;
  currentLetter = this->rawProgram.begin();
}
bool Parser::hasMoreCommands(){
  return currentLetter != rawProgram.end();
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
    cout << "Command::Comment" << endl;
    while(*currentLetter != '\n'){
      currentLetter++;
      cout << *currentLetter;
    }
    cout << endl;
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
        cout << "symbol::Parser: " << result << endl;
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
      cout << result << endl;
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
      cout << "result::comp: " << result << endl;
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
      cout << "result::jump: " << result << endl;
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
    cout << "currentLetter: " << *currentLetter << endl;
    currentLetter++;
  }
}
