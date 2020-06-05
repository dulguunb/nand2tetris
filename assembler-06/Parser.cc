#include "Parser.h"
using namespace std;
Parser::Parser(string rawProgram){
  this->rawProgram = rawProgram;
  currentLetter = this->rawProgram.begin();
  cout << "Constructor of Parser: " << endl;
  cout << rawProgram << endl;
}
bool Parser::hasMoreCommands(){
  return letterCnt != this->rawProgram.size() - 1;
}
Command Parser::commandType(){
    if(this->rawProgram[letterCnt] == '@'){
      return Command::A;
    }
    if(isdigit(this->rawProgram[letterCnt]) || this->rawProgram[letterCnt] == '!' || this->rawProgram[letterCnt] == '-' || this->rawProgram[letterCnt] == 'D' || this->rawProgram[letterCnt] == 'A' || this->rawProgram[letterCnt] == 'M')
    {
      return Command::C;
    }
    if(this->rawProgram[letterCnt] == '('){
      return Command::L;
    }
    if(this->rawProgram[letterCnt] == '/' && this->rawProgram[letterCnt++] == '/'){
      return Command::Comment;
    }
    if(isspace(this->rawProgram[letterCnt]) || this->rawProgram[letterCnt] == '\n'){
      return Command::Space;
    }
}
void Parser::skipComment(){
  if(commandType() == Command::Comment){
    string result = "";
    while(this->rawProgram[letterCnt] != '\n' && hasMoreCommands()){
      result+=this->rawProgram[letterCnt];
      advance();
    }
    advance();
    cout << "comment: " << result << endl;
  }
}
string Parser::symbol(){
  auto currentCommandType = commandType();
  if(currentCommandType == Command::A){
      string result = "";
      // result+=this->rawProgram[letterCnt];
      advance();
      while(this->rawProgram[letterCnt] != '\n' && hasMoreCommands()){ // constant
        result+=this->rawProgram[letterCnt];
        // cout << "hasMoreCommand: " << hasMoreCommands() << endl;
        advance();
      }
    return result;
  }
  if (currentCommandType == Command::L){
      string result = "";
      // result+=this->rawProgram[letterCnt];
      advance();
      while(this->rawProgram[letterCnt] != ')' && hasMoreCommands()){ // constant
        result+=this->rawProgram[letterCnt];
        advance();
      }
  }
  return "";
}
string Parser::dest(){
  if (commandType() == Command::C){
    string result = "";
    while(this->rawProgram[letterCnt] != '=' && hasMoreCommands()){
      result+=this->rawProgram[letterCnt];
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
    while(this->rawProgram[letterCnt] != ';' && this->rawProgram[letterCnt] != '\n' && this->rawProgram[letterCnt] != EOF && hasMoreCommands()){
      result+=this->rawProgram[letterCnt];
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
    while(this->rawProgram[letterCnt] != '\n' && this->rawProgram[letterCnt] != EOF
    && hasMoreCommands()){
      result+=this->rawProgram[letterCnt];
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
  }
}
