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
#include "Symbol.h"
#include "AsmCode.h"
#include <vector>
using namespace std;



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

static bool isEqual(Token* op){
    return (isEq(op, _OPERATION, "<=")||
            isEq(op, _OPERATION, ">=")||
            isEq(op, _OPERATION, "!=")||
            isEq(op, _OPERATION, "==")||
            isEq(op, _OPERATION, ">")||
            isEq(op, _OPERATION, "<")

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

class ExprNode {
protected:
    Token *token;
public:
    int index;
    friend class Parser;
    friend class BinOpNode;
    friend class MyException;
    void SetIndex( int i ){index = i;}
	virtual void generate(AsmCode &code){}
	virtual void generateLvalue(AsmCode& code){}
    virtual bool isExpression() const { return true; }
    Token* getToken(){ return token;}
    ExprNode(){token = nullptr;}
    ExprNode(Token* t, int i = 0):token(t), index(i){}
    ExprNode* init_var(SymbolType *);
    virtual void print(int deep = 0) const {};
    virtual void print(ofstream *t, int deep = 0) const {}
    virtual SymbolType* getType() { return 0; }
    virtual bool isLvalue() const { return false; }
    virtual bool isModifiableLvalue() const { return false; }
    static ExprNode* makeTypeCoerce(ExprNode* expr, SymbolType *from, SymbolType *to,  Token* t = nullptr);
    friend class ArrNode;

	friend class PoinerNode;
	friend class FuncCallNode;
};

class OpNode : public ExprNode{
protected:
    string opName() const;
public:
    OpNode(Token *op);
    virtual void generate(AsmCode &code) {}
    virtual void generateLValue(AsmCode &){}
    virtual void print(int deep) const {}
    virtual void print(int deep, ofstream* f) const {}
};

class BinOpNode : public OpNode{
protected:
    ExprNode *left;
    ExprNode *right;

public:
    friend class Parser;
    friend class VarSymbol;
	friend class PointerNode;
    BinOpNode(Token *op, ExprNode* l, ExprNode* r);
    virtual SymbolType *getType();
    bool isLvalue() const;
    void SetIndex(int i);
    virtual void generate(AsmCode &code);
    virtual void generateLvalue(AsmCode &);
    bool isModifiableLvalue() const;
    void print(int deep) const;
    void print(ofstream *f, int deep);
};


class UnOpNode : public OpNode{
protected:
    ExprNode *operand;
public:
    UnOpNode(Token *t, ExprNode *op);
    bool isLvalue() const;
    bool isModifiableLvalue() const;
    virtual SymbolType *getType();
    void SetIndex(int i);
    virtual void generate(AsmCode &code);
    virtual void generateLvalue(AsmCode &);
    void print(int deep) const;
    void print(ofstream *f, int deep) const;
};


class PostfixUnaryOpNode : public UnOpNode{
public:
    PostfixUnaryOpNode(Token *t, ExprNode *op);
    void print(int deep) const;
    void print(ofstream *f, int deep) const;
    bool isLvalue() const;
    bool isModifiableLvalue() const;
};


class ConstNode : public ExprNode{
public:
    string value;
    TYPES type;

    void print(int deep) const;
    ConstNode (Token t){
        value = t.Value;
        type = t.Type;
    }
    virtual void generate(AsmCode &code){}
    virtual void generateLValue(AsmCode &){}
    ConstNode (string _value) {
        if (!_value.find('.'))
            type = _FLOAT;
         else
            type = _INTEGER;
        value = _value;
    }

};

class VarNode : public ExprNode{
    Symbol* varSym;
public:
    string name;
    void print(int deep) const;
    virtual void generate(AsmCode &code);
    virtual void generateLValue(AsmCode &);
    VarNode(string _name, SymbolType* _type = nullptr){
        name = _name;
        varSym = _type;
    }
    Symbol* getSym(){
        return varSym;
    }
    void setType( Symbol* _type){
        varSym = _type;
    }
};


class StmtListNode : public ExprNode {
    ExprNode * m_left, * m_right;

public:
    StmtListNode(ExprNode * _left, ExprNode *_right) : m_left(_left), m_right(_right){}
    void print(int deep) const;
};




class ExprStmtNode : public ExprNode {
    ExprNode * m_expr;

public:
    ExprStmtNode(ExprNode *_expr):m_expr(_expr){}
    void print(int deep) const;
    virtual void generate(AsmCode &code);
    virtual void generateLValue(AsmCode &);
    void print(ofstream *f, int deep) const;
    SymbolType* getType();
    bool isLvalue() const ;
    bool isModifiableLvalue() const ;
};

class CastNode : public UnOpNode{
private:
   vector< SymbolType*> *s_type;
    SymbolType* type;
    
public:
    CastNode(Token *op, ExprNode *oper, vector<SymbolType*> *ts);
    CastNode(Token* op, ExprNode* oper, SymbolType* ts);
	virtual void generate(AsmCode &code){ operand->generate(code); }
    virtual void generateLValue(AsmCode &){}
	bool isModifiableLvalue() const{ return true; }
     SymbolType *getType();
    void print(int deep) const;
    void print(ofstream *f, int deep) const;
};

class RetNode : public UnOpNode{
public:
    RetNode(Token *token, ExprNode *n) : UnOpNode(token, n) {}
    virtual void generate(AsmCode &code);
    virtual void generateLValue(AsmCode &);
    void print(int deep) const;
};


class RecordNode : public ExprNode {
    ExprNode * m_left, * m_right;

public:
    RecordNode(ExprNode *left, Token *op, ExprNode *right) : ExprNode(op), m_left(left), m_right(right){}
    void print(int deep) const;
    void print(ofstream *f, int deep) const;
    bool isLvalue() const {return false;}
    virtual void generate(AsmCode &code);
    virtual void generateLValue(AsmCode &);
    bool isModifiableLvalue() const { return false;}
};

class TernaryOpNode : public BinOpNode{
private:
    ExprNode* condition;
public:
    virtual void generate(AsmCode &code){}
    virtual void generateLValue(AsmCode &){}
    TernaryOpNode(Token* op, ExprNode* c, ExprNode* l, ExprNode* r);
    void print(int deep) const;
    void print(ofstream *f, int deep) const;
};





class IdentifierNode : public ExprNode{
public:
    VarSymbol *var;
    IdentifierNode(Token *t, VarSymbol *v);
    IdentifierNode(const string & name, int line, int col);
    SymbolType* getType();
    void print(int deep) const;
    void print(ofstream* f, int deep) const;
    bool isLvalue() const;
    virtual void generate(AsmCode &code);
	virtual void generateLvalue(AsmCode& code);
	void generateLvaluePointer(AsmCode& code);
	bool isModifiableLvalue() const;
};



class FunctionalNode : public ExprNode{
protected:
    FuncSymbol *symbol;
    vector<ExprNode*> args;
    void printArgs(int deep) const;
    void SetIndex(int i );
    void printArgs(ofstream *f, int deep) const;
    void print(int deep) const;
    void print(ofstream *f, int deep) const;
public:

	ExprNode *name;

    SymbolType *getType();
    FunctionalNode(ExprNode *n);
    FunctionalNode(Token *t,ExprNode *n, FuncSymbol *s = nullptr);
    void addArg(ExprNode* arg);
};

class ArrNode : public FunctionalNode{
    ExprNode *arg;
public:
    ArrNode(ExprNode* arr, ExprNode* arg = nullptr);
    void print(int deep) const;
    void print(ofstream *f, int deep) const;
    virtual void generate(AsmCode &code);
    virtual void generateLvalue(AsmCode &);
    bool isLvalue() const;
    bool isModifiableLvalue() const;
    SymbolType *getType();
};


class PointerNode : public FunctionalNode{
    ExprNode *arg;
public:
    PointerNode(ExprNode* arr, ExprNode* arg = nullptr);
	void print(int deep) const;
	void print(ofstream *f, int deep) const;
	virtual void generate(AsmCode &code);
	virtual void generateLvalue(AsmCode &);
    bool isLvalue() const;
    bool isModifiableLvalue() const;
    SymbolType *getType();
};

class Statement : public ExprNode{
    virtual void print(int deep = 0) {}
};

class Block : public Statement{
private:
    vector<ExprNode*> body;
    bool isExpression() const { return false; }
    int size() const {
        return body.size();
    }

    SymTable *table;
    AsmLabelArg *endLabel ;


    friend class Parser;

public:
    bool isCanUseBreak;
    virtual void generate(AsmCode &code);

    Block() : table(new SymTable()) { isCanUseBreak = false;}
    Block(SymTable *t) : table(t) ,endLabel(0){ isCanUseBreak = false;}
    void AddStatement(ExprNode *st) { body.push_back(st); }
    void print(int deep = 0) const;
};

class StringNode;
class IONode : public FunctionalNode{
private:
    Token *token;

    StringNode *format;

public:
    friend class Parser;
    IONode(Token *t, StringNode *n) : token(t), format(n), FunctionalNode(0, 0) {};
    SymbolType *getType();
    virtual void generate(AsmCode &code);
    void print(int deep) const;
};


class FuncCallNode : public FunctionalNode{
private:
    FuncSymbol *symbol;

public:
    FuncCallNode(Token* t, ExprNode* func, FuncSymbol* funcsym): FunctionalNode(t, func), symbol(funcsym) {}
    void generate(AsmCode& code);
    SymbolType* getType();
    void print(int deep) const;
    friend class FuncSymbol;
};

class IntNode : public ExprNode{
public:
    IntNode(Token *t);
    void print(int deep) const;
    virtual void generate(AsmCode &code);

    void print(ofstream *f, int deep) const;
    SymbolType* getType();
};


class FloatNode : public ExprNode{
public:
    FloatNode(Token *t);
    string constName() const;
    void print(int deep) const;
    void print(ofstream* f, int deep) const;
    virtual void generate(AsmCode &code);
    virtual void generateLvalue(AsmCode &);
    SymbolType* getType();
};


class CharNode : public ExprNode{
public:
    CharNode(Token *t);
    void print(int deep) const;
    SymbolType* getType();
	virtual void generate(AsmCode& code);
    void print(ofstream* f, int deep) const;
};

class StringNode : public ExprNode{
public:
    StringNode(Token *t);
    void print(int deep) const;
    SymbolType *getType();
	virtual void generate(AsmCode &code);
    virtual void generateData(AsmCode &code) ;
    void print(ofstream* f, int deep) const;
};

class ForStatement : public Statement{
private:
    ExprNode *first_cond;
    ExprNode *second_cond;
    ExprNode *third_cond;
    Block *body;
public:
    virtual void generate(AsmCode &code){}
    virtual void generateLvalue(AsmCode &){}
    ForStatement(ExprNode *first, ExprNode *second, ExprNode *third, Block *block) : first_cond(first), second_cond(second), third_cond(third), body(block) {
    }
    void print(int deep = 0) const;
};


class IfStatement : public Statement{
private:
    ExprNode *condition;
    Block *if_branch;
    Block *else_branch;
public:
    virtual void generate(AsmCode &code){}
    virtual void generateLvalue(AsmCode &){}
    IfStatement(ExprNode *cond, Block *$if, Block *$else = 0) : condition(cond), if_branch($if), else_branch($else) {

    }
    void print(int deep = 0) const;
};

class WhileStatement : public Statement{
private:
    ExprNode *condition;
    Block *body;
public:
    virtual void generate(AsmCode &code){}
    virtual void generateLvalue(AsmCode &){}
    WhileStatement(ExprNode *c, Block *b) : condition(c), body(b) {
    }
    void print(int deep = 0) const;

};

class DoWhileStatement : public Statement{
private:
    ExprNode *condition;
    Block *body;
public:
    virtual void generate(AsmCode &code){}
    virtual void generateLvalue(AsmCode &){}
    DoWhileStatement(ExprNode *c, Block *b) : condition(c), body(b) {
    }
    void print(int deep = 0) const;
};

class ContinueStatement : public Statement {
    void print(int deep = 0) const;
    virtual void generate(AsmCode &code){}

};

class BreakStatement : public Statement {
    void print(int deep = 0) const;
    virtual void generateLvalue(AsmCode &){}
    virtual void generate(AsmCode &code){}

};

class ReturnStatement : public Statement {
private:
    ExprNode *value;
    friend class Parser;
public:
    ReturnStatement(ExprNode *v) : value(v) {}
    void print(int deep = 0) const;
    virtual void generate(AsmCode &code){}
    virtual void generateLvalue(AsmCode &){}
};


static bool isAssing(Token* op);
//static bool isEq(Token* op);
static bool isUnary(Token* op);



