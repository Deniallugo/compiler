//
//  scanner.h
//  compiller
//
//  Created by Данил on 17.09.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>


using namespace std;


enum TYPES {
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
    _END_OF_FILE
    
    
};



 void createKeyWords();

 void createOperations();

class Token{
public:
    string sType;
    TYPES Type;
    string Value;
    string Text;
    int num, line;
    Token(string _sType, TYPES _type, string _Value, string _Text, int _num, int _line);
    void Print() const;
    void Print(ofstream*) const;
    virtual bool operator == (string v) {return v == Value;}
    virtual bool operator != (string v) {return v != Value;}
    bool operator == (TYPES t) {return t == Type;}
    bool operator != (TYPES t) {return t != Type;}

};


class Scanner {
private:
    
    ifstream f;
    Token* t;
    char buf = 1;
    bool end_of_file, last_token;
    int col = 0, line = 1;
public:
    static bool isSeparation(char c);
    bool isSpace( char c);
    static bool isOperation( char c);
    static bool isNumber (char c);
    static bool isSymbol (char c);
    bool isEnd();
    Token* Get();
    bool Next();
    Scanner(string s);
    ~Scanner(void);
};


class MyException{
private:
    int line, col;
    string massage;

public:
    MyException(const string &m, int l = -1, int c = -1) : line(l), col(c), massage(m) {}
    MyException(const string &m, Token *token) : line(token->line), col(token->num), massage(m) {}
    void Print() const;
    void Print(ofstream *f) const;
};


/*
static bool isOperation(char s);
static bool isSeparation(char s);
static bool isSpace(char s);
static bool isNumber(char s);
static bool isSymbol(char s);*/
