#include "Code.h"
int Code::dest(string mnemocis){
  if(mnemocis=="")
    return codeTableDest["null"];

  return codeTableDest[mnemocis];
}
pair<int,int> Code::comp(string mnemocis){
  return codeTableComp[mnemocis];
}
int Code::jump(string mnemocis){
  if(mnemocis=="")
    return codeTableJump["null"];

  return codeTableJump[mnemocis];
}
Code::Code(){
  codeTableComp["0"] = make_pair(0b101010,0b0);
  codeTableComp["1"] = make_pair(0b111111,0b0);
  codeTableComp["-1"] = make_pair(0b111010,0b0);
  codeTableComp["D"] = make_pair(0b001100,0b0);
  codeTableComp["A"] = make_pair(0b110000,0b0);
  codeTableComp["M"] = make_pair(0b110000,0b1);
  codeTableComp["!D"] = make_pair(0b001101,0b0);
  codeTableComp["!A"] = make_pair(0b110001,0b0);
  codeTableComp["!M"] = make_pair(0b110001,0b1);
  codeTableComp["-D"] = make_pair(0b001111,0b0);
  codeTableComp["-A"] = make_pair(0b110011,0b0);
  codeTableComp["-M"] = make_pair(0b110011,0b1);
  codeTableComp["D+1"] = make_pair(0b011111,0b0);
  codeTableComp["A+1"] = make_pair(0b110111,0b0);
  codeTableComp["M+1"] = make_pair(0b110111,0b1);
  codeTableComp["D-1"] = make_pair(0b001110,0b0);
  codeTableComp["A-1"] = make_pair(0b110010,0b0);
  codeTableComp["M-1"] = make_pair(0b110010,0b1);
  codeTableComp["D+A"] = make_pair(0b000010,0b0);
  codeTableComp["D+M"] = make_pair(0b000010,0b1);
  codeTableComp["D-A"] = make_pair(0b010011,0b0);
  codeTableComp["D-M"] = make_pair(0b010011,0b1);
  codeTableComp["A-D"] = make_pair(0b000111,0b0);
  codeTableComp["M-D"] = make_pair(0b000111,0b1);
  codeTableComp["D&A"] = make_pair(0b000000,0b0);
  codeTableComp["D&M"] = make_pair(0b000000,0b1);
  codeTableComp["D|A"] = make_pair(0b010101,0b0);
  codeTableComp["D|M"] = make_pair(0b010101,0b1);

  codeTableDest["null"] = 0b000;
  codeTableDest["M"] = 0b001;
  codeTableDest["D"] = 0b010;
  codeTableDest["MD"] = 0b011;
  codeTableDest["A"] = 0b100;
  codeTableDest["AM"] = 0b101;
  codeTableDest["AD"] = 0b110;
  codeTableDest["AMD"] = 0b111;

  codeTableJump["null"] = 0b000;
  codeTableDest["JGT"] = 0b001;
  codeTableDest["JEQ"] = 0b010;
  codeTableDest["JGE"] = 0b011;
  codeTableDest["JLT"] = 0b100;
  codeTableDest["JNE"] = 0b101;
  codeTableDest["JLE"] = 0b110;
  codeTableDest["JMP"] = 0b111;
}