#ifndef CODEWRITER_H
#define CODEWRITER_H
#include <iostream>
#include <stack>
#include <fstream>
#include <vector>
#include "CommandType.h"
using namespace std;
class CodeWriter{
  public:
    CodeWriter(string);
    void writeArithmetic(string);
    void WritePushPop(CommandType,string,int);
    ~CodeWriter();
  private:
    vector<string> stack;
    int spBase = 256;
    map<string,int> baseAddresses;
    int sp = 256;
    ofstream assemblyFile;
};

#endif