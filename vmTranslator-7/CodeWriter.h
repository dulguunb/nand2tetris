#ifndef CODEWRITER_H
#define CODEWRITER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "CommandType.h"
using namespace std;
class CodeWriter{
  public:
    CodeWriter(string);
    void writeArithmetic(string);
    void WritePushPop(CommandType,string,int);
    ~CodeWriter();
  private:
    string staticVariableName;
    vector<string> stack;
    int spBase = 256;
    map<string,int> baseAddresses;
    map<string,string> segmentConverter;
    int sp = 256;
    ofstream assemblyFile;
};

#endif