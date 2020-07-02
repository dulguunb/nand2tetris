#include "Parser.h"
Parser::Parser(string filename){
  program.open(filename);
  lineCnt = 0;
  for(string line; getline(program,line);){
    auto iter=line.begin();
    if(*iter != '/' && *(iter++) != '/'){
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
    if(keyword == "add" || keyword == "sub" || keyword == "eq"
    || keyword == "lt" || keyword == "gt" || keyword == "not"
    || keyword == "and" || keyword == "or"|| keyword == "neg"){
      #ifdef debug7
      cout << "CommandType::C_ARITHMETIC" << endl;
      #endif
      return CommandType::C_ARITHMETIC;
    }
    if(keyword == "push"){
      #ifdef debug7
      cout << "CommandType::C_PUSH" << endl;
      #endif
      return CommandType::C_PUSH;
    }
    if(keyword == "pop"){
      #ifdef debug7
      cout << "CommandType::C_POP" << endl;
      #endif
      return CommandType::C_POP;
    }
    if(keyword == "if-goto"){
      #ifdef debug7
      cout << "CommandType::IF" << endl;
      #endif
      return CommandType::C_IF;
    }
    if (keyword == "goto"){
      #ifdef debug7
      cout << "CommandType::C_GOTO" << endl;
      #endif
      return CommandType::C_GOTO;
    }
    if(keyword == "label"){
      #ifdef debug7
      cout << "CommandType::C_LABEL" << endl;
      #endif
      return CommandType::C_LABEL;
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
  if (type == CommandType::C_ARITHMETIC ){
    result = *(tokens.begin());
  }
  else if (type == CommandType::C_LABEL
  || type == CommandType::C_GOTO ||
     type == CommandType::C_IF){
    #ifdef debug7
    cout << "result arg1: " << *(tokens.begin()) << endl;
    #endif
    result = *(tokens.begin()+1);
  }
  else{
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
    #ifdef debug7
    cout <<"token.end(): " << *(tokens.begin() + 2) << endl;
    cout <<"token.end()-1: " << *(tokens.begin() + 1) << endl;
    #endif
    result = stoi(*(tokens.begin() + 2));
    #ifdef debug7
    cout << "result: " << result << endl;
    #endif
  }
  return result;
}
Parser::~Parser(){
}