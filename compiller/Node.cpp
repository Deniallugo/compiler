//
//  Node.cpp
//  compiller
//
//  Created by Данил on 10.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "Node.h"

OpNode :: OpNode(Token *op) : ExprNode(op) {}
ExprNode :: ExprNode(Token *t) : token(t) {}

string OpNode :: opName() const{
    return token->Value;
}

BinOpNode :: BinOpNode(Token *t, ExprNode *l, ExprNode *r) : OpNode(t), left(l), right(r) {
    if((l == 0) || (r == 0))
        throw MyException("Lost operand", t->line, t->num);
}

void BinOpNode :: print(int deep) const{
    left->print(deep + 1);
    cout << string(deep * 2, ' ') << opName() << endl;
    right->print(deep + 1);
}


void BinOpNode :: print(ofstream *f, int deep){
    left->print(f, deep + 1);
    *f << string(deep * 2, ' ') << opName() << endl;
    right->print(deep + 1);
}


UnOpNode :: UnOpNode(Token *t, ExprNode *n) : OpNode(t), operand(n){};

void UnOpNode :: print(int deep) const{
    cout << string(deep * 2, ' ') << opName() << endl;
    operand->print(deep + 1);
}

void UnOpNode :: print(ofstream *f, int deep) const{
    *f << string(deep * 2, ' ') << opName() << endl;
    operand->print(f, deep + 1);
}
void ConstNode :: print(int deep) const{
    cout << string(deep * 2, ' ') << value << endl;
}
void VarNode :: print(int deep) const{
    cout << string(deep * 2, ' ') << name << endl;
}
/*
bool UnOpNode :: isLvalue() const{
    Values operation = token->Value;
    return operation == factor || (operation == incr || operation == decr) && operand->isLvalue();
}


*/