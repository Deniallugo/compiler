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

    ExprNode* ParseExpr();
    ExprNode* ParseAssing();
    ExprNode* ParseCond();
    ExprNode* ParseBinaryPriority(int priority);
    ExprNode* findPriority (Token* op, int priority);
    ExprNode* ParseCasr();
    ExprNode* ParseUnary();
    ExprNode* ParsePostfix();
    void ParseArgList();
    ExprNode* ParsePrimary();
    ExprNode* ParseCast();

public:
    Parser ( Scanner &_scan):scan(_scan){
        scan.Next();
    }

};
