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
    ExprNode* ParseExpr();
    ExprNode* ParseTerm();
    ExprNode* ParseFactor();
    Scanner &scan;
    Token* plusTkn = new Token("Operation", _OPERATION, "+\t", "plus", 0, 0);
    Token* minusTkn = new Token("Operation", _OPERATION, "-\t", "minus", 0, 0);
    Token* lParTkn = new Token("Separation", _SEPARATION, "(\t", "", 0, 0);
    Token* RParTkn = new Token("Separation", _SEPARATION, ")\t", "", 0, 0);


    Parser ( Scanner &_scan):scan(_scan){
        scan.Next();
    }


};
