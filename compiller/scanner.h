//
//  scanner.h
//  compiller
//
//  Created by Данил on 17.09.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#pragma once


#include "Token.h"
#include "MyException.h"

using namespace std;




 void createKeyWords();

 void createOperations();


class Scanner {
private:
    
    ifstream f;
    Token* t;
    char buf = '#';
    bool end_of_file, last_token;
    int col = 0, line = 1;
    string open;
public:
    static bool isSeparation(char c);
    bool isSpace( char c);
    static bool isOperation( char c);
    static bool isNumber (char c);
    static bool isSymbol (char c);
    bool ErrorIf(bool op, string message);
    void CheckEscape(char ch, string s);
    bool isEnd();
    Token* Get();
    bool Next();
    Scanner(string s);
    Scanner(Scanner const &scan);
    ~Scanner(void);
};



/*
static bool isOperation(char s);
static bool isSeparation(char s);
static bool isSpace(char s);
static bool isNumber(char s);
static bool isSymbol(char s);*/
