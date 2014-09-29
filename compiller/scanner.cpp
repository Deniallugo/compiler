//
//  scanner.cpp
//  compiller
//
//  Created by Данил on 17.09.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//
#include "scanner.h"
static string get_out_addr(const string& in){
    string out = in;
    out.erase(out.find('.') + 1);
    out += "out";
    return out;
}

bool isNumber(char c){
    
    return (c >= '0' && c <= '9');
}

bool isSymbol( char c){
    
    return ((c >= 'a' && c <= 'z')||
            (c >= 'A' && c <= 'Z')
             );
}

static bool isOperation(char c){
    return  c == '<' ||
            c == '>' ||
            c == '=' ||
            c == '+' ||
            c == '-' ||
            c == '*' ||
            c == '/' ||
            c == '&' ||
            c == '%' ||
            c == '|' ||
            c == '^' ||
            c == '?' ||
            c == '!';
}

static bool isSeparation (char c){

    return ( c == '{' ||
             c == '}' ||
             c == '(' ||
             c == ')' ||
             c == '/' ||
             c == '[' ||
             c == ']' ||
             c == '.' ||
             c == ',' ||
             c == ';'
            );
}

static bool isSpace (char c){
    if ( c == '\t'){
        line++;
        col = 0;
        return true ;
    }
    else
    if( c ==' ' || c == '\n'  ){
        col++;
        return true;
    }
    return false;
}


bool isInteger( string s){
    return 0;
}

bool isFloat(){
    return 0;
    
}
Scanner::~Scanner(void){
    delete t;
}

bool Scanner :: isEnd(){
    return end_of_file;
}


void createKeyWords(){

    keyWords["for"] = "kwFor";
    keyWords["bool"] = "kwBool";
    keyWords["break"] = "kwBreak";
    keyWords["catch"] = " kwCatch";
    keyWords["char"] = "kwChar";
    keyWords["const"] = " kwConst";
    keyWords["continue"] = " kwContinue";
    keyWords["default"] = " kwDefault";
    keyWords["delete"] = " kwDelete";
    keyWords["do"] = " kwDo";
    keyWords["double"] = " kwDouble";
    keyWords["else"] = " kwElse";
    keyWords["enum"] = "kwEnum";
    keyWords["false"] = " kwFalse";
    keyWords["float"] = "kwFloat";
    keyWords["friend"] = "kwFriend ";
    keyWords["goto"] = "kwGoto";
    keyWords["if"] = "kwIf";
    keyWords["int"] = "integer number < 2147483647";
    keyWords["long"] = "integer number > 2147483647";
    keyWords["struct"] = "kwStruct";
    keyWords["this"] = "kwThis";
    keyWords["true"] = "kwTrue";
    keyWords["typedef"] = "kwTypedef";
    keyWords["typename"] = "typename";
    keyWords["return"] = " kwReturn";
    keyWords["void"] = "kwVoid";
    keyWords["while"] = "kwWhile";

}

void createOperations(){
    Operations["+"] = "plus";
    Operations["-"] = "minus";
    Operations["*"] = "mul";
    Operations["/"] = "div";
    Operations["++"] = "incr";
    Operations["--"] = "decr";
    Operations["<"] = "less";
    Operations[">"] = "more";
    Operations["=="] = "equal";
    Operations["<="] = "less or equal";
    Operations[">="] = "more or equal";
    Operations["+="] = "plus and assign";
    Operations["-="] = "minus and assign";
    Operations["&&"] = "logical and";
    Operations["||"] = "logical or";
    Operations["&"] = "byte and";
    Operations["|"] = "byte or";
    Operations["*="] = "mul and assign";
    Operations["/="] = "div and assign";
    Operations["!"] = "logical not";
    Operations["~"] = "byte not";
    Operations["!="] = "non equal";
    Operations["->"] = "arrow";


}



Token:: Token(string _sType, TYPES _type, string _Value, string _Text, int _num, int _line){
    sType = _sType;
    Type = _type;
    Value = _Value;
    Text = _Text;
    num = _num;
    line = _line;
};

void Token :: Print() const{
    cout << sType << string(13 - sType.length(), ' ') <<Value << line << "\t" << num << "\t" << Text << string(Text.length(), ' ')  << endl ;
}

void Token :: Print(ofstream *t) const{
    *t  << sType << "\t" <<Value << "\t" <<line << "\t" << num << "\t" << Text << string(Text.length(), ' ')  << endl ;
}

