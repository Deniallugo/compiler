//
//  scanner.cpp
//  compiller
//
//  Created by Данил on 17.09.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "scanner.h"

enum _cases{
    BEGIN,
    NUMBER,
    OPERATION,
    SEPARATION,
    WHITESPACE,
    CHARs,
    STRINGT,
    SYMBOL,
    OK,
    END
    
};


bool isNumber(char s){
    
    return (s >= '0' && s <= '9');
}

bool isSymbol( char s){
    
    return ( (s >= 'a' && s <= 'z')||
             (s >= 'A' && s <= 'Z'));
}

bool isOperation( char s){
    
    return ( s == '*' ||
             s == '+' ||
             s == '-' ||
             s == '/'
            );
}

bool isSeparation (char s){
    
    return ( s == '{' ||
             s == '}' ||
             s == '(' ||
             s == ')' ||
             s == '/' ||
             s == '[' ||
             s == ']' ||
             s == '.' ||
             s == ','
            );
}

bool isSpace (char s){
    if ( s == '\t'){
        line++;
    }
    
    return ( s ==' '   ||
             s == '\t' ||
             s == '\n'
            );
}

bool isKeyWord( string s){
    return 0;
}

bool isInteger( string s){
    return 0;
}

bool isFloat(){
    return 0;
    
}

void createKeyWords(){
    insert( keyWords,"for","begin cycle");
    insert( keyWords,"bool", " type of numbers 0 and 1");
    insert( keyWords,"break", " stop coplete comand");
    insert( keyWords,"catch", " catch the erroe");
    insert( keyWords,"char", " character type");
    insert( keyWords,"const", " make variabels const");
    insert( keyWords,"continue"," continue command execution");
    insert( keyWords,"default", " default value");
    insert( keyWords,"delete", " delete the object of class");
    insert( keyWords,"do", " do comand");
    insert( keyWords,"double", " double precision of float number");
    insert( keyWords,"else", " alternative branch of if");
    insert( keyWords,"enum", "something type");
    insert( keyWords,"false", " false");
    insert( keyWords,"float", "number with floating point");
    insert( keyWords,"friend", "friend ");
    insert( keyWords,"goto", "go to the some point");
    insert( keyWords,"if", "branching");
    insert( keyWords,"int", "integer number < 2147483647");
    insert( keyWords,"long", "integer number > 2147483647");
    insert( keyWords,"struct", "create new struct of variables");
    insert( keyWords,"this", "use this object");
    insert( keyWords,"true", " true");
    insert( keyWords,"typedef", "define some type");
    insert( keyWords,"typename", "typename");
    insert( keyWords,"return", " return variable");
    insert( keyWords,"void", "void");
    insert( keyWords,"while", "while");
}




Token:: Token(TYPES _type, string _Value, string _Text, int _num, int _line){
    Type = _type;
    Value = _Value;
    Text = _Text;
    num = _num;
    line = _line;
};

Scanner::Scanner(string s){
    
    f.open(s);
    
}

bool Scanner::Next(){
    if(last_token)
        return end_of_file = true;
    _cases cas = BEGIN;
    char ch;
    bool point = false;
    string s = "";
    if ( buf == 1){
        f >> buf;
    }
    ch = buf;
    bool success  = false;
    while(!success)
    switch (cas) {
        case BEGIN:
            if(isNumber(ch)){
                cas = NUMBER;
                s+=ch;
            }
            else if(isSymbol(ch)){
                cas = SYMBOL;
                s+=ch;
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
                    f >> buf;
                    if (isNumber(buf)){
                        point = true;
                        cas = NUMBER;
                    }
                    break;
                }
                cas = SEPARATION;
                s+= ch;
                break;
            }
            else if (isSpace(ch)){
                success = true;
                break;
            }
        case NUMBER:
            if(!point){
                while( isNumber(ch)){
                    f >> ch;
                    s +=ch;
                }
                if (ch == '.'){
                    point = true;
                    break;
                }
                f>>ch;
                if ( isSpace(ch)){
                    col++;
                    t = new Token(_FLOAT, s, "",col, line );
                    break;
                }
                
                else{}
                    // throw exception(" all is bad");
                    }
            
            else{
                while( isNumber(ch)){
                    f >>ch;
                    s +=ch;
                }
                f>>ch;
                if ( isSpace(ch)){
                    col++;
                    t = new Token(_FLOAT, s, "", col, line );
                    break;
                }
                else
                   // throw exception(" all is bad");
                
                break;
            }
            case SYMBOL:
            while( isSymbol(ch)){
                f >> ch;
                s += ch;
                
            }
            f >> buf;// обратить внимание
            if (isNumber(buf)){
                while (isNumber(ch)) {
                    
                    f >> ch;
                    s += ch;
                }
                
                t = new Token(_IDENTIFIER, s, "", col, line);
            }
            else
            if ( keyWords.count(s) ){
                col++;
                t = new Token(_KEYWORD, s, keyWords[s], col, line);
            }
                       //добавить в лист с кодом identifier
            break;
            
        case SEPARATION:
            f >> buf;
            if (  buf =='/' && ch =='/' ){
                while (ch != '\n') {
                    f >> ch;
                    s += ch;
                }
                t = new Token(_COMMENT, s, "not usable text", col, line);
            }
            t = new Token(_SEPARATION, s, Separations[s], col ,line);
            break;
        default:
            break;
    }
    if ( last_token){
            return end_of_file = true;
    }
        return end_of_file = false;
}
    