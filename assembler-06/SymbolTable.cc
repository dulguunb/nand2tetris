#include "SymbolTable.h"
SymbolTable::SymbolTable(){
  // initalize the predefined symboltable
  table["SP"] = 0x0000;
  table["LCL"] = 0x0001;
  table["ARG"] = 0x0002;
  table["THIS"] = 0x0003;
  table["THAT"] = 0x0004;
  table["R0-R15"] = 0x0000-0xf;
  table["screen"] = 0x4000;
  table["KBD"] = 0x6000;
}
void SymbolTable::addEntry(string key,int value){
  table[key] = value;
}
bool SymbolTable::contains(string key){
  return table.find(key) != table.end();
}
int SymbolTable::getAddress(string key){
  return table[key];
}