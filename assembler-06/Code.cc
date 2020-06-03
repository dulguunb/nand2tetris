#include "Code.h"
int Code::dest(string mnemocis){

}
int Code::comp(string mnemocis){

}
int Code::jump(string mnemocis){

}
Code::Code(){
  codeTableComp["0"] = 0b101010;
  codeTableComp["1"] = 0b111111;
  codeTableComp["-1"] = 0b111010;
  codeTableComp["D"] = 0b001100;
  codeTableComp["A"] = 0b110000;
  codeTableComp["M"] = 0b110000;
  codeTableComp["!D"] = 0b001101;
  codeTableComp["!A"] = 0b110001;
  codeTableComp["!M"] = 0b110001;
  codeTableComp["-D"] = 0b001111;
  codeTableComp["-A"] = 0b110011;
  codeTableComp["-M"] = 0b110011;
  codeTableComp["D+1"] = 0b011111;
  codeTableComp["A+1"] =  0b110111;
  codeTableComp["M+1"] =  0b110111;
  codeTableComp["D-1"] = 0b001110;
  codeTableComp["A-1"] = 0b110010;
  codeTableComp["M-1"] = 0b110010;
  codeTableComp["D+A"] = 0b000010;
  codeTableComp["D+M"] = 0b000010;
  codeTableComp["D-A"] = 0b010011;
  codeTableComp["D-M"] = 0b010011;
  codeTableComp["A-D"] =  0b000111;
  codeTableComp["M-D"] =  0b000111;
  codeTableComp["D&A"] = 0b000000;
  codeTableComp["D&M"] = 0b000000;
  codeTableComp["D|A"] = 0b010101;
  codeTableComp["D|M"] = 0b010101;
}