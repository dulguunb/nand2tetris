#include "CodeWriter.h"
CodeWriter::CodeWriter(string filename){
  segmentConverter["local"] = "R1";
  segmentConverter["argument"] = "R2";
  segmentConverter["this"] = "R3";
  segmentConverter["that"] = "R4";
  string newFileDirectory = "";
  staticVariableName="";
  string parentDirectory = "";
  auto iter = filename.begin();
  do{
    if (*iter != '.'){
      parentDirectory+=*iter;
      if ( *iter == '/'){
        staticVariableName="";
        newFileDirectory+=parentDirectory+"/";
        parentDirectory="";
      }
      else {
        staticVariableName+=*iter;
      }
      iter++;
    }
  }while(*iter != '.' && iter != filename.end());
  if(newFileDirectory == ""){
    // root directory
    assemblyFile.open(staticVariableName+".asm");
  }
  else{ 
    assemblyFile.open(newFileDirectory+"/"+staticVariableName+".asm");
  }
}
CodeWriter::~CodeWriter(){
  assemblyFile.close();
}
void CodeWriter::writeArithmetic(string argument){
  string assembly = "";
  map<string,string> arguments = {
    {"add","+"},{"sub","-"},{"neg","-"},
    {"not","!"},{"and","&"},{"or","|"},
    {"lt","JGE"},{"gt","JLE"},{"eq","JNE"}
    };
  if(argument == "neg" || argument == "not"){
    assembly+="// START of "+ argument +" opeartion on the stack\n";
    assembly+="@R0\n";
    assembly+="D=M-1\n"; // sp --
    assembly+="A=D\n";
    assembly+="M="+arguments[argument]+"M\n";
    assembly+="// End of "+ argument +" opeartion on the stack\n";
    assemblyFile << assembly;
    return;
  }
  assembly+="@R0\n";
  assembly+="M=M-1\n"; // sp --
  assembly+="A=M\n";
  assembly+="D=M\n"; // store it
  assembly+="A=A-1\n"; // go to (sp-2)
  if (argument == "add" || argument == "sub" ||  argument == "and" ||  argument == "or"){
    assembly+="// START OF "+ argument +" opeartion on the stack\n";
    assembly+="M=M"+arguments[argument]+"D\n";
    assembly+="// End OF "+ argument +" opeartion on the stack\n";
    assemblyFile << assembly;
    return;
  }
  if (argument == "eq" || argument == "gt" || argument == "lt"){
    assembly+="// START OF " + argument + " operation on the stack\n";
    assembly+="D=M-D\n";
    assembly+="@False_" + to_string(jumpCnt) + "\n";
    assembly+="D;"+arguments[argument]+"\n";
    assembly+="@R0\n";
    assembly+="A=M-1\n";
    assembly+="M=-1\n"; // setting it 1111111111111111
    assembly+="@Continue_" + to_string(jumpCnt) + "\n";
    assembly+="0;JMP\n";
    assembly+="(False_" + to_string(jumpCnt)+ ")\n";
    assembly+="@R0\n";
    assembly+="A=M-1\n";
    assembly+="M=0\n"; // setting it to 0000000000000000
    assembly+="(Continue_"+to_string(jumpCnt)+")\n";
    assembly+="// END OF " + argument + " operation on the stack\n";
    assemblyFile << assembly;
    jumpCnt++;
    return;
  }
  
}
void CodeWriter::WritePushPop(CommandType type,string segment,int index){
  string assembly="";
  if(type == CommandType::C_POP
  && 
  (segment == "local" || segment == "argument" || 
   segment == "this" || segment == "that")){
    assembly+="// POP operation " + segment + " " + to_string(index) + "\n";

    assembly+="@"+to_string(index)+"\nD=A\n";
    assembly+="@"+segmentConverter[segment]+"\n";
    assembly+="D=D+M\n@a\nM=D\n"; // addr=LCL+i

    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --

    assembly+="@R0\n";
    assembly+="A=M\nD=M\n"; // *addr=*SP
    assembly+="@a\nA=M\nM=D\n";
  }
  if(type == CommandType::C_PUSH
  &&
  (segment == "local" || segment == "argument" || 
  segment == "this"  || segment == "that")
  ){
    assembly+="// PUSH operation " + segment + " " + to_string(index) + "\n";
    assembly+="@"+to_string(index)+"\n";
    assembly+="D=A\n";
    assembly+="@"+segmentConverter[segment]+"\n";
    assembly+="D=D+M\n";
    assembly+="@a\n"; // addr = segment + i
    assembly+="M=D\n";
    assembly+="@a\nA=M\nD=M\n";
    assembly+="@R0\n";
    assembly+="A=M\nM=D\n"; // *SP=*addr
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp ++
  }
  if(type == CommandType::C_PUSH && segment == "temp"){
    assembly+="// PUSH operation " + segment + " " + to_string(index) + "\n";

    assembly+="@"+to_string(index)+"\nD=A\n";
    assembly+="@5\n";
    assembly+="D=D+A\n@a\nM=D\n"; // addr= 5 + i

    assembly+="@a\nA=M\nD=M\n";
    assembly+="@R0\n";
    assembly+="A=M\nM=D\n"; // *SP=*addr

    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp ++

  }
  if(type == CommandType::C_POP && segment == "temp"){
    assembly+="// POP operation " + segment + " " + to_string(index) + "\n";

    assembly+="@"+to_string(index)+"\nD=A\n";
    assembly+="@5\n";
    assembly+="D=D+A\n@a\nM=D\n"; // addr=5+i

    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --

    assembly+="@R0\n";
    assembly+="A=M\nD=M\n"; // *addr=*SP
    assembly+="@a\nA=M\nM=D\n";

  }
  if(type == CommandType::C_PUSH && segment == "constant"){
    assembly+="// PUSH CONSTANT " + to_string(index) + "\n";
    assembly+="@"+to_string(index)+"\n";
    assembly+="D=A\n";
    assembly+="@R0\n";
    assembly+="A=M\nM=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp ++

  }
  if(type == CommandType::C_POP && segment == "static"){
    string currentStaticVariable=staticVariableName+"."+to_string(index) + "\n";
    assembly+="// Popping an element from stack\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\n";
    assembly+="A=M-D\nD=M\n";
    assembly+="// static variable " + currentStaticVariable + "\n";
    assembly+="@"+currentStaticVariable+"\n";
    assembly+="M=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --

  }
  if(type == CommandType::C_PUSH && segment == "static"){
    string currentStaticVariable=staticVariableName+"."+to_string(index) + "\n";
    assembly+="// Pushing an element from stack\n";
    assembly+="// static variable " + currentStaticVariable+"\n";
    assembly+="@"+currentStaticVariable+"\n";
    assembly+="D=M\n";
    assembly+="@R0\n";
    assembly+="A=M\nM=D\n";
    assembly+="// incrementing stack pointer\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp ++

  }
  if(type == CommandType::C_POP && segment == "pointer"){
    string pointer = "";
    if(index == 0){
      // this pointer
      pointer = "@R3\n";
    }
    else if(index == 1){
      // that pointer
      pointer = "@R4\n";
    }
    else{
      #ifdef debug7
      cout << "ERROR: This/That pointer only needs 0/1 index" << endl;
      #endif
    }
    assembly+="// pop pointer\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\n";
    assembly+="M=M-D\nA=M\nD=M\n";
    assembly+=pointer;
    assembly+="M=D\n";

  }
  if (type == CommandType::C_PUSH && segment == "pointer"){
    string pointer = "";
    if(index == 0){
      // this pointer
      pointer = "@R3\n";
    }
    else if(index == 1){
      // that pointer
      pointer = "@R4\n";
    }
    else{
      #ifdef debug7
      cout << "ERROR: This/That pointer only needs 0/1 index" << endl;
      #endif
    }
    assembly+="// push pointer\n";
    assembly+=pointer;
    assembly+="D=M\n";
    assembly+="@R0\nA=M\nM=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp ++

  }
  assemblyFile << assembly;
}
void CodeWriter::WriteFunction(CommandType type,string arg1, int arg2){
  string assembly = "";
  if (type == CommandType::C_CALL){
    assembly+="@R2\n";
    "@R0\n";
    "D=M-1\n";
    "D=D-1\n";
    "A=D\n";
    "@R2\n";
    "M=D\n";
    "@R0\n";
    "A=M-1\n";
    "D=M\n";
     
    assembly+="@";
  }
}
void CodeWriter::WriteBranching(CommandType type,string argument){
  string assembly = "";
  if(type == CommandType::C_GOTO){
    assembly+="// CommandType::C_GOTO start\n";
    assembly+="@"+argument+"\n";
    assembly+="0;JMP\n";
    assembly+="// CommandType::C_GOTO end\n";
  }
  if(type == CommandType::C_IF){
    assembly+="// CommandType::C_IF start\n";
    assembly="@R0\n";
    assembly+="@R0\n";
    assembly+="M=M-1\n"; // sp --
    assembly+="A=M\n";
    assembly+="D=M\n"; // store it
    assembly+="@"+argument+"\n";
    assembly+="D;JNE\n"; // if it's 0 then it must be true
    // else just carry on with the execution
    assembly+="// CommandType::C_IF end\n";
  }
  if(type == CommandType::C_LABEL){
    assembly+="// CommandType::C_LABEL start\n";
    assembly+="("+argument+")\n";
    assembly+="// CommandType::C_LABEL end\n";
  }
  assemblyFile << assembly;
}