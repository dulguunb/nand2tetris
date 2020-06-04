#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"
#include <bitset>
//#include "SymbolTable.h"
#include "Code.h"
using namespace std;

int main(int argc, char *argv[]){
    string line;
    string filename = argv[1];
    cout << "file name: " << filename << endl;
    ifstream hackProgram(filename);
    ofstream machineCode(filename+".hack");
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
      Code code;
      while(parser.hasMoreCommands()){
        if(parser.commandType() == Command::Comment){
          parser.skipComment();
        }
        else if(parser.commandType() == Command::C){
          auto dest = parser.dest();
          auto comp = parser.comp();
          auto jump = parser.jump();
          cout << " ===== C command ===== " << endl;
          cout << "dest: " << dest << endl;
          cout << "comp : " << comp << endl;
          cout << "jump: " << jump << endl;
          auto compBin = code.comp(comp);
          auto destBin = code.dest(dest);
          auto jumpBin = code.jump(dest);
          string a = bitset<1>(compBin.second).to_string<char,std::string::traits_type,
            std::string::allocator_type>();
          string c = bitset<6>(compBin.first).to_string<char,std::string::traits_type,
            std::string::allocator_type>();
          string d = bitset<3>(destBin).to_string<char,std::string::traits_type,
            std::string::allocator_type>();
          string j = bitset<3>(jumpBin).to_string<char,std::string::traits_type,
            std::string::allocator_type>();
          cout << "dest: " << d << endl;
          cout << "comp<c,a>: " << c << "," << a << endl;
          cout << "jump: " << j << endl;
          cout << " ================== " << endl;
          // concatinating opcode
          string result = "111"+a+c+d+j;
          cout << "result: " << result << endl;
          machineCode << result;
        }
        else if(parser.commandType() == Command::L){
          cout << "=========L command=========" << endl;
          cout << "symbol: " << parser.symbol() << endl;
          cout << "=======================" << endl;
        }
        else if ( parser.commandType() == Command::A ){
          cout << "=======A command=======" << endl;
          auto symbol = parser.symbol();
          int aInt = stoi(symbol);
          // concatinating the opcode
          auto symbolBin = "0"+bitset<15>(aInt).to_string();
          cout << "symbol: " << symbol << endl;
          cout << "symbolBin: " << symbolBin << endl;
          machineCode << symbolBin ;
          cout << "=======================" << endl;
        }
        else if(parser.commandType() == Command::Space){
          parser.advance();
        }
        machineCode << endl;
      }
      hackProgram.close();
      machineCode.close();
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
