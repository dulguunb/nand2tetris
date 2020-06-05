#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <vector>
using namespace std;
enum Command {
  A,
  C,
  L,
  Comment,
  Space
};

class Parser{
  public:
    string::iterator currentLetter;
    vector<vector<Command>> tokens;
    Parser(string rawProgram);
    void advance();
    void skipComment();
    bool hasMoreCommands();
    Command commandType();
    string symbol();
    string dest();
    string comp(); // returns comp mnemoics in current C_COMMAND
    string jump(); // returns jump mnemoics in current C_COMMAND
    void reset();
  private:
    bool isEnd = false;
    int letterCnt = 0;
    string rawProgram="";
};
#endif