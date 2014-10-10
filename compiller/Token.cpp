//
//  Token.cpp
//  compiller
//
//  Created by Данил on 09.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "Token.h"



Token::Token(string _sType, TYPES _type, string _Value, string _Text, int _num, int _line){
    sType = _sType;
    Type = _type;
    Value = _Value;
    Text = _Text;
    num = _num;
    line = _line;
};

void Token::Print() const{
    cout << sType << string(13 - sType.length(), ' ') << Value <<"\t line:"<< line << "\t col:" << num << "\t" << Text << string(Text.length(), ' ') << endl;
}

void Token::Print(ofstream *t) const{
    *t << sType << string(13 - sType.length(), ' ') << Value <<"\t line:"<< line << "\t col:" << num << "\t" << Text << string(Text.length(), ' ') << endl;
}
/*
bool Token::operator == (Token* t){
    return (t->Type == Type && Value == t->Value);
}
*/