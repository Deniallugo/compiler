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
#include <stack>
class Parser{
    Scanner &scan;
    int struct_counter;
    map <string, int> priorityTable;
    map <string, bool> right;
    map <string, bool> unary;
    stack<Block*> blocks;

    Symbol * symbolBuffer;
    FuncSymbol *parsingFunc;
    bool isCanUseBreak;
    FuncSymbol* main_func;
    Block global_field;
    std::map<string, bool> assignmentOper, unaryOper;
    SymTableStack *symStack;

    void errorIf(bool, string, Token * = NULL);
    void errorIf_sem(bool, string, Token * = NULL);
    bool isBaseType(Token *);

public:
    Parser(Scanner *, bool = false);
    
    void parse();
    void ParseProgram();
    ExprNode* ParseDeclaration(SymbolType* sym_type = nullptr);
    SymTable* ParseStructBlock();
    bool CheckArgs(FuncSymbol* func1);
    ExprNode* ParseAssing();
    ExprNode* ParseCond();
    VarSymbol* ParseDirectDeclaration();
    VarSymbol* ParseComplexDeclaration(SymbolType* start_type);
    ExprNode* ParseBinaryPriority(int priority);
    bool findPriority (Token* op, int priority);
    ExprNode* ParseCasr();
    SymbolType* ParseType(bool param = false);
    Block *ParseBlock();
    void print_declaration(int deep)const;
    void print()const;
    ExprNode* ParseMember(ExprNode* left);
    ExprNode* ParseUnary();
    ExprNode* ParsePostfix();
    void ParseArgList();
    ExprNode* ParsePrimary();
    ExprNode* ParseCast();
    ExprNode* ParseFactor();
    ArrNode* ParseArrIndex(ExprNode *root);
    FunctionalNode* ParseFuncCall(ExprNode *r);
    void SymCreate(SymTable *table, const string& name, S_types tp = t_Int );
    void ParseParam();
    VarSymbol *ParseIdentifier(SymbolType *type, bool param = false);
    SymbolType *ParseArrayDimension(SymbolType *type, bool param = false);
    FuncSymbol* createFunction(const string &name, SymbolType *type, bool ConstFun  = false);
    StructSymbol *ParseStruct(bool param = false);
    ForStatement* ParseFor();
    ExprNode *ParseStatement();
    IfStatement* ParseIf();
    WhileStatement* ParseWhile();
    DoWhileStatement* ParseDoWhile();
    Statement* ParseJumpStatement();
    void CheckReturn(FuncSymbol* func);
    ExprNode* ParseExpr(int priority = 0 );
    Parser ( Scanner &_scan);
    Statement* CycleStatement;
    friend class MyException;
};
