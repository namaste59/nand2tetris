#include <iostream>
#include <fstream>
#include <bitset>
#include <map>
#include <typeinfo>
using namespace std;

//Parserクラス
class Parser {
    public:
        ifstream file;
        string filename;
        string currentCommand;
        Parser();//コンストラクタ、入力ファイル・ストリームを開き、現在のコマンドを初期化
        bool hasMoreCommands();//入力にまだコマンドが存在するか
        void advance();//入力から次のコマンドを読み、現在のコマンドにする
        string commandType();//現在のコマンドの種類を返す
        string symbol();//現在のコマンドのシンボルまたは10進数の値を返す
        string dest();//現在のCコマンドのdestニーモニックを返す
        string comp();//現在のCコマンドのcompニーモニックを返す
        string jump();//現在のCコマンドのjumpニーモニックを返す
};

Parser::Parser() {
    cout << "Enter the file name" << endl;
    cin >> filename;
    file.open(filename);
    if(!file) {
        cout << "File not found" << endl;
        exit(1);
    }
}

bool Parser::hasMoreCommands() {
    //入力にまだコマンドが存在するか
    return !file.eof(); //eof()はファイルの終わりに達したらtrueを返す
}

void Parser::advance() {
    //入力から次のコマンドを読み、現在のコマンドにする
    getline(file, currentCommand); //ファイルから一行読み込む
    //最後尾の改行文字を削除する
    if (!currentCommand.empty() && currentCommand[currentCommand.size() - 1] == '\r') {
        currentCommand.erase(currentCommand.size() - 1);
    }
    //前方の空白を削除
    while(currentCommand.length() != 0 && currentCommand[0] == ' ') {
        currentCommand = currentCommand.substr(1);
    }
    //スペースがあれば、その後は全部削除
    int space = currentCommand.find(" ");
    if(space != string::npos) {
        currentCommand = currentCommand.substr(0, space);
    }
}

string Parser::commandType() {
    //現在のコマンドの種類を返す
    if(currentCommand[0] == '@') {
        return "A_COMMAND";
    } else if(currentCommand[0] == '(') {
        return "L_COMMAND";
    } else if (currentCommand.find("=") != string::npos || currentCommand.find(";") != string::npos){
        return "C_COMMAND";
    }else{
        return "COMMENT";
    }
}

string Parser::symbol() {
    //現在のコマンドのシンボルまたは10進数の値を返す
    if(commandType() == "A_COMMAND") {
        return currentCommand.substr(1);
    } else if(commandType() == "L_COMMAND") {
        return currentCommand.substr(1, currentCommand.length() - 2);
    } else {
        return "";
    }
}

string Parser::dest(){
    //現在のCコマンドのdestニーモニックを返す
    if(commandType() == "C_COMMAND") {
        int equal = currentCommand.find("=");
        if(equal != string::npos) {
            return currentCommand.substr(0, equal);
        } else {
            return "null";
        }
    } else {
        return "";
    }
}

string Parser::comp(){
    //現在のCコマンドのcompニーモニックを返す
    if(commandType() == "C_COMMAND") {
        int equal = currentCommand.find("=");
        int semicolon = currentCommand.find(";");
        //equalとsemicolonが発見できるかどうかで場合わけ
        //equalが発見できる場合
        if(equal != string::npos) {
            if(semicolon != string::npos) {
                //semicolonが発見できる場合、=と;の間の文字列を返す
                return currentCommand.substr(equal + 1, semicolon - equal - 1);
            } else {
                //semicolonが発見できないばあい、=の後の文字列を返す
                return currentCommand.substr(equal + 1);
            }
        } else {
            if(semicolon != string::npos) {
                //equalが発見できず、semicolonが発見できる場合、;の前の文字列を返す
                return currentCommand.substr(0, semicolon);
            } else {
                return "";
            }
        }
    } else {
        return "";
    }
}

string Parser::jump(){
    //現在のCコマンドのjumpニーモニックを返す
    if(commandType() == "C_COMMAND") {
        int semicolon = currentCommand.find(";");
        if(semicolon != string::npos) {
            return currentCommand.substr(semicolon + 1);
        } else {
            return "null";
        }
    } else {
        return "";
    }
}

