//
//  Token.h
//  compiller
//
//  Created by Данил on 09.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#ifndef __compiller__Token__
#define __compiller__Token__

#include <stdio.h>
#include <map>
#include <iostream>
#include <fstream>
#include <string>



enum TYPES {
    _TYPEDEF,
    _INTEGER,
    _FLOAT,
    _STRING,
    _OPERATION,
    _SEPARATION,
    _WHITESPACE,
    _CHAR,
    _COMMENT,
    _IDENTIFIER,
    _KEYWORD,
    _END_OF_FILE,
    _VOID,
    _ERROR,
    _STRUCT,
    _CONST

};

using namespace::std;
class Token{
public:
    string sType;
    TYPES Type;
    string Value;
    string Text;
    int num, line;
    Token(string _sType, TYPES _type, string _Value, string _Text, int _num, int _line);\
    Token(){}
    void Print() const;
    void Print(ofstream*) const;
    bool operator == (string v) {return v == Value;}
    bool operator != (string v) {return v != Value;}
    bool operator == (Token* t) ;
    bool operator != (Token t) ;

};

#endif /* defined(__compiller__Token__) */
