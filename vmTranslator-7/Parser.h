#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <string>
#include <fstream>
#include <experimental/filesystem>
#include <vector>
#include <algorithm>
#include "CommandType.h"
using namespace std;
namespace fs = std::experimental::filesystem;

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
    vector<string> tokenize();
    vector<string> rawProgram;
    string currentLine;
    string _arg1;
    int _arg2;
};

#endif