//Codeクラス
class Code{
    public:
        string dest(string mnemonic);
        string comp(string mnemonic);
        string jump(string mnemonic);
};

string Code::dest(string mnemonic) {
    if(mnemonic == "null") {
        return "000";
    } else if(mnemonic == "M") {
        return "001";
    } else if(mnemonic == "D") {
        return "010";
    } else if(mnemonic == "MD") {
        return "011";
    } else if(mnemonic == "A") {
        return "100";
    } else if(mnemonic == "AM") {
        return "101";
    } else if(mnemonic == "AD") {
        return "110";
    } else if(mnemonic == "AMD") {
        return "111";
    } else {
        return "";
    }
}

string Code::comp(string mnemoninc){
    //compニーモニックをバイナリに変換する
    if(mnemoninc == "0") {
        return "0101010";
    } else if(mnemoninc == "1") {
        return "0111111";
    } else if(mnemoninc == "-1") {
        return "0111010";
    } else if(mnemoninc == "D") {
        return "0001100";
    } else if(mnemoninc == "A") {
        return "0110000";
    } else if(mnemoninc == "!D") {
        return "0001101";
    } else if(mnemoninc == "!A") {
        return "0110001";
    } else if(mnemoninc == "-D") {
        return "0001111";
    } else if(mnemoninc == "-A") {
        return "0110011";
    } else if(mnemoninc == "D+1") {
        return "0011111";
    } else if(mnemoninc == "A+1") {
        return "0110111";
    } else if(mnemoninc == "D-1") {
        return "0001110";
    } else if(mnemoninc == "A-1") {
        return "0110010";
    } else if(mnemoninc == "D+A") {
        return "0000010";
    } else if(mnemoninc == "D-A") {
        return "0010011";
    } else if(mnemoninc == "A-D") {
        return "0000111";
    } else if(mnemoninc == "D&A") {
        return "0000000";
    } else if(mnemoninc == "D|A") {
        return "0010101";
    } else if(mnemoninc == "M") {
        return "1110000";
    } else if(mnemoninc == "!M") {
        return "1110001";
    } else if(mnemoninc == "-M") {
        return "1110011";
    } else if(mnemoninc == "M+1") {
        return "1110111";
    } else if(mnemoninc == "M-1") {
        return "1110010";
    } else if(mnemoninc == "D+M") {
        return "1000010";
    } else if(mnemoninc == "D-M") {
        return "1010011";
    } else if(mnemoninc == "M-D") {
        return "1000111";
    } else if(mnemoninc == "D&M") {
        return "1000000";
    } else if(mnemoninc == "D|M") {
        return "1010101";
    } else {
        return "";
    }
}

string Code::jump(string mnemonic) {
    if(mnemonic == "null") {
        return "000";
    } else if(mnemonic == "JGT") {
        return "001";
    } else if(mnemonic == "JEQ") {
        return "010";
    } else if(mnemonic == "JGE") {
        return "011";
    } else if(mnemonic == "JLT") {
        return "100";
    } else if(mnemonic == "JNE") {
        return "101";
    } else if(mnemonic == "JLE") {
        return "110";
    } else if(mnemonic == "JMP") {
        return "111";
    } else {
        return "";
    }
}

//SymbolTableクラス
class SymbolTable{
    public:
        SymbolTable();
        bool contains(string symbol);
        int getAddress(string symbol);
        void addEntry(string symbol, int address);
    private:
        map<string, int> symbolTable; 
};

SymbolTable::SymbolTable(){
    //シンボルテーブルを初期化する
    symbolTable["SP"] = 0;
    symbolTable["LCL"] = 1;
    symbolTable["ARG"] = 2;
    symbolTable["THIS"] = 3;
    symbolTable["THAT"] = 4;
    symbolTable["R0"] = 0;
    symbolTable["R1"] = 1;
    symbolTable["R2"] = 2;
    symbolTable["R3"] = 3;
    symbolTable["R4"] = 4;
    symbolTable["R5"] = 5;
    symbolTable["R6"] = 6;
    symbolTable["R7"] = 7;
    symbolTable["R8"] = 8;
    symbolTable["R9"] = 9;
    symbolTable["R10"] = 10;
    symbolTable["R11"] = 11;
    symbolTable["R12"] = 12;
    symbolTable["R13"] = 13;
    symbolTable["R14"] = 14;
    symbolTable["R15"] = 15;
    symbolTable["SCREEN"] = 16384;
    symbolTable["KBD"] = 24576;
}
bool SymbolTable::contains(string symbol){
    //シンボルテーブルにシンボルが存在するかどうかを返す
    if(symbolTable.find(symbol) != symbolTable.end()){
        return true;
    }else{
        return false;
    }
}

