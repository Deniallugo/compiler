//
//  Parser.h
//  compiller
//
//  Created by Данил on 03.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//


#pragma once
#include <stdio.h>
#include "scanner.h"
#include "Node.h"



class Parser{
    Scanner &scan;
    Token* plusTkn = new Token("Operation", _OPERATION, "+", "plus", 0, 0);
    Token* mulTkn = new Token("Operation", _OPERATION, "*", "mul", 0, 0);
    Token* divTkn = new Token("Operation", _OPERATION, "/", "div", 0, 0);
    Token* minusTkn = new Token("Operation", _OPERATION, "-", "minus", 0, 0);
    Token* lParTkn = new Token("Separation", _SEPARATION, "(", "", 0, 0);
    Token* RParTkn = new Token("Separation", _SEPARATION, ")", "", 0, 0);
public:
    Parser ( Scanner &_scan):scan(_scan){
        scan.Next();
    }
    ExprNode* ParseExpr();
    ExprNode* ParseTerm();
    ExprNode* ParseFactor();


};
