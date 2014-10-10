//
//  Node.h
//  compiller
//
//  Created by Данил on 10.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Token.h"
#include "scanner.h"



class ExprNode{
protected:
    Token *token;
public:
    friend class Parser;
    friend class BinaryOpNode;
    ExprNode(){}
    ExprNode(Token* t);
    virtual void print(int deep = 0) const {};
    virtual void print(ofstream *t, int deep = 0) const {}
    virtual bool isLvalue() const { return false; }
    virtual bool isModifiableLvalue() const { return false; }
};


class OpNode : public ExprNode{
protected:
    string opName() const;
public:
    OpNode(Token *op);
    virtual void print(int deep) const {}
    virtual void print(int deep, ofstream* f) const {}
};

class BinOpNode : public OpNode{
protected:
    ExprNode *left;
    ExprNode *right;

public:
    friend class Parser;
    BinOpNode(Token *op, ExprNode *l, ExprNode *r);
    void print(int deep) const;
    void print(ofstream *f, int deep);
};

class UnOpNode : public OpNode{
protected:
    ExprNode *operand;
public:
    UnOpNode(Token *t, ExprNode *op);
    void print(int deep) const;
    void print(ofstream *f, int deep) const;
    };

class ConstNode : public ExprNode{
public:
    string value;
    TYPES type;
    void print(int deep) const;
    ConstNode (string _value) {
        if (!_value.find('.'))
            type = _FLOAT;
         else
            type = _INTEGER;
        value = _value;
    }

};

class VarNode : public ExprNode{
public:
    string value;
    void print(int deep) const;
    VarNode(string _value){
        value = _value;
    }
};
