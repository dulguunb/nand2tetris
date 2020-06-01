#include <iostream>
#include <fstream>
#include "Parser.h"
//#include "SymbolTable.h"
//#include "Code.h"
using namespace std;

int main(int argc, char *argv[]){
    string line;
    string filename = argv[1];
    cout << "file name: " << filename << endl;
    ifstream hackProgram(filename);
    if (hackProgram.is_open()){
      string rawProgram = "";
      while( getline(hackProgram,line) ){
        rawProgram+=line;
        rawProgram+='\n';
      }
      cout << "RAW Assembly: " << endl;
      cout << rawProgram;
      cout << "====== Scanner/Parser ======" << endl;
      Parser parser(rawProgram);
      hackProgram.close();
    }
    return 0;
}
