#include "Parser.h"
Parser::Parser(string filename){
  string extension="";
  string newFileDirectory = "";
  string fName="";
  string parentDirectory = "";
  vector<string> fileNames;
  lineCnt = 0;
  auto iter = filename.begin();
  string lastParentDirectory = "";
  if(filename.find(".vm") != string::npos){ // singlefile
    #ifdef debug8
    cout << "single file\n";
    #endif
    fileNames.push_back(filename);
  }
  else { // directory
    string vmExtension = ".vm";
    for(auto& p: fs::directory_iterator(filename)){
      string path = p.path();
      if(path.find(vmExtension) != string::npos){
        fileNames.push_back(path);
      }
    }
  }
  #ifdef debug8
  cout << "Number of files: " << fileNames.size() << endl;
  #endif
  for(auto iterFileNames=fileNames.begin();
    iterFileNames!=fileNames.end(); iterFileNames++){
    string newfilename = *iterFileNames;
    ifstream program;
    program.open(newfilename);
    #ifdef debug8
    cout << "iterfilename: " << newfilename << endl;
    #endif
    for(string line; getline(program,line);){
      auto iter = line.begin();
      if(*iter != '/' && *(iter++) != '/'){
        rawProgram.push_back(line);
      }
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
  for(auto iter=currentLine.begin();
    iter!=currentLine.end();iter++){
    keyword+=*iter;
    if(keyword == "add" || keyword == "sub" || keyword == "eq"
    || keyword == "lt" || keyword == "gt" || keyword == "not"
    || keyword == "and" || keyword == "or"|| keyword == "neg"){
      return CommandType::C_ARITHMETIC;
    }
    if(keyword == "push"){
      return CommandType::C_PUSH;
    }
    if(keyword == "pop"){
      return CommandType::C_POP;
    }
    if(keyword == "if-goto"){
      return CommandType::C_IF;
    }
    if (keyword == "goto"){
      return CommandType::C_GOTO;
    }
    if(keyword == "label"){
      return CommandType::C_LABEL;
    }
    if(keyword == "function"){
      return CommandType::C_FUNCTION;
    }
    if(keyword == "return"){
      return CommandType::C_RETURN;
    }
    if(keyword == "call"){
      return CommandType::C_CALL;
    }
  }
  return result;
}
vector<string> Parser::tokenize(){
  vector<string> tokens;
  string keyword="";
  for(auto iter=currentLine.begin();
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
    #ifdef debug7
    cout << "result arg1: " << *(tokens.begin()) << endl;
    #endif
    result = *(tokens.begin());
  }
  else if (
    type == CommandType::C_LABEL ||
    type == CommandType::C_GOTO ||
    type == CommandType::C_IF ||
    type == CommandType::C_FUNCTION ||
    type == CommandType::C_CALL
    ){
    #ifdef debug7
    cout << "result arg1: " << *(tokens.begin()+1) << endl;
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
    result = stoi(*(tokens.begin() + 2));
    #ifdef debug7
    cout << "result: " << result << endl;
    #endif
  }
  return result;
}
Parser::~Parser(){
}