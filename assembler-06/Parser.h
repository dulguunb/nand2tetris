#include <iostream>
#include <vector>
using namespace std;
enum Token{
 at=1,
 d=2,
 a=3,
 m=3,
 jgt=4,
};

class Parser{
  public:
    string rawProgram;
    vector<vector<Token>> tokens;
    Parser(string rawProgram);
  private:
   void scan();
};
