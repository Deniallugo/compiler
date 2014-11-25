//
//  MyException.h
//  compiller
//
//  Created by Данил on 09.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#ifndef __compiller__MyException__
#define __compiller__MyException__

#include "Token.h"

class MyException{
private:
    int line, col;
    string massage;

public:
    MyException(const string &m, int l , int c ) : line(l), col(c), massage(m) {}
    MyException(const string &m, Token *token) : line(token->line), col(token->num), massage(m) {}
    MyException(const string &m) : line(-1), col(-1), massage(m) {}

    void Print() const;
    void Print(ofstream *f) const;
};


#endif /* defined(__compiller__MyException__) */
