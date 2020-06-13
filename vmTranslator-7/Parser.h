#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "CommandType.h"
using namespace std;

class Parser{
  public:
    Parser(string filename);
    bool hasMoreCommands();
    void advance();
    CommandType commandType();
    string arg1();
    int arg2();
    ~Parser();
    string getCurrentLine();
  private:
    int lineCnt;
    int lineLength;
    vector<string> rawProgram;
    string currentLine;
    ifstream program;
    string _arg1;
    int _arg2;
    vector<string> tokens;
};

#endif