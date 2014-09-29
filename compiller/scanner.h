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


using namespace std;
map< string, string> keyWords;
map< string, string> Operations;
map< string, string> Separations;

int col = 0;
int line = 0;


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
    _KEYWORD
    
    
};
void insert(map<string, string> m, string s1, string s2){
   m.insert(make_pair(s1, s2));
    
}
void createKeyWords();


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
    int line;
    int col; // исправить колумн на колонку переделать 
    Token* t;
    char buf = 1;
    bool end_of_file, last_token;
public:
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
