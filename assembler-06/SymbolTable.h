#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <iostream>
#include <map>
using namespace std;

class SymbolTable {
  private:
  map<string,int> table;
  public:
    SymbolTable();
  void addEntry(string,int);
  bool contains(string);
  int getAddress(string);
};

#endif