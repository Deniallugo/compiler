//
//  Parser.cpp
//  compiller
//
//  Created by Данил on 03.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "Parser.h"



ExprNode* Parser::ParseExpr(){

    auto result = ParseTerm();

    Token* op  = scan.Get();

    if ( op ==  plusTkn || op == minusTkn){
        scan.Next();
        result = new BinOpNode( op, result, ParseExpr() );
    }
    return result;
}

ExprNode* Parser::ParseTerm(){

    auto result = ParseFactor();

    Token* op  = scan.Get();
    if ( op == plusTkn || op == minusTkn){
        scan.Next();
        result = new BinOpNode( op, result, ParseTerm() );
    }
    return result;
}

bool isConst( Token t){
    return (t.Type == _FLOAT || t.Type == _INTEGER);
}
bool isIdent( Token t){
    return (t.Type == _IDENTIFIER);
}

ExprNode* Parser::ParseFactor(){

    auto tok = scan.Get();

    if( isConst(*tok) ){
        scan.Next();
        return new ConstNode(tok->Value);
    }
    if ( isIdent(*tok)){
            scan.Next();
            return new VarNode( tok -> Value);
    }

    if( tok == lParTkn){

        scan.Next();
        auto result = ParseExpr();
        if(scan.Get() != RParTkn)
            throw MyException("hren");
            scan . Next();
        return result;
    }

        throw MyException("hren");
}
    
