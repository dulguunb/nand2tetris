#include <iostream>
#include <bitset>
#include <map>
using namespace std;
class Code{
  public:
    Code();
    int dest(string);
    int comp(string);
    int jump(string);
  private:
    map<string,int> codeTableComp;
    map<string,int> codeTableJump;
    map<string,int> codeTableDest;
};
