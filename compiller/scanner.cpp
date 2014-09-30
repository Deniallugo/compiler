//
//  scanner.cpp
//  compiller
//
//  Created by Данил on 17.09.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//
#include "scanner.h"


static map< string, string> keyWords;
static map< string, string> Operations;
static map< string, string> Separations;

static int col = 0;
static int line = 0;


enum States{
    BEGIN,
    NUMBER,
    OPERATION,
    SEPARATION,
    WHITESPACE,
    CHARs,
    STRINGT,
    SYMBOL,
    OK,
    END,
    COMMENT

};


static bool isNumber(char c){

    return (c >= '0' && c <= '9');
}

static bool isSymbol(char c){

    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_'
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

static bool isSeparation(char c){

    return (c == '{' ||
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

static bool isSpace(char c){
    if (c == '\t'){
        line++;
        col = 0;
        return true;
    }
    else
        if (c == ' ' || c == '\n'){
            col++;
            return true;
        }
    return false;
}


bool isInteger(string s){
    return 0;
}

bool isFloat(){
    return 0;

}
Scanner::~Scanner(void){
    delete t;
}

bool Scanner::isEnd(){
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



Token::Token(string _sType, TYPES _type, string _Value, string _Text, int _num, int _line){
    sType = _sType;
    Type = _type;
    Value = _Value;
    Text = _Text;
    num = _num;
    line = _line;
};

void Token::Print() const{
    cout << sType << string(13 - sType.length(), ' ') << Value << line << "\t" << num << "\t" << Text << string(Text.length(), ' ') << endl;
}

void Token::Print(ofstream *t) const{
    *t << sType << "\t" << Value << "\t" << line << "\t" << num << "\t" << Text << string(Text.length(), ' ') << endl;
}

void MyException::Print(ofstream *f) const{
    if (line != -1)
        *f << massage << "\t line " << line << ", column " << col << endl;
    else
        *f << massage << endl;
}

void MyException::Print() const{
    if (line != -1)
        cout << massage << "\t line " << line << ", column " << col << endl;
    else
        cout << massage << endl;
}

Token* Scanner::Get(){
    return t;
}

Scanner::Scanner(string s){

    f.open(s);

}

bool Scanner::Next(){
    if (last_token)
        return end_of_file = true;
    States cas = BEGIN;
    char ch;
    bool point = false;
    if (f.eof() && (buf == 1)){

        cas = END;

        end_of_file = true;

        throw MyException("File is empty");;

    }
    if (buf == 1){
        f.get(buf);
        col++;
    }

    string s = "";

    ch = buf;
    bool success = false;
    while (!success)
        switch (cas) {
            case BEGIN:
                if (isNumber(ch)){
                    cas = NUMBER;
                    break;
                }
                else if (isSymbol(ch)){
                    cas = SYMBOL;
                    break;
                }

                else if (ch == '\"'){
                    cas = STRINGT;
                    break;
                }
                else if (ch == '\''){
                    cas = CHARs;
                    break;
                }
                else if (isSeparation(ch)) {
                    if (ch == '.'){
                        f.get(buf);
                        col++;
                        if (isNumber(buf)){
                            point = true;
                            cas = NUMBER;
                        }
                        break;
                    }
                    cas = SEPARATION;
                    break;
                }
                else if (isOperation(ch)){
                    f.get(buf);

                    s += ch;
                    if (!f.eof()){
                        if (isOperation(buf)){
                            s += buf;
                            f.get(ch);
                        }
                        col++;
                    }
                    t = new Token("Operation", _OPERATION, s, Operations[s], col, line);
                    f.eof() ? cas = END : success = true;
                    buf = 1;
                    break;

                }

                else if (isSpace(ch)){
                    success = true;
                    buf = 1;
                    break;
                }
            case NUMBER:
                if (!point){
                    while (isNumber(ch) && !f.eof()){
                        col++;
                        s += ch;
                        f.get(ch);
                    }
                    if (ch == '.' && !point && !f.eof()){
                        f.get(buf);
                        point = true;
                        break;
                    }

                    if (f.eof()){

                        if (point)

                            t = new Token("float", _FLOAT, s, s, col, line);

                        else

                            t = new Token("integer", _INTEGER, s, s, col, line);

                        cas = END;

                        break;

                    }

                    f.get(buf);
                    if (isNumber(buf)){
                        col += 2;
                        s += ch;
                        s += buf;
                        while (isNumber(ch)) {
                            col++;
                            s += ch;

                        }
                    }
                    f.get(buf);
                    if ( ch == 'e'){
                        if ( isNumber(buf) || buf == '+' || buf == '-'){
                            col += 2;
                            s +=ch;
                            s += buf;
                            f.get(ch);
                            while (isNumber(ch) && !f.eof() ) {
                                col++;
                                s += ch;
                                f.get(ch);
                            }
                             t = new Token("integer", _INTEGER, s, s, col, line);
                        }

                    }
                    if (ch != '.'){

                        if (isSpace(ch) || isSeparation(ch) || f.eof()){   //поправить
                            t = new Token("integer", _INTEGER, s, s, col, line);
                            success = true;
                            if (isSeparation(ch))
                                buf = ch;
                            break;
                        }

                        else  throw MyException("wrong identifier", line, col);


                    }
                }
                else{
                    s += ".";
                    ch = buf;
                    while (isNumber(ch) && !f.eof()){
                        col++;
                        s += ch;
                        f.get(ch);

                    }
                    f.get(buf);
                    if ( ch == 'e'){
                        if ( isNumber(buf) || buf == '+' || buf == '-'){
                            col += 2;
                            s +=ch;
                            s += buf;
                            f.get(ch);
                            while (isNumber(ch) && !f.eof() ) {
                                col++;
                                s += ch;
                                f.get(ch);
                            }
                            t = new Token("integer", _INTEGER, s, s, col, line);
                        }

                    }
                    if (ch == '.')
                        throw MyException("wrong identifier", line, col);
                    if (isSpace(ch) || isSeparation(ch) || f.eof()){
                        col++;
                        t = new Token("float", _FLOAT, s, s, col, line);
                        f.eof() ? cas = END : success = true;
                        if (isSeparation(ch))
                            buf = ch;
                        break;
                    }
                    else
                        throw MyException("wrong identifier", line, col);
                    break;
                }

            case SYMBOL:
                while (isSymbol(ch) && !f.eof()){

                    s += ch;
                    f.get(ch);

                }

                if (keyWords.count(s)){
                    col++;
                    t = new Token("keyword", _KEYWORD, s, keyWords[s], col, line);
                    cas = BEGIN;
                    break;
                }

                while ((isNumber(ch) || isSymbol(ch)) && !f.eof()) {
                    col++;

                    s += ch;
                    f.get(ch);
                }
                buf = ch;
                t = new Token("identifer", _IDENTIFIER, s, s, col, line);
                f.eof() ? cas = END : success = true;
                break;

            case SEPARATION:
                f.get(buf);
                col++;
                if ((buf == '/' || buf == '*') && ch == '/'){
                    cas = COMMENT;
                    break;
                }
                s += ch;
                f.eof() ? cas = END : success = true;
                t = new Token("separation", _SEPARATION, s, Separations[s], col, line);
                break;
            case STRINGT:

                f.get(ch);
                f.get(buf);
                col += 2;
                while (((buf != '\"') && (buf != '\n') && (!f.eof())) ||
                       ((ch == 92) && (buf == '\"') && (buf != '\n') && (!f.eof()))){
                    s += ch;
                    ch = buf;
                    f.get(buf);
                    col++;
                }

                if (buf == '\"'){
                    s += ch;
                    f.get(buf);
                    col++;
                }
                else
                    throw MyException("\n There is no closing quote", line, col);

                f.eof() ? cas = END : success = true;
                t = new Token("string", _STRING, "\"" + s + "\"", s, line, col);
                break;
                
            case COMMENT:
                if (buf == '/'){
                    while (ch != '\n') {
                        col++;
                        f.get(ch);
                        
                    }
                    t = new Token("comment", _COMMENT, s, "not usable text", col, line);
                }
                else{
                    while ((ch != '*' || buf != '/') && !f.eof()){
                        col += 2;
                        if (ch == '\n'){
                            col = 0;
                            line++;
                        }
                        f.get(ch);
                        f.get(buf);
                    }
                    if (ch != '*' && buf != '/' && f.eof()){
                        throw new MyException("There is no closing comment");
                    }
                    t = new Token("Multiline comment", _COMMENT, s, "not usable text", col, line);
                }
                f.get(ch);
                buf = 1;
                f.eof() ? cas = END : success = true;
                
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
                    if (ch == 't')
                        s += '\t';
                    else if (ch == 'n')
                        s += '\n';
                    else
                        s += ch;
                    f.get(ch);
                    col++;
                    if (ch != '\'' || f.eof()){
                        if (f.eof())
                            s = "\n Unexpected end of file";
                        else
                            s = "\n There is no closing quote";
                        
                        throw MyException(s, line, col);
                    }
                    else {
                        t = new Token("char", _CHAR, s, s, line, col);
                        col++;
                        f.get(buf);
                        if (f.eof())
                            cas = END;
                        break;
                    }
                }
                s += ch;
                f.get(ch);
                col++;
                if (ch != '\'')
                    throw MyException("\n Too many characters in quotes", line, col);
                
                else {
                    t = new Token("char", _CHAR, s, s, line, col);
                    col++;
                    f.get(buf);
                    f.eof() ? cas = END : success = true;
                    break;
                    
                }
            case END:
                end_of_file = true;
                return last_token = true;
            default:
                break;

        }
    return end_of_file;
}



