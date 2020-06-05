#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"
#include <bitset>
#include "SymbolTable.h"
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
      Parser *parser = new Parser(rawProgram);
      Code code;
      SymbolTable symbolTable;
      int romAddress = 0;
      // First Pass
      cout << "================First Pass===============" << endl;
      while(parser->hasMoreCommands()){
        if(parser->commandType() == Command::Comment){
          parser->skipComment();
        }
        else if(parser->commandType() == Command::C){
          romAddress++;
          auto dest = parser->dest();
          auto comp = parser->comp();
          auto jump = parser->jump();
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
        }
        else if( parser->commandType() == Command::L ){
          cout << "=========L command=========" << endl;
          auto symbol = parser->symbol();
          cout << "symbol: " << symbol << endl;
          symbolTable.addEntry(symbol,romAddress);
          cout << "=======================" << endl;
        }
        else if ( parser->commandType() == Command::A ){
          cout << "=======A command=======" << endl;
          auto symbol = parser->symbol();
          cout << "Symbol: " << symbol << endl;
          bool isDigit = true;
          int aInt;
          for(auto iter=symbol.begin();iter!=symbol.end();iter++){
            if(!isdigit(*iter)){
              isDigit=false;
            }
          }
          if(isDigit){
            cout << "aInt: " << aInt << endl;
            int aInt = stoi(symbol);
            // concatinating the opcode
            auto symbolBin = "0"+bitset<15>(aInt).to_string();
            cout << "symbol: " << symbol << endl;
            cout << "symbolBin: " << symbolBin << endl;
            cout << "=======================" << endl;
          }
          else {
            cout << "Variable is occured: " << symbol << endl;
            symbolTable.addEntry(symbol,romAddress);
          }
        }
        else{
          parser->advance();
        }
        cout << "PARSER END: " << parser->hasMoreCommands() << endl;
      }
      cout << "===================Second Pass ========================" << endl;
      // Second Pass
      int newVariableAddress = 16;
      delete parser;
      parser = new Parser(rawProgram);
      cout << "=========Second Pass=========" << endl;
      while(parser->hasMoreCommands()){
        if(parser->commandType() == Command::Comment){
          parser->skipComment();
        }
        else if(parser->commandType() == Command::C){
          auto dest = parser->dest();
          auto comp = parser->comp();
          auto jump = parser->jump();
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
          machineCode << endl;
        }
        else if(parser->commandType() == Command::L){
          cout << "=========L command=========" << endl;
          cout << "=======================" << endl;
        }
        else if ( parser->commandType() == Command::A ){
          cout << "=======A command=======" << endl;
          auto symbol = parser->symbol();
          bool isNotDigit = true;
          for(auto iter=symbol.begin();iter!=symbol.end();iter++){
            if(isdigit(*iter)){
              isNotDigit=false;
            }
          }
          if(isNotDigit){ // this is a variable or label
            if (symbolTable.contains(symbol)){ // variable is already stored varieble
              // concatinating the opcode
              cout << "================ Allocated Variable ==========" << endl;
              int aInt = symbolTable.getAddress(symbol);
              auto symbolBin = "0"+bitset<15>(aInt).to_string();
              cout << "symbol: " << symbol << endl;
              cout << "symbolBin: " << symbolBin << endl;
              machineCode << symbolBin ;
              machineCode << endl;
              cout << "=======================" << endl;
            }
            else { // variable doesn't exist
              cout << "=================A new Variable Occured =========" << endl;
              symbolTable.addEntry(symbol,newVariableAddress);
              newVariableAddress++;
              int aInt = symbolTable.getAddress(symbol);
              auto symbolBin = "0"+bitset<15>(aInt).to_string();
              cout << "symbol: " << symbol << endl;
              cout << "symbolBin: " << symbolBin << endl;
              machineCode << symbolBin ;
              machineCode << endl;
              cout << "============================" << endl;
            }
          }
          else { // a raw number
              int aInt = stoi(symbol);
              auto symbolBin = "0"+bitset<15>(aInt).to_string();
              cout << "symbol: " << symbol << endl;
              cout << "symbolBin: " << symbolBin << endl;
              machineCode << symbolBin ;
              machineCode << endl;
              cout << "=======================" << endl;
          }
        }
        else{
          parser->advance();
        }
      }
      delete parser;
      hackProgram.close();
      machineCode.close();
    }
    return 0;
}
