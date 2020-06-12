#include "Parser.h"
Parser::Parser(string filename){
  program.open(filename);
  lineCnt = 0;
  string line;
  while(getline(program,line)){
    rawProgram.push_back(line);
  }
  currentLine = rawProgram[lineCnt];
}
string Parser::getCurrentLine(){
  return currentLine;
}
bool Parser::hasMoreCommands(){
  return lineCnt < rawProgram.size();
}
void Parser::advance(){
  if(hasMoreCommands()){
      currentLine = rawProgram[lineCnt];
      lineCnt++;
  }
}
CommandType Parser::commandType(){
  CommandType result;
  string keyword="";
  for(string::iterator iter=currentLine.begin();iter!=currentLine.end();iter++){
    if(!isspace(*iter)){
      keyword +=* iter;
    }
    else{
      if(keyword == "push"){
        cout << lineCnt << ") C_PUSH" << endl;
        result = CommandType::C_PUSH;
      }
      else if(keyword == "pop"){
        cout << lineCnt << ") C_POP" << endl;
        result = CommandType::C_POP;
      }
      else if(keyword == "add" || keyword == "sub"){
        cout << lineCnt << ") add|sub" << endl;
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
  string result = "";
  CommandType type = commandType();
  if (type == CommandType::C_ARITHMETIC){
    result = tokens[0];
  }
  return result;
}
int Parser::arg2(){
  CommandType type = commandType();
  int result = -1;
  if (type == CommandType::C_PUSH || type == CommandType::C_POP || type == CommandType::C_FUNCTION || type == CommandType::C_CALL){
    result = stoi(*(tokens.end()-1));
  }
  return result;
}
Parser::~Parser(){
}