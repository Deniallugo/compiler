//
//  scanner.h
//  compiller
//
//  Created by Данил on 17.09.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#ifndef __compiller__scanner__
#define __compiller__scanner__
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
    
    TYPES Type;
    string Value;
    string Text;
    int num, line;
    void Print();
    Token(TYPES _type, string _Value, string _Text, int _num, int _line);
};

class Scanner {
private:
    
    ifstream f;
    int line;
    int col;
    Token* t;
    char buf;
    bool end_of_file, last_token;
    
public:
    bool isEnd();
    Token* Get();
    bool Next();
    Scanner(string s);
    ~Scanner(void);
};

//TODO : СОЗДАТЬ КЛАССЫ С И СОЗДАТЬ ENUM 

#endif /* defined(__compiller__scanner__) */
