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
    bool isEnd = false;
    string rawProgram;
    string::iterator currentLetter;
    vector<vector<Command>> tokens;
    Parser(string rawProgram);
  // private:
   void advance();
   void skipComment();
   bool hasMoreCommands();
   Command commandType();
   string symbol();
   string dest();
   string comp(); // returns comp mnemoics in current C_COMMAND
   string jump(); // returns jump mnemoics in current C_COMMAND
};
