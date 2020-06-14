#include "CodeWriter.h"
CodeWriter::CodeWriter(string filename){
  baseAddresses["LCL"] = 1015;
  baseAddresses["ARG"] = 2015;
  baseAddresses["THIS"] = 3015;
  baseAddresses["THAT"] = 4015;
  baseAddresses["TMP"] = 5;
  baseAddresses["sp"] = 256;
  segmentConverter["local"] = "LCL";
  segmentConverter["argument"] = "ARG";
  segmentConverter["this"] = "THIS";
  segmentConverter["that"] = "THAT";
  segmentConverter["temp"] = "TMP";
  staticVariableName=filename;
  assemblyFile.open(filename+".asm");
  string init="";
  init+="@R0\n";
  init+="M="+to_string(baseAddresses["sp"])+"\n";
  init+="@R1\n";
  init+="M="+to_string(baseAddresses["LCL"])+"\n";
  init+="@R2\n";
  init+="M="+to_string(baseAddresses["ARG"])+"\n";
  init+="@R3\n";
  init+="M="+to_string(baseAddresses["THIS"])+"\n";
  init+="@R4\n";
  init+="M="+to_string(baseAddresses["THAT"])+"\n";
  cout << init << endl;
  assemblyFile << init;

}
CodeWriter::~CodeWriter(){
  assemblyFile.close();
}
void CodeWriter::writeArithmetic(string argument){
  if (argument == "add"){
    string assembly="";
    assembly+="@"+to_string(sp-1)+"\n";
    assembly+="D=M\n";
    assembly+="@"+to_string(sp)+"\n";
    assembly+="M=M+D\n";
    assemblyFile << assembly;
  }
  if (argument == "sub"){
    string assembly="";
    assembly+="@"+to_string(sp-1)+"\n";
    assembly+="D=M\n";
    assembly+="@"+to_string(sp)+"\n";
    assembly+="M=M-D\n";
  }
  if (argument == "gt"){
    string assembly = "";
    assembly+="@"+to_string(sp-1)+"\n";
    assembly+="D=M\n";
    assembly+="@"+to_string(sp)+"\n";
    assembly+="D=M-D\n";
    // "@"+to_string(sp-1) + "\n";
    // "D;JGT"
    // int a = stoi(stack.back());
    // stack.pop_back();
    // int b = stoi(stack.back());
    // stack.pop_back();
    // string _return = "false";
    // if (a>b){
    //     _return = "true";
    // }
    // stack.push_back(_return);
  }
  if (argument == "lt"){
    // int a = stoi(stack.back());
    // stack.pop_back();
    // int b = stoi(stack.back());
    // stack.pop_back();
    // string _return = "false";
    // if (a<b){
    //     _return = "true";
    // }
    // stack.push_back(_return);
  }
  sp--;
  // init+="@R0\n";
  // init+="M="+to_string(sp)+"\n";
  // init+="@"+to_string(sp)+"\n";
  // init+="M="+stack.back();
  // assemblyFile << init;
  
}
void CodeWriter::WritePushPop(CommandType type,string segment,int index){
  string assembly="";
  
  if(type == CommandType::C_POP
  && 
  (segment == "local" || segment == "argument" || 
   segment == "this" || segment == "that" || segment == "temp")){
    int offset = baseAddresses[segmentConverter[segment]] + index;
    sp--;
    assembly+="// POP operation " + segment + " " + to_string(index) + "\n";
    assembly+="@R0\n";
    assembly+="A=M-1\nD=M\n"; // go to the address where sp-1 points at and save it
    assembly+="@"+to_string(offset)+"\n";
    assembly+="M=D\n";
    assembly+="@R0\n";
    assembly+="M=M-1\n";
  }
  if(type == CommandType::C_PUSH
  &&
  (segment == "local" || segment == "argument" || 
  segment == "this"  || segment == "that" || segment == "temp")
  ){
    int offset =   baseAddresses[segmentConverter[segment]] + index;
    assembly+="// PUSH operation " + segment + " " + to_string(index) + "\n";
    assembly+="@"+to_string(offset)+"\n";
    assembly+="D=M\n";
    assembly+="@R0\n";
    assembly+="A=M\nM=D\n"; // push the value to the stack
    assembly+="@R0\n";
    assembly+="M=M+1\n";
  }
  if(type == CommandType::C_PUSH && segment == "constant"){
    assembly+="// PUSH CONSTANT " + to_string(index) + "\n";
    assembly+="@R0\n";
    assembly+="A=M\nM="+to_string(index) + "\n";
    assembly+="@R0\n";
    assembly+="M=M+1\n";
    sp++;
  }
  if (type == CommandType::C_POP && segment == "static"){
    string currentStaticVariable=staticVariableName+"."+to_string(index) + "\n";
    assembly+="// Popping an element from stack\n";
    assembly+="@R0\n";
    assembly+="A=M-1\nD=M\n";
    assembly+="//static variable " + currentStaticVariable + " is now created\n";
    assembly+="@"+currentStaticVariable+"\n";
    assembly+="M=D\n"
    assembly+="@R0\n";
    assembly+="M=M-1\n";
  }
  if (type == CommandType::C_PUSH && segment == "static"){
    string currentStaticVariable=staticVariableName+"."+to_string(index) + "\n";
    assembly+="// Pushing an element from stack\n";
    
    assembly+="//static variable " + currentStaticVariable + " is now created\n";
    assembly+="@"+currentStaticVariable+"\n";
    assembly+="D=M\n";
    assembly+="@R0\n";
    assembly+="A=M\nM=D\n";
    assembly+="// incrementing stack pointer\n";
    assembly+="@R0\n";
    assembly+="M=M+1\n";
  }
  if (type == CommandType::C_PUSH && segment == "pointer"){
    string pointer;
    if(index == 0){
      // this pointer
      pointer = baseAddresses["THIS"];
    }
    else if(index == 1){
      // that pointer
      pointer = baseAddresses["THAT"];
    }
    else{
      cout << "ERROR: This/That pointer only needs 0/1 index" << endl;
    }
    assembly+="// pop pointer\n";
    assembly+="@R0\n";
    assembly+="A=M-1\nD=M\n";
    assembly="@"+pointer+"\n";
    assembly+="M=D\n";
    assembly+="@R0\n";
    assembly+="M=M-1\n";
  }
  if (type == CommandType::C_PUSH && segment == "pointer"){
    string pointer;
    if(index == 0){
      // this pointer
      pointer = baseAddresses["THIS"];
    }
    else if(index == 1){
      // that pointer
      pointer = baseAddresses["THAT"];
    }
    else{
      cout << "ERROR: This/That pointer only needs 0/1 index" << endl;
    }
    assembly+="// push pointer\n";
    assembly="@"+pointer+"\n";
    assembly+="D=M\n";
    assembly+="@R0\n";
    assembly+="A=M\nM=D\n";
    assembly+="@R0\n";
    assembly+="M=M+1\n";
  }
  assemblyFile << assembly;
}