
//
//  scanner.cpp
//  compiller
//
//  Created by Данил on 17.09.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//
#include "scanner.h"

static map< string, string> Operations;
static map< string, string> Separations;



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

void Scanner::CheckEscape(char ch , string s){

    if ( ch == 'e'){
        f.get(buf);
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
}

bool Scanner::ErrorIf(bool op, string message){
    if (op)
    throw MyException( message, line, col);
    return op;

}

bool Scanner::isNumber(char c){

    return (c >= '0' && c <= '9');
}

bool Scanner::isSymbol(char c){

    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_'
            );
}

bool Scanner::isOperation(char c){
    return  c == '<' ||
    c == '>' ||
    c == '=' ||
    c == '+' ||
    c == '-' ||
    c == '*' ||
    c == '~' ||
    c == '/' ||
    c == '&' ||
    c == '%' ||
    c == '|' ||
    c == '.' ||
    c == '^' ||
    c == ':' ||
    c == '?' ||
    c == '!';
}

bool  Scanner::isSeparation(char c){

    return (c == '{' ||
            c == '}' ||
            c == '(' ||
            c == ')' ||
            c == '[' ||
            c == ']' ||
            c == ',' ||
            c == ';'
            );
}

bool Scanner::isSpace(char c){
    if (c == '\n' || c == '\r'){
        line++;
        col = 0;
        return true;
    }
    else
    if (c == ' ' || c == '\t'){
        col++;
        return true;
    }
    return false;
}

Token* Scanner::GetNext(){
    Next();
    return Get();
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
    keyWords["continue"] = " kwContinue";
    keyWords["default"] = " kwDefault";
    keyWords["delete"] = " kwDelete";
    keyWords["do"] = " kwDo";
  //  keyWords["double"] = " kwDouble";
    keyWords["else"] = " kwElse";
    keyWords["enum"] = "kwEnum";
    keyWords["false"] = " kwFalse";
    keyWords["float"] = "kwFloat";
    keyWords["friend"] = "kwFriend ";
    keyWords["goto"] = "kwGoto";
    keyWords["if"] = "kwIf";
    keyWords["int"] = "kwInteger number ";
    keyWords["long"] = "kwLong";
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
    Operations["."] = "point";


}

Token* Scanner::Get(){
    return t;
}

Scanner::Scanner(string s){

    f.open(s);
    open = s;

}
Scanner::Scanner(Scanner const &scan){
    open = scan.open;
    f.open(open);

}

