#include "CodeWriter.h"
CodeWriter::CodeWriter(string filename){
  baseAddresses["LCL"] = 300;
  baseAddresses["ARG"] = 400;
  baseAddresses["THIS"] = 3000;
  baseAddresses["THAT"] = 3010;
  baseAddresses["TMP"] = 5;
  baseAddresses["sp"] = 256;
  segmentConverter["local"] = "R1";
  segmentConverter["argument"] = "R2";
  segmentConverter["this"] = "R3";
  segmentConverter["that"] = "R4";
  staticVariableName="";
  auto iter = filename.begin();
  do{
    if (*iter != '.'){
      if ( *iter == '/'){
        staticVariableName="";
      }
      else {
        staticVariableName+=*iter;
      }
      iter++;
    }
  }while(*iter != '.' && iter != filename.end());
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
  map<string,string> arguments = {{"add","+"},{"sub","-"},
    {"neg","-"},{"not","!"},{"and","&"},{"or","|"}};
  if(argument == "neg" || argument == "not"){
    assembly+="// START of "+ argument +" opeartion on the stack\n";
    assembly+="@R0\n";
    assembly+="M=M-1\n"; // sp --
    assembly+="A=M\n";
    assembly+="M="+arguments[argument]+"M\n";
    assembly+="// End of "+ argument +" opeartion on the stack\n";
  }
  if (argument == "add" || argument == "sub" ||  argument == "and" ||  argument == "or"){
    assembly+="// START OF "+ argument +" opeartion on the stack\n";
    assembly+="@R0\n";
    assembly+="M=M-1\n"; // sp --
    // SP points to x
    assembly+="A=M\n"; // go to sp--
    assembly+="D=M\n"; // store it
    assembly+="A=A-1\n"; // go to (sp-2)
    assembly+="M=M"+arguments[argument]+"D\n";
    assembly+="// End OF "+ argument +" opeartion on the stack\n";
  }
  if (argument == "eq"){
    assembly+="// START OF EQ opeartion on the stack\n";
    assembly+="@R0\n";
    assembly+="M=M-1\n"; // y
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
    assembly+="@1\nD=A\n";
    assembly+="@R0\n";
    assembly+="M=M-D\n";
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
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --

    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=0\n"; // M=false
    assembly+="0;JMP\n";// jump to the end
    assembly+="(fliptrue)\n";

    assembly+="@"+to_string(sp-2)+"\n";
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
    assembly+="@"+to_string(sp-2)+"\n";
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
    
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --

    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=0\n"; // M=false
    assembly+="0;JMP\n";
    
    assembly+="(fliptrue)\n"; // M = true
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --

    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=1\n"; // M=true

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

    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=0\n"; // M=false
    assembly+="0;JMP\n";

    assembly+="(fliptrue)\n"; // M = true
    assembly+="@"+to_string(sp-2)+"\n";
    assembly+="M=1\n"; // M=true
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M-D\n"; // sp --
    
    assembly+="// END OF OR opeartion on the stack\n";
    sp--;
  }
  assemblyFile << assembly;
}
void CodeWriter::WritePushPop(CommandType type,string segment,int index){
  string assembly="";
  if(type == CommandType::C_POP
  && 
  (segment == "local" || segment == "argument" || 
   segment == "this" || segment == "that")){
    int offset = baseAddresses[segmentConverter[segment]] + index;
    sp--;
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
    int offset =   baseAddresses[segmentConverter[segment]] + index;
    assembly+="// PUSH operation " + segment + " " + to_string(index) + "\n";
    
    assembly+="@"+to_string(index)+"\nD=A\n";
    assembly+="@"+segmentConverter[segment]+"\n";
    assembly+="D=D+M\n@a\nM=D\n"; // addr = segment + i

    assembly+="@a\nA=M\nD=M\n";
    assembly+="@R0\n";
    assembly+="A=M\nM=D\n"; // *SP=*addr

    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp ++
    sp++;
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
    sp++;
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
    sp--;
  }
  if(type == CommandType::C_PUSH && segment == "constant"){
    assembly+="// PUSH CONSTANT " + to_string(index) + "\n";
    assembly+="@"+to_string(index)+"\n";
    assembly+="D=A\n";
    assembly+="@R0\n";
    assembly+="A=M\nM=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp ++
    sp++;
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
    sp--;
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
    sp++;
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
      cout << "ERROR: This/That pointer only needs 0/1 index" << endl;
    }
    assembly+="// pop pointer\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\n";
    assembly+="M=M-D\nA=M\nD=M\n";
    assembly+=pointer;
    assembly+="M=D\n";
    sp--;
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
      cout << "ERROR: This/That pointer only needs 0/1 index" << endl;
    }
    assembly+="// push pointer\n";
    assembly+=pointer;
    assembly+="D=M\n";
    assembly+="@R0\nA=M\nM=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@R0\nM=M+D\n"; // sp ++
    sp++;
  }
  assemblyFile << assembly;
}