int SymbolTable::getAddress(string symbol){
    //シンボルテーブルからアドレスを取得する
    return symbolTable[symbol];
}

void SymbolTable::addEntry(string symbol, int address){
    //シンボルテーブルに新しいシンボルを追加する
    //すでにシンボルが存在する場合は何もしない
    if(contains(symbol) == false){
        symbolTable[symbol] = address;
    }
}


int main(){
    string Prog;
    string Progline;
    Parser parser;
    Code code;
    SymbolTable symbolTable;
    int currentCommandNum = 0;
    int ramAdd = 16;
    //1回目のパス
    while (parser.hasMoreCommands()){
        parser.advance();
        if(parser.commandType() == "L_COMMAND"){
            //シンボルテーブルにシンボルを追加する
            symbolTable.addEntry(parser.symbol(), currentCommandNum);
            cout << parser.symbol() << endl;
            cout <<  currentCommandNum << endl;
        }else if(parser.commandType() != "COMMENT"){
            currentCommandNum++;
        }
    }
    //2回目のパスを読む前に、ファイルポインタ、currentcommandnumを先頭に戻す
    parser.file.clear();
    parser.file.seekg(0, ios::beg);
    while (parser.hasMoreCommands()){
        parser.advance();
        cout << "currentCommand:" << parser.currentCommand << endl;
        cout << "currentCommandlength:" << parser.currentCommand.length() << endl;
        cout << "commandType:" << parser.commandType() << endl;
        if(parser.commandType() == "COMMENT"){
            continue;
        }else if(parser.commandType() == "A_COMMAND"){
            //Aコマンドの場合(0vvv vvvv vvvv vvvv)
            Progline += "0";
            //シンボルが文字列の時
            cout << parser.symbol() << endl;
            if(parser.symbol()[0] >= 'A' && parser.symbol()[0] <= 'z'){
                //シンボルテーブルにシンボルが存在するかどうかを確認する
                if(symbolTable.contains(parser.symbol()) == false){
                    //シンボルテーブルにシンボルが存在しない場合は新しいアドレスを割り当てる
                    symbolTable.addEntry(parser.symbol(), ramAdd);
                    ramAdd++;
                }
                //シンボルテーブルからアドレスを取得する
                int num = symbolTable.getAddress(parser.symbol());
                //10進数を2進数に変換する
                bitset<15> bits(num);
                //2進数を文字列に変換する
                string str = bits.to_string();
                Progline += str;
            }else{
                //シンボルを10進数に変換する
                int num = stoi(parser.symbol());
                //10進数を2進数に変換する
                bitset<15> bits(num);
                //2進数を文字列に変換する
                string str = bits.to_string();
                Progline += str;
            }
        }else if(parser.commandType() == "C_COMMAND"){
            //Cコマンドの場合(111a cccc ccdd djjj)
            Progline += "111";
            cout << "comp:" << parser.comp() << endl;
            cout << "dest:" << parser.dest() << endl;
            cout << "jump:" << parser.jump() << endl;
            cout << "compbinary:" << code.comp(parser.comp()) << endl;
            cout << "destbinary:" << code.dest(parser.dest()) << endl;
            cout << "jumpbinary:" << code.jump(parser.jump()) << endl;
            Progline += code.comp(parser.comp());
            Progline += code.dest(parser.dest());
            Progline += code.jump(parser.jump());
        }else if(parser.commandType() == "L_COMMAND"){
            continue;
        }
        cout << "binaryline:" << Progline << endl;
        Prog += Progline;
        Prog += "\n";
        Progline = "";
    }
    //ファイルに書き込む
    string filename = parser.filename.substr(0, parser.filename.length() - 3) + "hack";
    ofstream file;
    file.open(filename);
    file << Prog;
    return 0;
}