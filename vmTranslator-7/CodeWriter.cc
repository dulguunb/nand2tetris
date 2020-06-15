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
  string assembly = "";
  if (argument == "add"){
    /*
         x
         y
    sp- >
    ========> add
          x+y
    sp -> 
    */
    assembly+="@R0\n";
    assembly+="M=A-1\nD=M\n"; // y
    assembly+="@R0\n";
    assembly+="M=A-2\n"; // x
    assembly+="M=M+D\n"; // x=x+y
    assembly+="@R0\nM=M-1\n"; // sp--
  }
   /*
         x
         y
    sp- >
    ========> add
          x-y
    sp -> 
    */
  if (argument == "sub"){
    assembly+="@R0\n";
    assembly+="M=A-1\nD=M\n"; // y
    assembly+="@R0\n";
    assembly+="M=A-2\n"; // x
    assembly+="M=M-D\n"; // x=x-y
    assembly+="@R0\nM=M-1\n"; // sp--
  }
  if(argument == "neg"){
    assembly+="@R0\n";
    assembly+="M=A-1\nM=0-M\n";
  }
  if (argument == "eq"){
    assembly+="@R0\n";
    assembly+="@M=A-1\nD=M\n"; // y
    assembly+="@R0\n";
    assembly+="D;JEQ\n"; // if D == 0
    assembly+="M=A-1\nM=1\n";//  set A-1 to M=1 -> True
    assembly+="@R0\n";
    assembly+="M=A-1\nM=0\n"; //  set A-1 to M=0 -> False
  }
  if (argument == "gt"){
    assembly+="@R0\n";
    assembly+="M=A-1\nD=M\n"; // y
    assembly+="@R0\n";
    assembly+="M=A-2\n"; // x
    assembly+="D=M-D\n"; // z=(y - x)
    assembly+="@fliptrue\n";
    assembly+="D;JGT\n"; // z>0 positive then set sp-2 to true and sp--
    assembly+="@R0\n";
    assembly+="M=A-2\nM=0\n"; // M=false
    assembly+="@R0\nM=M-1\n"; // sp --
    assembly+="0;JMP\n";// jump to the end
    assembly+="(fliptrue)\n";
    assembly+="@R0\n";
    assembly+="M=A-2\nM=1\n"; // M=true
  }
  if (argument == "lt"){
    assembly+="@R0\n";
    assembly+="M=A-1\nD=M\n"; // y
    assembly+="@R0\n";
    assembly+="M=A-2\n"; // x
    assembly+="D=M-D\n"; // z=(y - x)
    assembly+="@flipfalse\n";
    assembly+="D;JGT\n"; // z>0 positive then set sp-2 to true and sp--
    assembly+="@R0\n";
    assembly+="M=A-2\nM=1\n"; // M=true
    assembly+="@R0\nM=M-1\n"; // sp --
    assembly+="0;JMP\n";
    assembly+="(flipfalse)\n";
    assembly+="@R0\n";
    assembly+="M=A-2\nM=0\n"; // M=false
    assembly+="@R0\nM=M-1\n"; // sp --
  }
  if(argument == "and"){
    assembly+="@2\n";
    assembly+="D=A\n";
    assembly+="@R0\n";
    assembly+="M=A-1\nD=M-1\n"; // 2 - (y)
    assembly+="@R0\n";
    assembly+="M=A-2\n"; // x
    assembly+="@R0\nD=D-1\n"; // 2 - (x)
    assembly+="@fliptrue\n";
    assembly+="D;JEQ\n";
    assembly+="@R0\n";
    assembly+="@M=A-2\nM=0\n"; // M=false
    assembly+="0;JMP\n";
    assembly+="(fliptrue)\n"; // M = true
    assembly+="@R0\n";
    assembly+="M=A-2\nM=1\n"; // M=true
    assembly+="@R0\nM=M-1\n"; // sp --
  }
  if(argument == "or"){
    assembly+="@0\n";
    assembly+="D=A\n";
    assembly+="@R0\n";
    assembly+="M=A-1\nD=M-1\n"; // 0 - (y)
    assembly+="@R0\n";
    assembly+="M=A-2\n"; // x
    assembly+="@R0\nD=D-1\n"; // 0 - (x)
    assembly+="@fliptrue\n";
    assembly+="D;JLT\n";
    assembly+="@R0\n";
    assembly+="M=A-2\nM=0\n"; // M=false
    assembly+="@R0\nM=M-1\n"; // sp --
    assembly+="0;JMP\n";
    assembly+="(fliptrue)\n"; // M = true
    assembly+="@R0\n";
    assembly+="M=A-2\nM=1\n"; // M=true
    assembly+="@R0\nM=M-1\n"; // sp --
  }
  if(argument == "not"){
    assembly+="@R0\n";
    assembly+="M=A-2\nD=M\n"; // x
    assembly+="@fliptrue\n";
    assembly+="D;JEQ\n";
    assembly+="@R0\n";
    assembly+="M=A-2\nM=0\n"; // M=false
    assembly+="@R0\nM=M-1\n"; // sp --
    assembly+="0;JMP\n";
    assembly+="(fliptrue)\n"; // M = true
    assembly+="@R0\n";
    assembly+="M=A-2\nM=1\n"; // M=true
    assembly+="@R0\nM=M-1\n"; // sp --
  }
  assemblyFile << assembly;
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
  }
  if (type == CommandType::C_POP && segment == "static"){
    string currentStaticVariable=staticVariableName+"."+to_string(index) + "\n";
    assembly+="// Popping an element from stack\n";
    assembly+="@R0\n";
    assembly+="A=M-1\nD=M\n";
    assembly+="//static variable " + currentStaticVariable + " is now created\n";
    assembly+="@"+currentStaticVariable+"\n";
    assembly+="M=D\n";
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