bool Scanner::Next(){
    if (last_token){
        //   t = new Token("End", _END_OF_FILE,"end","end", line, col);

        return end_of_file = true;
        
    }
    States cas = BEGIN;
    char ch;
    bool point = false;
    if (f.eof() && (buf == '#')){

        cas = END;

        end_of_file = true;

        throw MyException("File is empty");

    }
    if (buf == '#'){
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
            else if ( ch == '/'){
                f.get(buf);
                if ( buf == '/' || buf == '*'){
                    cas = COMMENT;
                    break;
                }
                if (isNumber(buf) || buf == '.' || buf == '(' || buf == '-' || isSymbol(buf) || isSpace(buf)
                    || buf == '='
                    ){
                    cas = OPERATION;
                    break;
                }

            }
            else if (isSeparation(ch)) {

                cas = SEPARATION;
                break;
            }
            else if (isOperation(ch)){
                cas = OPERATION;
                f.get(buf);
                break;
            }

            if (isSpace(ch)){
                f.get(ch);
                if(f.eof()){
                    cas = END ;
                    t = new Token("End", _END_OF_FILE, "", "", col, line);
                    break;
                }

                break;
            }
            throw MyException("\n There are no symbols in file or unknown symbol");
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

                    f.eof() ? cas = END : success = true;
                    break;

                }

                CheckEscape(ch, s);
                f.eof() ? cas = END : success = true;
                if (ch != '.'){

                    if (!ErrorIf(!isOperation(ch)&&!isSpace(ch) && !isSeparation(ch) && !f.eof(),  "wrong number")){
                        t = new Token("integer", _INTEGER, s, s, col, line);
                        f.eof() ? cas = END : success = true;
                        if (isSeparation(ch) || isOperation(ch))
                        buf = ch;
                        else
                        buf = '#';
                        break;
                    }

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

                CheckEscape(ch, s);
                f.eof() ? cas = END : success = true;

                buf = ch;
                ErrorIf(ch == '.' ,"wrong number");

                ErrorIf(!isSpace(ch) && !isSeparation(ch) && !f.eof(),  "wrong number");
                col++;
                t = new Token("float", _FLOAT, s, s, col, line);
                f.eof() ? cas = END : success = true;
                if (isSeparation(ch))
                buf = ch;



                break;
            }

            case SYMBOL:

            do{
                col++;
                s += ch;
                f.get(ch);

            } while ((isSymbol(ch) ||isNumber(ch)) && !f.eof());


            if ( s == "struct"){
                t = new Token("struct", _STRUCT, s, keyWords[s], col, line);
                buf = ch;
                f.eof() ? cas = END : success = true;

                break;
            }
            if ( s == "const"){


                t = new Token("const", _CONST, s, keyWords[s], col, line);
                buf = ch;
                f.eof() ? cas = END : success = true;

                break;
            }

            if (keyWords.count(s)){
                col++;
                t = new Token("keyword", _KEYWORD, s, keyWords[s], col, line);
                buf = ch;
                // f.get(buf);
                f.eof() ? cas = END : success = true;

                break;
            }


            buf = ch;
            t = new Token("identifer", _IDENTIFIER, s, "identifier :" + s, col, line);
            f.eof() ? cas = END : success = true;
            break;

            case SEPARATION:

            col++;
            if ((buf == '/' || buf == '*') && ch == '/'){
                cas = COMMENT;
                break;
            }
            s += ch;
            t = new Token("separation", _SEPARATION, s, Separations[s], col, line);
            f.get(buf);
            f.eof() ? cas = END : success = true;

            break;
            case OPERATION:
            if (ch == '.'){

                if (isNumber(buf)){
                    point = true;
                    cas = NUMBER;
                    break;
                }
                s+=ch;
                t = new Token("Operation", _OPERATION, s, Operations[s], col, line);
                f.eof() ? cas = END : success = true;


                break;
            }


            s += ch;
            if (!f.eof()){
                if (isOperation(buf) && ch != '*'){
                    s += buf;
                    f.get(buf);
                }
                col++;
            }
            t = new Token("Operation", _OPERATION, s, Operations[s], col, line);
            f.eof() ? cas = END : success = true;

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

            if (! ErrorIf(buf != '\"', "There is no closing quote")){
                s += ch;
                f.get(buf);
                col++;
            }

            f.get(buf);
            t = new Token("string", _STRING, "\"" + s + "\"", s, col, line);
            f.eof() ? cas = END : success = true;
            break;

            case COMMENT:
            if (buf == '/'){
                while (ch != '\n' && !f.eof()) {
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
                    if (ch == '*'){
                        f.get(buf);
                        if (buf != '/') {
                            ch = buf;
                        }
                    }
                }
                ErrorIf(ch == '/' && buf == '*' && !f.eof(), "There is no closing comment");
                t = new Token("multi_comment", _COMMENT, s, "not usable text", col, line);

            }
            f.get(ch);
            buf = '#';
            f.eof() ? cas = END : success = true;

            break;


            case CHARs:

            f.get(ch);
            col++;
            ErrorIf (ch == '\''," There is no character in single quotes");
            if (ch == '\n' || f.eof()){


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
                string ms;
                if (f.eof())
                ms = " Unexpected end of file";
                else
                ms = " There is no closing quote";
                ErrorIf (ch != '\'' || f.eof(), ms);;;;;;;;;;;;



            }
            s += ch;
            f.get(ch);
            col++;
            ErrorIf (ch != '\'', "Too many characters in quotes");
            t = new Token("char", _CHAR, s, s, line, col);
            col++;
            f.get(buf);
            f.eof() ? cas = END : success = true;
            break;


            case END:
            end_of_file = true;
            return last_token = true;
        default:
            break;

    }
    return end_of_file;
}



