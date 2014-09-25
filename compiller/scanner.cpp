//
//  scanner.cpp
//  compiller
//
//  Created by Данил on 17.09.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "scanner.h"


bool isNumber(char s){
    
    return (s >= '0' && s <= '9');
}

bool isSymbol( char s){
    
    return ( (s >= 'a' && s <= 'z')||
             (s >= 'A' && s <= 'Z'));
}
/*
static bool isOperation(char s){
    return  s == '<' ||
            s == '>' ||
            s == '=' ||
            s == '+' ||
            s == '-' ||
            s == '*' ||
            s == '/' ||
            s == '&' ||
            s == '%' ||
            s == '|' ||
            s == '^' ||
            s == '?' ||
            s == '!';
}*/

static bool isSeparation (char s){

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

static bool isSpace (char s){
    if ( s == '\t'){
        line++;
        return true ;
    }
    
    return ( s ==' '   ||
             s == '\n'
            );
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

Scanner:: Scanner(string s){
    
    f.open(s);
    
}

bool Scanner::Next(){
    if(last_token)
        return end_of_file = true;
    States cas = BEGIN;
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
            if ( keyWords.count(s) ){
                col++;
                t = new Token(_KEYWORD, s, keyWords[s], col, line);
                break;
            }
            f>>ch;
            while (isNumber(ch) || isSymbol(ch)) {
                    
                    f >> ch;
                    s += ch;
                }
                
            t = new Token(_IDENTIFIER, s, "", col, line);
            break;
            
        case SEPARATION:
            f >> buf;
            if (  buf =='/' && ch =='/' ){
                while (ch != '\n') {
                    f >> ch;
                    s += ch;
                }
                col++;
                t = new Token(_COMMENT, s, "not usable text", col, line);
            }
            col ++;

            t = new Token(_SEPARATION, s, Separations[s], col ,line);
            break;
        case STRINGT:
            f >> ch;
            s += ch;
            while (ch != '\"'){
                f >> ch;
                s += ch;

            }
            col ++;
             t = new Token(_STRING, s, "", col ,line);
        case CHARs:
            f >> ch;
            s += ch;
            while (ch != '\''){
                f >> ch;
                s += ch;

            }
            col ++;
            t = new Token(_CHAR, s, "", col ,line);
        default:
            break;
    }
    if ( last_token){
            return end_of_file = true;
    }
        return end_of_file = false;
}
