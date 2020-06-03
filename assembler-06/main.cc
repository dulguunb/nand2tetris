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
      while(parser.hasMoreCommands()){
        if(parser.commandType() == Command::Comment){
          parser.advance();
        }
        else if(parser.commandType() == Command::C){
          auto dest = parser.dest();
          auto comp = parser.comp();
          auto jump = parser.jump();
          cout << "C command " << endl;
          cout << dest << '=' << comp << ';' << jump << endl;
        }
        else if(parser.commandType() == Command::L || parser.commandType() == Command::A){
          cout << "A&L command " << endl;
          cout << parser.symbol() << endl;
        }
        else if(parser.commandType() == Command::Space){
          parser.advance();
        }
        cout << parser.hasMoreCommands() << endl;
      }
      // while(true){
      //   cout << "while loop!" << endl;
      //   string instruction;
      //   cin >> instruction;
      //   cout << instruction << endl;
      //   Parser parser(instruction);
      //   cout << "Command Type: " <<  parser.commandType() << endl;
      //   if(parser.commandType() == Command::Comment){
      //     parser.advance();
      //   }
      //   else if(parser.commandType() == Command::C){
      //     auto dest = parser.dest();
      //     auto comp = parser.comp();
      //     auto jump = parser.jump();
      //     cout << "C command " << endl;
      //     cout << dest << '=' << comp << ';' << jump << endl;
      //   }
      //   else if(parser.commandType() == Command::L || parser.commandType() == Command::A){
      //     cout << "A&L command " << endl;
      //     cout << parser.symbol() << endl;
      //   }
      //   cout << parser.hasMoreCommands() << endl;
      // }
      hackProgram.close();
    }
    return 0;
}
