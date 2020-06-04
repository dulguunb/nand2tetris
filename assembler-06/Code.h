#ifndef CODE_H
#define CODE_H
#include <iostream>
#include <bitset>
#include <map>
using namespace std;
class Code{
  public:
    Code();
    int dest(string);
    pair<int,int> comp(string);
    int jump(string);
  private:
    map<string,pair<int,int>> codeTableComp;
    map<string,int> codeTableJump;
    map<string,int> codeTableDest;
};
#endif
