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
  init+="@"+to_string(baseAddresses["sp"])+"\n";
  init+="D=A\n";
  init+="@R0\n";
  init+="M=D\n";
  init+="@"+to_string(baseAddresses["LCL"])+"\n";
  init+="D=A\n";
  init+="@R1\n";
  init+="M=D\n";
  init+="@"+to_string(baseAddresses["ARG"])+"\n";
  init+="D=A\n";
  init+="@R2\n";
  init+="M=D\n";
  init+="@"+to_string(baseAddresses["THIS"])+"\n";
  init+="D=A\n";
  init+="@R3\n";
  init+="M=D\n";
  init+="@"+to_string(baseAddresses["THAT"])+"\n";
  init+="D=A\n";
  init+="@R4\n";
  init+="M=D\n";
  cout << init << endl;
  assemblyFile << init;

}
CodeWriter::~CodeWriter(){
  assemblyFile.close();
}
void CodeWriter::writeArithmetic(string argument){
  string assembly = "";
  if (argument == "add"){
    assembly+="// START OF ADD opeartion on the stack\n";
    /*
         x
         y
    sp- >
    ========> add
          x+y
    sp -> 
    */
    assembly+="@"+to_string(sp-1)+"\n";
    assembly+="D=M\n"; // y
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=M+D\n"; // x=x+y
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp--
    assembly+="// END OF add opeartion on the stack\n";
    sp--;
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
    assembly+="// START OF SUB opeartion on the stack\n";
    assembly+="@"+to_string(sp-1)+"\n";
    assembly+="D=M\n"; // y
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=M-D\n"; // x=x+y
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp--
    sp--;
    assembly+="// END OF sub opeartion on the stack\n";
  }
  if(argument == "neg"){
    assembly+="// START OF NEG opeartion on the stack\n";
    assembly+="@0\nD=A\n";
    assembly+="@R0\n";
    assembly+="M=A-1\nM=D-M\n";
    assembly+="// END OF neg opeartion on the stack\n";
  }
  if (argument == "eq"){
    assembly+="// START OF EQ opeartion on the stack\n";
    assembly+="@"+to_string(sp-1)+"\n";
    assembly+="D=M\n"; // y
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="D=M-D\n";
    assembly+="@fliptrue\n";
    assembly+="D;JEQ\n"; // if D == 0
    assembly+="@"+to_string(sp-2)+"\n";//  M=1 -> False
    assembly+="M=0\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\n";
    assembly+="M=M-D\n"; //  set A-1 to M=0 -> False
    assembly+="0;JMP\n";// jump to the end
    assembly+="(fliptrue)\n";
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=1\n"; // M=true
    assembly+="// END OF AND opeartion on the stack\n";
    sp--;
  }
  if (argument == "gt"){
    assembly+="// START OF GT opeartion on the stack\n";
    assembly+="@"+to_string(sp-1)+"\n";
    assembly+="D=M\n"; // y
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="D=D-M\n"; // y-x
    assembly+="@fliptrue\n";
    assembly+="D;JGT\n"; // z>0 positive then set sp-2 to true and sp--
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=0\n"; // M=false
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    assembly+="0;JMP\n";// jump to the end
    assembly+="(fliptrue)\n";
    assembly+="@"+to_string(sp-2);
    assembly+="M=1\n"; // M=true
    assembly+="// END OF AND opeartion on the stack\n";
    sp--;
  }
  if (argument == "lt"){
    assembly+="// START OF LT opeartion on the stack\n";
    assembly+="@"+to_string(sp-1)+"\n";
    assembly+="D=M\n"; // y
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="D=D-M\n"; // z=(y - x)
    assembly+="@flipfalse\n";
    assembly+="D;JGT\n"; // z>0 positive then set sp-2 to true and sp--
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=1\n"; // M=true
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    assembly+="0;JMP\n";
    assembly+="(flipfalse)\n";
    assembly+="@"+to_string(sp-2);
    assembly+="M=0\n"; // M=false
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    assembly+="// END OF AND opeartion on the stack\n";
    sp--;
  }
  if(argument == "and"){
    assembly+="// START OF AND opeartion on the stack\n";
    assembly+="@2\n";
    assembly+="D=A\n";
    assembly+="@"+to_string(sp-1)+"\n";
    assembly+="D=D-M\n"; // z=2 - (y)
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="D=D-M\n"; // D=z-(x)
    assembly+="@fliptrue\n";
    assembly+="D;JEQ\n";
    assembly+="@\n"+to_string(sp-2);
    assembly+="M=0\n"; // M=false
    assembly+="0;JMP\n";
    assembly+="(fliptrue)\n"; // M = true
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=1\n"; // M=true
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    assembly+="// END OF AND opeartion on the stack\n";
    sp--;
  }
  if(argument == "or"){
    assembly+="// START OF OR opeartion on the stack\n";
    assembly+="@0\n";
    assembly+="D=A\n";
    assembly+="@"+to_string(sp-1)+"\n";
    assembly+="D=D-M\n"; // z = 0 - (y)
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="D=D-M\n"; // z - (x)
    assembly+="@fliptrue\n";
    assembly+="D;JLT\n";
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=0\n"; // M=false
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    assembly+="0;JMP\n";
    assembly+="(fliptrue)\n"; // M = true
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=1\n"; // M=true
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    assembly+="// END OF OR opeartion on the stack\n";
    sp--;
  }
  if(argument == "not"){
    assembly+="// START OF NOT opeartion on the stack\n";
    assembly+="@R0\n";
    assembly+="M=A-2\nD=M\n"; // x
    assembly+="@fliptrue\n";
    assembly+="D;JEQ\n";
    assembly+="@"+to_string(sp-2);
    assembly+="M=0\n"; // M=false
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    assembly+="0;JMP\n";
    assembly+="(fliptrue)\n"; // M = true
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=1\n"; // M=true
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    assembly+="0;JMP\n";
    assembly+="// START OF NOT opeartion on the stack\n";
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
    assembly+="@1\nD=A\n";
    assembly+="@R0\n";
    assembly+="A=M-D\nD=M\n"; // go to the address where sp-1 points at and save it
    assembly+="@"+to_string(offset)+"\n";
    assembly+="M=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
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
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp --
    sp++;
  }
  if(type == CommandType::C_PUSH && segment == "constant"){
    assembly+="// PUSH CONSTANT " + to_string(index) + "\n";
    assembly+="@"+to_string(index)+"\n";
    assembly="D=A\n";
    assembly+="@R0\n";
    assembly+="A=M\nM=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp ++
    sp++;
  }
  if (type == CommandType::C_POP && segment == "static"){
    string currentStaticVariable=staticVariableName+"."+to_string(index) + "\n";
    assembly+="// Popping an element from stack\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\n";
    assembly+="A=M-D\nD=M\n";
    assembly+="//static variable " + currentStaticVariable + " is now created\n";
    assembly+="@"+currentStaticVariable+"\n";
    assembly+="M=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    sp--;
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
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp ++
    sp++;
  }
  if (type == CommandType::C_POP && segment == "pointer"){
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
    assembly+="@1\nD=A\n";
    assembly+="@R0\n";
    assembly+="A=M-D\nD=M\n";
    assembly="@"+pointer+"\n";
    assembly+="M=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    sp--;
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
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp ++
    sp++;
  }
  assemblyFile << assembly;
}