//
//  Parser.cpp
//  compiller
//
//  Created by Данил on 03.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "Parser.h"


static bool isConst( Token t){
    return (t.Type == _FLOAT || t.Type == _INTEGER);
}

static bool isVar( Token t){
    
    return (t.Type == _IDENTIFIER);
}

static bool isEq(Token* t, TYPES _type,  string _value){
    return (t->Type == _type && t->Value == _value);
}

static bool isAssing(Token* op){
    return (isEq(op, _OPERATION, "=")||
            isEq(op, _OPERATION, "+=")||
            isEq(op, _OPERATION, "-=")||
            isEq(op, _OPERATION, "*=")||
            isEq(op, _OPERATION, "/=")||
            isEq(op, _OPERATION, "%=")||
            isEq(op, _OPERATION, "|=")||
            isEq(op, _OPERATION, "&=")||
            isEq(op, _OPERATION, "^=")
            );
}

static bool isUnary(Token* op){
    return (isEq(op, _OPERATION, "+")||
            isEq(op, _OPERATION, "-")||
            isEq(op, _OPERATION, "&")||
            isEq(op, _OPERATION, "*")||
            isEq(op, _OPERATION, "!")||
            isEq(op, _OPERATION, "~")
            );
}


static bool findPriority(Token * op, int priority) {
    switch (priority) {
        case 0: return isEq(op, _OPERATION, "||"); break;
        case 1: return isEq(op, _OPERATION, "&&"); break;
        case 2: return isEq(op, _OPERATION, "|"); break;
        case 3: return isEq(op, _OPERATION, "^"); break;
        case 4: return isEq(op, _OPERATION, "&"); break;
        case 5: return isEq(op, _OPERATION, "==") || isEq(op, _OPERATION, "!="); break;
        case 6: return isEq(op, _OPERATION, ">") || isEq(op, _OPERATION, "<") || isEq(op, _OPERATION, ">=") || isEq(op, _OPERATION,"<="); break;
        case 7: return  isEq(op, _OPERATION,"<<") ||
            isEq(op, _OPERATION,">>"); break;
        case 8: return isEq(op, _OPERATION, "+") || isEq(op, _OPERATION, "-");  break;
        case 9: return isEq(op, _OPERATION, "*") || isEq(op, _OPERATION,"/") || isEq(op, _OPERATION, "%"); break;
    }
    return false;
}





ExprNode* Parser::ParseExpr(){

    auto result = ParseAssing();
    auto op  = scan.Get();

    if (isEq(op, _OPERATION, ",")){
        scan.Next();
        result = new BinOpNode( op, result, ParseExpr() );
    }
//    if ( isUnary(*op)){
  //      scan.Next();
    //    return new UnOpNode(op,result);
    //}
        return result;

}


ExprNode* Parser::ParseAssing(){

    auto result = ParseBinaryPriority(0);
    auto op  = scan.Get();

    if ( isAssing(op)){
        scan.Next();
        result = new BinOpNode( op, result, ParseAssing());
    }

    return result;
}
ExprNode* Parser::ParseBinaryPriority(int priority){


    if (priority == 9) {
        auto result = ParseUnary();
    }
    auto result = ParseBinaryPriority(priority + 1);
    auto op  = scan.Get();

    while (findPriority(op, priority)){
        scan.Next();
        if (priority == 9)
            result = new BinOpNode( op, result, ParseAssing());
        else result = new BinOpNode( op, result, ParseBinaryPriority(priority + 1));
        op = scan.Get();

    }
    return result;
}


ExprNode* Parser::ParseUnary(){

    auto op = scan.Get();

    if  (isEq( op, _OPERATION, "++") || isEq(op, _OPERATION, "--")){
        scan.Next();
        return new UnOpNode(op,ParseUnary());


    }
    if (isUnary(op)){
        scan.Next();
        return new UnOpNode(op,ParseUnary());
    }
    if (keyWords.count(op->Value)){
        scan.Next();
        op
    }
        throw MyException("All bad");
}