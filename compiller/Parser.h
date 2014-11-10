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
#include "Symbol.h"


class Parser{
    Scanner &scan;
    int struct_counter;
    map <string, int> priorityTable;
    map <string, bool> right;
    map <string, bool> unary;
    ExprNode * m_top;
    Symbol * m_symbolBuffer;

    //SymTableStack tablesStack;
    Block global_field;
   // FuncSymbol *parsingFunc;
    std::map<string, bool> assignmentOper, unaryOper;
    SymTableStack * m_symStack;
    bool m_semOff, m_onlyConstExpr, m_onlyListInit, m_constDecl;
    //Symbol * m_symbolBuffer;

    // initialization methods
    void initAssignmentOper();
    void initUnaryOper();
    void initBaseTypes();

    // helper methods
    void errorIf(bool, string, Token * = NULL);
    void errorIf_sem(bool, string, Token * = NULL);
    bool isBaseType(Token *);

    // parse methods
    ExprNode * statementList();
    ExprNode * statement();
    ExprNode * typeSpecifier();
    ExprNode * constantExpr();
    ExprNode * expressionStmt();
    ExprNode * expression();
    ExprNode * assignmentExpr();
    ExprNode * conditionalExpr();
    ExprNode * binaryOperExpr(int);
    ExprNode * castExpr();
    ExprNode * structSpecifier();
    ExprNode * unaryExpr();
    ExprNode * postfixExpr();
    ExprNode * declarationList();
    void argsListExpr(std::vector<ExprNode *> &);
    ExprNode * primaryExpr();
    ExprNode* declaration();
    ExprNode* initDeclaratorList(SymbolType * type);
    ExprNode * initDeclarator(SymbolType * type);
    ExprNode * directDeclarator(Symbol * type) ;
    ExprNode * structDeclarationList(Symbol * typeStruct);
    ExprNode * structDeclaration(Symbol * typeStruct);
    ExprNode * structDeclaratorList(Symbol * type);
    ExprNode * pointer(Symbol * type);
    ExprNode * initializer();
    void initializerList(std::vector<ExprNode *> & inits);
public:
    Parser(Scanner *, bool = false);
    
    void parse();
    void ParseProgram();
    void ParseDeclaration();
    ExprNode* ParseAssing();
    ExprNode* ParseCond();
    //VarSymbol* ParseDirectDeclaration();
    //VarSymbol* ParseComplexDeclaration(SymbolType* start_type);
   /// VarSymbol* ParseIdentifier(SymbolType* start_type);
   // ExprNode* ParseBinaryPriority(int priority);
    bool findPriority (Token* op, int priority);
    ExprNode* ParseCasr();
   // SymbolType* ParseType();
    Block *ParseBlock();
    ExprNode* ParseUnary();
    ExprNode* ParsePostfix();
    void ParseArgList();
    ExprNode* ParsePrimary();
    ExprNode* ParseCast();
 //   void SymCreate(SymTable *table, const string& name, S_types tp = t_Int );
    void ParseParam();
  /*  SymbolType *ParseType(bool param = false);
    VarSymbol *ParseIdentifier(SymbolType *type, bool param = false);
    SymbolType *ParseArrayDimension(SymbolType *type, bool param = false);
    FuncSymbol* createFunction(const string &name, SymbolType *type);
    StructSymbol *ParseStruct(bool param = false);
    // ForStatement* ParseFor();*/

public:
    ExprNode* ParseExpr();
    Parser ( Scanner &_scan):scan(_scan){
        scan.Next();
    }

};
