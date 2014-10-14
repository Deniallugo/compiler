//
//  Parser.cpp
//  compiller
//
//  Created by Данил on 03.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "Parser.h"


bool isConst( Token t){
    return (t.Type == _FLOAT || t.Type == _INTEGER);
}

bool isVar( Token t){
    
    return (t.Type == _IDENTIFIER);
}

bool isEq(Token t, Token t1){
    return (t.Type == t1.Type && t.Value == t1.Value);
}

ExprNode* Parser::ParseExpr(){

    auto result = ParseTerm();

    auto op  = scan.Get();

    if (isEq(*op, *plusTkn) || isEq(*op, *minusTkn)){
        scan.Next();
        result = new BinOpNode( op, result, ParseExpr() );
    }
    if ( isUnary(*op)){
        scan.Next();
        return new UnOpNode(op,result);
    }
        return result;

}


ExprNode* Parser::ParseTerm(){

    auto result = ParseFactor();

    auto op  = scan.Get();
    if ( isEq(*op, *mulTkn) || isEq(*op, *divTkn)){
        scan.Next();
        result = new BinOpNode( op, result, ParseTerm() );
    }

    return result;
}

ExprNode* Parser::ParseFactor(){

    auto tok = scan.Get();

    if( isConst(*tok) ){
        scan.Next();
        return new ConstNode(tok->Value);
    }
    if ( isVar(*tok)){
            scan.Next();
            return new VarNode( tok -> Value);
    }

    if( isEq(*tok , *lParTkn)){

        scan.Next();
        auto result = ParseExpr();
        if(!isEq(*scan.Get() , *RParTkn))
            throw MyException("ERROR: No closing bucket");
        scan.Next();
        return result;
    }

        throw MyException("no right operand");

}