void MyException :: Print(ofstream *f) const{
    if(line != -1)
        *f << massage << "\t line " << line << ", column " << col << endl;
    else
        *f << massage << endl;
}

void MyException :: Print() const{
    if(line != -1)
        cout << massage << "\t line " << line << ", column " << col << endl;
    else
        cout << massage << endl;
}

Token* Scanner :: Get(){
    return t;
}

Scanner:: Scanner(string s){
    
    f.open(s);
    
}

bool Scanner::Next(){
    if(last_token)
        return end_of_file = true;
    States cas = BEGIN;
    char ch;
    bool point = false;
    if(f.eof() && (buf == 1)){
        cas = END;
        end_of_file = true;
        throw MyException("File is empty");;
    }
    if ( buf == 1){
        f.get(buf);
        col++;
    }

    string s = "";

    ch = buf;
    bool success  = false;
    while(!success)
    switch (cas) {
        case BEGIN:
            if(isNumber(ch)){
                cas = NUMBER;
               // s+=ch;
                break;
            }
            else if(isSymbol(ch)){
                cas = SYMBOL;
               // s+=ch;
                break;
            }
            
            else if(ch == '\"'){
                cas = STRINGT;
                break;
            } else if(ch == '\''){
                cas = CHARs;
                break;
            }
            else if (isSeparation(ch)) {
                if( ch == '.'){
                    f.get(buf);
                    col++;
                    if (isNumber(buf)){
                        point = true;
                        cas = NUMBER;
                    }
                    break;
                }
                cas = SEPARATION;
              //  s+= ch;
                break;
            }
            else if (isOperation(ch)){
                f.get(buf) ;
                s += ch;
                if (isOperation(buf))
                    s += ch;
                col++;
                t = new Token("Operation",_OPERATION, s, Operations[s], col , line);
                success = true;
                buf = 1;
                break;
            }

            else if (isSpace(ch)){
                success = true;
                buf = 1;
                break;
            }
        case NUMBER:
            if(!point){
                while( isNumber(ch) && !f.eof()){
                    col++;
                    s += ch;
                    f.get(ch);
                }
                if (ch == '.' && !point && !f.eof()){
                    s += ch;
                    point = true;
                    break;
                }

                if(f.eof()){
                    if(point)
                        t = new Token("float",_FLOAT, s, "",col, line );
                    else
                       t = new Token("integer",_INTEGER, s, "",col, line );
                    cas = END;
                    break;
                }

                f.get(buf);
                if( isNumber(buf)){
                    col += 2;
                    s += ch;
                    s += buf;
                    while (isNumber(ch)) {
                        col++;
                        s += ch;

                    }
                }

            if ( isSpace(ch) || isSeparation(ch)){
                t = new Token("integer", _INTEGER, s, "",col, line );
                success = true;
                buf = ch;
                break;
            }

            else  throw MyException("wrong identifier",line,col);
    }

            else{
                f.get(ch);
                while( isNumber(ch)){
                    col++;
                    s +=ch;
                    f.get(ch);

                }
                if ( isSpace(ch) || isSeparation(ch)){
                    col++;
                    t = new Token("float", _FLOAT, s ,"", col, line );
                    f.eof() ? cas = END :success = true;
                    buf = ch;
                    break;
                }
                else
                    throw MyException("wrong identifier",line,col);

                
                break;
            }

        case SYMBOL:
            while( isSymbol(ch)){

                s += ch;
                f.get(ch);

            }

            if ( keyWords.count(s) ){
                col++;
                t = new Token("keyword", _KEYWORD, s, keyWords[s], col, line);
                cas = BEGIN;
                break;
            }

            while (isNumber(ch) || isSymbol(ch)) {
                    col++;

                    s += ch;
                f.get(ch);
                }
                 buf = ch;
            t = new Token("identifer", _IDENTIFIER, s, "", col, line);
              f.eof() ? cas = END :success = true;
            break;
            
        case SEPARATION:
            f.get(buf);
            col++;
            if (  (buf =='/' || buf == '*' )&& ch =='/' ){
                cas = COMMENT;
                break;
            }
            s += ch;
            f.eof() ? cas = END :success = true;
            t = new Token("separation", _SEPARATION, s, Separations[s], col ,line);
            break;
        case STRINGT:
            f.get(ch);
            f.get(buf);
            col+=2;

            while(((buf != '\"') && (buf != '\n') && (!f.eof()) )||
                  ((ch == 92) && (buf == '\"') && (buf != '\n') && (!f.eof()))){
                s += ch;
                ch = buf;
                f.get(buf);
                col++;
            }

            if(buf == '\"'){
                s += ch;
                f.get(buf);
                col++;
            } else
                throw MyException("\n There is no closing quote", line, col);

             f.eof() ? cas  = END : success = true;
            t = new Token("string", _STRING,"\""+ s +"\"",s,line, col);

            break;
        case COMMENT:
            if( buf == '/'){
               while (ch != '\n') {
                   col++;
                   f.get(ch);

               }
                t = new Token("comment", _COMMENT, s, "not usable text", col, line);
            }
            else{
                while ((ch != '*' || buf != '/') && !f.eof()){
                    col += 2;
                    if ( ch =='\n'){
                        col = 0;
                        line++;
                    }
                    f.get(ch);
                    f.get(buf);
                }
                if( ch != '*' && buf != '/' && f.eof()){
                    throw new MyException("There is no closing comment");
                }
                t = new Token("Multiline comment", _COMMENT, s, "not usable text", col, line);
            }
            f.get(ch);
            buf = 1;
            f.eof() ?cas = END : success = true;

            break;

        case CHARs:
            f.get(ch);
            col++;
            if (ch == '\'')
                throw MyException("\n There is no character in single quotes", line, col);
            if (ch == '\\'){
                success = true;
                f.get(ch);
                col++;
                if (ch  == 't')
                    s += '\t';
                else if (ch == 'n')
                    s += '\n';
                else
                    s += ch;
                f.get(ch);
                col++;
                if (ch != '\'' || f.eof()){
                    if(f.eof())
                        s = "\n Unexpected end of file";
                    else
                        s = "\n There is no closing quote";
                    throw MyException(s, line, col);
                } else {
                    t = new Token("char", _CHAR,s,s,line, col);
                    col ++;
                    f.get(buf);
                    if (f.eof())
                        cas= END;
                    break;
                }
            }
            s += ch;
            f.get(ch);
            col++;
            if (ch != '\'')
                throw MyException( "\n Too many characters in quotes", line, col);
            else {
                t = new Token("char", _CHAR,s,s,line, col);
                col ++;
                f.get(buf);
                f.eof() ?cas = END : success = true;
                break;
            }
        case END:
            end_of_file = true;
            return last_token = true;
        default:
            break;


    }

        return end_of_file ;
    }
    



