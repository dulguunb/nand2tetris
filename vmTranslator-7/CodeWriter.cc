#include "CodeWriter.h"
CodeWriter::CodeWriter(string filename){
  baseAddresses["LCL"] = 1015;
  baseAddresses["ARG"] = 2015;
  baseAddresses["THIS"] = 3015;
  baseAddresses["THAT"] = 4015;
  baseAddresses["sp"] = 256;
  assemblyFile.open(filename);
  string init="";
  init+="@R0\n";
  init+="M="+to_string(spBase)+"\n";
  init+="@R1\n";
  init+="M="+to_string(LCL)+"\n";
  cout << init << endl;
  assemblyFile << init;
}
CodeWriter::~CodeWriter(){
  assemblyFile.close();
}
void CodeWriter::writeArithmetic(string argument){
  if (argument == "add"){
    int a = stoi(stack.back());
    stack.pop_back();
    int b = stoi(stack.back());
    stack.pop_back();
    stack.push_back(to_string(b+a));
  }
  if (argument == "sub"){
    int a = stoi(stack.back());
    stack.pop_back();
    int b = stoi(stack.back());
    stack.pop_back();
    stack.push_back(to_string(b-a));
  }
  if (argument == "gt"){
    int a = stoi(stack.back());
    stack.pop_back();
    int b = stoi(stack.back());
    stack.pop_back();
    string _return = "false";
    if (a>b){
        _return = "true";
    }
    stack.push_back(_return);
  }
  if (argument == "lt"){
    int a = stoi(stack.back());
    stack.pop_back();
    int b = stoi(stack.back());
    stack.pop_back();
    string _return = "false";
    if (a<b){
        _return = "true";
    }
    stack.push_back(_return);
  }
  sp--;
  // init+="@R0\n";
  // init+="M="+to_string(sp)+"\n";
  // init+="@"+to_string(sp)+"\n";
  // init+="M="+stack.back();
  // assemblyFile << init;
}
void CodeWriter::WritePushPop(CommandType type,string segment,int index){
  if(type == CommandType::C_POP
  && 
  (segment == "local" || segment == "argument" || 
   segment == "this" || segment == "that")){
    int offset = baseAddress[segment] + index;
    string assembly = "";
    sp--;
    assembly+="@"+to_string(sp)+"\n";
    assembly+="D=M\n";
    assembly+="@"+to_string(offset)+"\n";
    assembly+="M=D\n";
    assembly+="@R0\n";
    assembly+="M="+to_string(sp)+"\n";
    assemblyFile << assembly;
  }
  if(type == CommandType::C_PUSH
  &&
  (segment == "local" || segment == "argument" || 
  segment == "this"  || segment == "that")
  ){
    int offset =  baseAddress[segment] + index;
    string assembly = "";
    assembly+="@R0\n";
    assembly+="M="+to_string(sp)+"\n";
    assembly+="@"+to_string(sp)+"\n";
    assembly+="D=M\n";
    assembly+="@"+to_string(offset)+"\n";
    assembly+="M=D\n";
    sp++;
    assembly+="@R0\n";
    assembly+="M="+to_string(sp)+"\n";
    assemblyFile << assembly;
  }
  if(type == CommandType::C_PUSH && segment == "constant"){
    string assembly = "";
    assembly+="@"+to_string(sp);
    assembly+="M="+to_string(index);
    sp++;
    assemblyFile << assembly;
  }
}