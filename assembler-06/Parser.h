#include <iostream>
#include <vector>
using namespace std;
enum Command {
  A,
  C,
  L
};

class Parser{
  public:
    string rawProgram;
    string::iterator currentLetter;
    vector<vector<Token>> tokens;
    Parser(string rawProgram);
  private:
   void advance();
   bool hasMoreCommands();
   Command commandType();
   string symbol();
   string dest();
   string comp(); // returns comp mnemoics in current C_COMMAND
   string jump(); // returns jump mnemoics in current C_COMMAND
};