int main(int argc, char *argv[]){
    ofstream *output = 0;
    createKeyWords();
    createOperations();
    argv[1] = "-t";
    argv[2] = "05.txt";

    try{

            if(!(strcmp(argv[1] , "-help"))){
                cout << "-f - write to the file + output to the screen (lexer)" << endl <<
                "-s - output to the screen (lexel)" << endl <<
                "-t - write to the file (lexer)" << endl ;

            }
        if (!(strcmp(argv[1], "-f"))) {
            try{
                    ifstream *f = new ifstream;
                    f->open(argv[2], ios :: in);
                    ofstream *g = new ofstream;
                    output = g;
                    g->open(get_out_addr(argv[2]), ios :: out);
                    Scanner a(argv[2]);

                    while(!a.isEnd()){
                        a.Next();
                        a.Get()->Print(g);
                        a.Get()->Print();
                    }
                    g->close();
                    delete g;
                    f->close();
                    delete f;

            }
            catch(MyException &e){
                e.Print();
                e.Print(output);
                output->close();
            }
            
        }else
            if(!(strcmp(argv[1] , "-t"))){
                try{
                        ifstream *f = new ifstream;
                        f->open(argv[2], ios :: in);
                        ofstream *g = new ofstream;
                        output = g;
                        g->open(get_out_addr(argv[2]), ios ::out);
                        Scanner a(argv[2]);
                        while(!a.isEnd()){
                            a.Next();
                            a.Get()->Print(g);
                        }
                        g->close();
                        delete g;
                        
                        f->close();
                        delete f;
                }catch(MyException &e){
                    e.Print(output);
                    output->close();
                }
            }

    }catch(MyException &e){
            e.Print();
            if(output){
                e.Print(output);
                output->close();
            }
        }
        
        return 0;
}
