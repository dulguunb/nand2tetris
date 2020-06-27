#include "Parser.h"
Parser::Parser(string filename){
  program.open(filename);
  lineCnt = 0;
  for(string line; getline(program,line);){
    auto iter=line.begin();
    if(*iter != '/' && *(iter++) != '/'){
      cout << line << endl;
      rawProgram.push_back(line);
    }
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
  lineCnt++;
  if(hasMoreCommands()){
    currentLine = *(rawProgram.begin()+lineCnt);
  }
}
CommandType Parser::commandType(){
  CommandType result;
  string keyword="";
  for(string::iterator iter=currentLine.begin();
    iter!=currentLine.end();iter++){
    keyword+=*iter;
    if(keyword == "add" || keyword == "sub" || keyword == "eq" || keyword == "lt"
    || keyword == "gt" || keyword == "not" || keyword == "and" || keyword == "or" || keyword == "neg"){
      cout << "CommandType::C_ARITHMETIC" << endl;
      return CommandType::C_ARITHMETIC;
    }
    if(keyword == "push"){
      cout << "CommandType::C_PUSH" << endl;
      return CommandType::C_PUSH;
    }
    if(keyword == "pop"){
      cout << "CommandType::C_POP" << endl;
      return CommandType::C_POP;
    }
  }
  return result;
}
vector<string> Parser::tokenize(){
  vector<string> tokens;
  string keyword="";
  for(string::iterator iter=currentLine.begin();
      iter!=currentLine.end();iter++){
    if(isspace(*iter)){
      tokens.push_back(keyword);
      keyword="";
    }
    else {
     keyword +=*iter;
    }
  }
  tokens.push_back(keyword);
  return tokens;
}
string Parser::arg1(){
  CommandType type = commandType();
  auto tokens = tokenize();
  string result = "";
  if (type == CommandType::C_ARITHMETIC){
    result = *(tokens.begin());
  }
  else{
    cout << "result arg1: " << *(tokens.begin()) << endl;
    result = *(tokens.begin()+1);
  }
  return result;
}
int Parser::arg2(){
  CommandType type = commandType();
  auto tokens = tokenize();
  int result = -1;
  if ( type == CommandType::C_PUSH ||
       type == CommandType::C_POP ||
       type == CommandType::C_FUNCTION ||
       type == CommandType::C_CALL ){
    cout <<"token.end(): " << *(tokens.begin() + 2) << endl;
    result = stoi(*(tokens.begin() + 2));
    cout << "result: " << result << endl;
  }
  return result;
}
Parser::~Parser(){
}