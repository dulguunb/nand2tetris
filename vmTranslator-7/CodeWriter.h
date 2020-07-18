#ifndef CODEWRITER_H
#define CODEWRITER_H
#include <algorithm>
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
    void WriteBranching(CommandType,string);
    void WriteFunction(string,int);
    void WriteCall(string,int);
    void WriteReturn();
    void restoreEndFrame(string,int);
    string getOutputFile();
    ~CodeWriter();
  private:
    string outputFileName;
    string staticVariableName;
    vector<string> stack;
    int spBase = 256;
    int callCnt = 0;
    int jumpCnt = 0;
    string currentCalledFunction="";
    map<string,int> baseAddresses;
    map<string,string> segmentConverter;
    int sp = 256;
    ofstream assemblyFile;
};

#endif