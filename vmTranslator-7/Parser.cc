#include "Parser.h"
Parser::Parser(string filename){
  program.open(filename);
  lineCnt = 0;
  for(string line; getline(program,line);){
    rawProgram.push_back(line);
  }
  currentLine = *(rawProgram.begin() + lineCnt);
}
string Parser::getCurrentLine(){
  return currentLine;
}
bool Parser::hasMoreCommands(){
  return lineCnt < rawProgram.size();
}
void Parser::advance(){
  if(hasMoreCommands()){
    lineCnt++;
    currentLine = *(rawProgram.begin()+lineCnt);
  }
}
CommandType Parser::commandType(){
  CommandType result;
  string keyword="";
  for(string::iterator iter=currentLine.begin();iter!=currentLine.end();iter++){
    if(!isspace(*iter)){
      keyword +=*iter;
    }
    else{
      if(keyword == "push"){
        result = CommandType::C_PUSH;
      }
      else if(keyword == "pop"){
        result = CommandType::C_POP;
      }
      else if(keyword == "add" || keyword == "sub"){
        result = CommandType::C_ARITHMETIC;
      }
      tokens.push_back(keyword);
      keyword="";
    }
  }
  tokens.push_back(keyword);
  return result;
}
string Parser::arg1(){
  CommandType type = commandType();
  string result = *(tokens.end()-2);
  if (type == CommandType::C_ARITHMETIC){
    result = *(tokens.begin());
  }
  return result;
}
int Parser::arg2(){
  CommandType type = commandType();
  int result = -1;
  if (type == CommandType::C_PUSH ||
      type == CommandType::C_POP ||
      type == CommandType::C_FUNCTION ||
      type == CommandType::C_CALL){

    result = stoi(*(tokens.end() - 1));
  }
  return result;
}
Parser::~Parser(){
}