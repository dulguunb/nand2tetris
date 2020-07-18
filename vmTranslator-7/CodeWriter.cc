#include "CodeWriter.h"
CodeWriter::CodeWriter(string filename){
  segmentConverter["local"] = "LCL";
  segmentConverter["argument"] = "ARG";
  segmentConverter["this"] = "THIS";
  segmentConverter["that"] = "THAT";
  string newFileDirectory = "";
  staticVariableName="";
  string parentDirectory = "";
  auto iter = filename.begin();
  string extension = "";
  string lastDir = "";
  bool isDir = false;
  string outputFilename="";
  int extensionSPos = filename.find(".vm");
  if( extensionSPos != string::npos){ // singlefile
    outputFilename = filename.substr(0,extensionSPos)+".asm";
    staticVariableName = filename.substr(0,extensionSPos);
  }
  else { //directory
      isDir = true;
      if(*(filename.end()-1) == '/'){
         filename.pop_back();
      }
      int lastDirPos = filename.find_last_of('/');
      string asmFileName = filename;
      if(lastDirPos != string::npos){
        asmFileName = filename.substr(lastDirPos,filename.length());
      }
      staticVariableName = asmFileName;
      outputFilename = filename+'/'+asmFileName+".asm";
  }
  outputFileName = outputFilename;
  #ifdef debug8
  cout << "outputFilename: " << outputFilename << endl;
  #endif
  assemblyFile.open(outputFilename);
  if(isDir){
    string assembly="@256\nD=A\n@SP\nM=D\n";
    assemblyFile << assembly;
    WriteCall("Sys.init",0);
  }
}
string CodeWriter::getOutputFile(){
  return outputFileName;
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
    assembly+="@SP\n";
    assembly+="D=M-1\n"; // sp --
    assembly+="A=D\n";
    assembly+="M="+arguments[argument]+"M\n";
    assembly+="// End of "+ argument +" opeartion on the stack\n";
    assemblyFile << assembly;
    return;
  }
  assembly+="@SP\n";
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
    assembly+="@SP\n";
    assembly+="A=M-1\n";
    assembly+="M=-1\n"; // setting it 1111111111111111
    assembly+="@Continue_" + to_string(jumpCnt) + "\n";
    assembly+="0;JMP\n";
    assembly+="(False_" + to_string(jumpCnt)+ ")\n";
    assembly+="@SP\n";
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
    assembly+="@SP\nM=M-D\n"; // sp --

    assembly+="@SP\n";
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
    assembly+="@SP\n";
    assembly+="A=M\nM=D\n"; // *SP=*addr
    assembly+="@1\nD=A\n";
    assembly+="@SP\nM=M+D\n"; // sp ++
  }
  if(type == CommandType::C_PUSH && segment == "temp"){
    assembly+="// PUSH operation " + segment + " " + to_string(index) + "\n";

    assembly+="@"+to_string(index)+"\nD=A\n";
    assembly+="@5\n";
    assembly+="D=D+A\n@a\nM=D\n"; // addr= 5 + i

    assembly+="@a\nA=M\nD=M\n";
    assembly+="@SP\n";
    assembly+="A=M\nM=D\n"; // *SP=*addr

    assembly+="@1\nD=A\n";
    assembly+="@SP\nM=M+D\n"; // sp ++

  }
  if(type == CommandType::C_POP && segment == "temp"){
    assembly+="// POP operation " + segment + " " + to_string(index) + "\n";

    assembly+="@"+to_string(index)+"\nD=A\n";
    assembly+="@5\n";
    assembly+="D=D+A\n@a\nM=D\n"; // addr=5+i

    assembly+="@1\nD=A\n";
    assembly+="@SP\nM=M-D\n"; // sp --

    assembly+="@SP\n";
    assembly+="A=M\nD=M\n"; // *addr=*SP
    assembly+="@a\nA=M\nM=D\n";

  }
  if(type == CommandType::C_PUSH && segment == "constant"){
    assembly+="// PUSH CONSTANT " + to_string(index) + "\n";
    assembly+="@"+to_string(index)+"\n";
    assembly+="D=A\n";
    assembly+="@SP\n";
    assembly+="A=M\nM=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@SP\nM=M+D\n"; // sp ++

  }
  if(type == CommandType::C_POP && segment == "static"){
    string currentStaticVariable=staticVariableName+"."+to_string(index) + "\n";
    assembly+="// Popping an element from stack\n";
    assembly+="@1\nD=A\n";
    assembly+="@SP\n";
    assembly+="A=M-D\nD=M\n";
    assembly+="// static variable " + currentStaticVariable + "\n";
    assembly+="@"+currentStaticVariable+"\n";
    assembly+="M=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@SP\nM=M-D\n"; // sp --

  }
  if(type == CommandType::C_PUSH && segment == "static"){

    string currentStaticVariable=staticVariableName+"."+to_string(index) + "\n";
    assembly+="// Pushing an element from stack\n";
    assembly+="// static variable " + currentStaticVariable+"\n";
    assembly+="@"+currentStaticVariable+"\n";
    assembly+="D=M\n";
    assembly+="@SP\n";
    assembly+="A=M\nM=D\n";
    assembly+="// incrementing stack pointer\n";
    assembly+="@1\nD=A\n";
    assembly+="@SP\nM=M+D\n"; // sp ++

  }
  if(type == CommandType::C_POP && segment == "pointer"){

    string pointer = "";
    if(index == 0){
      // this pointer
      pointer = "@THIS\n";
    }
    else if(index == 1){
      // that pointer
      pointer = "@THAT\n";
    }
    else{
      #ifdef debug7
      cout << "ERROR: This/That pointer only needs 0/1 index" << endl;
      #endif
    }
    assembly+="// pop pointer\n";
    assembly+="@1\nD=A\n";
    assembly+="@SP\n";
    assembly+="M=M-D\nA=M\nD=M\n";
    assembly+=pointer;
    assembly+="M=D\n";

  }
  if (type == CommandType::C_PUSH && segment == "pointer"){

    string pointer = "";
    if(index == 0){
      // this pointer
      pointer = "@THIS\n";
    }
    else if(index == 1){
      // that pointer
      pointer = "@THAT\n";
    }
    else{
      #ifdef debug7
      cout << "ERROR: This/That pointer only needs 0/1 index" << endl;
      #endif
    }
    assembly+="// push pointer\n";
    assembly+=pointer;
    assembly+="D=M\n";
    assembly+="@SP\nA=M\nM=D\n";
    assembly+="@1\nD=A\n";
    assembly+="@SP\nM=M+D\n"; // sp ++

  }
  assemblyFile << assembly;
}
void CodeWriter::WriteCall(string arg1, int arg2){
  string label="RET_"+staticVariableName+'_'+to_string(callCnt);
  string assembly="@"+label+"\n";
  assembly+="// write call started\n";
  assembly+="D=A\n";
  assembly+="@SP\n";
  assembly+="A=M\n";
  assembly+="M=D\n";
  assembly+="@SP\n";
  assembly+="M=M+1\n";
  vector <string> segments = {"@LCL","@ARG","@THIS","@THAT"};
  for(auto iter=segments.begin();iter!=segments.end();iter++){
    assembly+=*iter+"\n";
    assembly+="D=M\n";
    assembly+="@SP\n";
    assembly+="A=M\n";
    assembly+="M=D\n";
    assembly+="@SP\n";// sp
    assembly+="M=M+1\n";// sp++
  }
  assembly+="@SP\n";
  assembly+="D=M\n";
  assembly+="@LCL\n";
  assembly+="M=D\n";

  assembly+="@"+to_string(arg2+5)+"\n";
  assembly+="D=D-A\n";
  assembly+="@ARG\n";
  assembly+="M=D\n";
  assembly+="@"+arg1+"\n";
  assembly+="0;JMP\n";
  assembly+="// end of JUMP to FunctionName\n";
  assembly+="("+label+")"+"\n";
  callCnt++;
  assembly+="// write call ended\n";
  assemblyFile << assembly;
}
void CodeWriter::WriteFunction(string arg1, int arg2){
  string assembly = "// WriteFunction \n";
  assembly+="("+arg1+")\n";
  currentCalledFunction=arg1+"$";
  for(int i=0;i<arg2;i++){
      assembly+="@0\n";
      assembly+="D=A\n";
      assembly+="@SP\n";
      assembly+="A=M\n";
      assembly+="M=D\n";
      assembly+="@SP\n";
      assembly+="M=M+1\n";
  }
  assemblyFile << assembly;
}
void CodeWriter::WriteReturn(){
  string assembly = "// endFrame = LCL\n";
  assembly+="@LCL\n"; //LCL
  assembly+="D=M\n";
  assembly+="@R14\n";
  assembly+="M=D\n"; // frame=LCL

  assembly+="@5\n";
  assembly+="A=D-A\n";//(endFrame-5)
  assembly+="D=M\n"; // *(endFrame - 5)
  assembly+="@R15\n";
  assembly+="M=D\n"; // retAddr = *(endFrame-5)
  // pop argument
  assembly+="@ARG\n";
  assembly+="D=M\n";
  assembly+="@0\n";
  assembly+="D=D+A\n";
  assembly+="@R13\n";
  assembly+="M=D\n";
  assembly+="@SP\n";
  assembly+="AM=M-1\n";
  assembly+="D=M\n";
  assembly+="@R13\n";
  assembly+="A=M\n";
  assembly+="M=D\n";
  // assemblyFile << assembly;
  assembly+="@ARG\n"; //arg
  assembly+="D=M\n";
  assembly+="@SP\n";
  assembly+="M=D+1\n";
  vector<string> segments = {"@THAT","@THIS","@ARG","@LCL"};
  for(auto segment=segments.begin();segment!=segments.end();segment++){
    assembly+="@R14\n";
    assembly+="AMD=M-1\n";
    assembly+="D=M\n";
    assembly+=*segment +"\n";
    assembly+="M=D\n";
  }
  assembly+="@R15\n";
  assembly+="A=M\n";
  assembly+="0;JMP\n";
  assemblyFile << assembly;
}
void CodeWriter::WriteBranching(CommandType type,string argument){
  string assembly = "";
  if(type == CommandType::C_GOTO){
    assembly+="// CommandType::C_GOTO start\n";
    assembly+="@"+currentCalledFunction+argument+"\n";
    assembly+="0;JMP\n";
    assembly+="// CommandType::C_GOTO end\n";
  }
  if(type == CommandType::C_IF){
    assembly+="// CommandType::C_IF start\n";
    assembly+="@SP\n";
    assembly+="M=M-1\n"; // sp --
    assembly+="A=M\n";
    assembly+="D=M\n"; // store it
    assembly+="@"+currentCalledFunction+argument+"\n";
    assembly+="D;JNE\n"; // if it's 0 then it must be true
    // else just carry on with the execution
    assembly+="// CommandType::C_IF end\n";
  }
  if(type == CommandType::C_LABEL){
    assembly+="// CommandType::C_LABEL start\n";
    assembly+="("+currentCalledFunction+argument+")\n";
    assembly+="// CommandType::C_LABEL end\n";
  }
  assemblyFile << assembly;
}