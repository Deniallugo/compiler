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

class Parser{
    Parser ( Scanner scan){
        scan.Next();
    }
};
class SyntaxNode{

};

class ExprNode : public SyntaxNode{


};

class BinOpNode : public ExprNode{

    ExprNode* ParseExpr();
    ExprNode* ParseTerm();
    ExprNode* ParseFactor();

};

class ConstNode : public ExprNode{

};

class VarNode : public ExprNode{

};

ExprNode* ParseExpr(){

    auto result = ParseTerm();
    auto op  = scan  -> get();
    if ( op == tokPlus || op == tokMinus){
        Scan-> next();
        result = new BinOpNode( op, res, ParseExp() );
    }
    return result
}

ExprNode* ParseTerm(){

    auto result = ParseFactor();
    auto op  = scan  -> get();
    if ( op == tokPlus || op == tokMinus){
        Scan-> next();
        result = new BinOpNode( op, res, ParseTerm() );
    }
    return result
}

ExprNode* ParseFactor(){
    auto tok = scan -> get();
    if(tok == const){
        scan->next();
        return new ConstNode(tok->value);
        if ( tok == tokIdent){
            scan->next();
            return new VarNode( tok -> strValue);
        }

        if( tok == tokLparen){				\\tok l paren  ==  "("
            scan->next();
            auto result = parseExpr();
            if(scan->get != forRparen)
                throw Error()
                scan -> Next();
            return result
        }
        
        throw SyntaxError
    }
