#pragma once
#include "scanner.h"
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
#include "AsmCode.h"

enum S_types{
    t_Int,
    t_Float,
    t_Char,
    t_Void,
    t_Struct,
    t_Error
};

class SymbolType;

class Symbol{
public:
    int offset;
    string name;
    Symbol(){}
    Symbol(string n, int i = 0  ): name(n), offset(i){}
	virtual int byteSize() const{ return 0; }
    virtual SymbolType *getType() { return 0; }
    virtual bool isFunc() const { return false; }
    virtual void print(int deep) const;
};

class SymbolType : public Symbol{
public:
    TYPES type;
    SymbolType(string n, TYPES t = _VOID) : Symbol(n) { type = t; }
    SymbolType(Token* op);
    virtual string typeName() const;
    virtual bool isStruct() { return false; }
    virtual SymbolType* upType() {return 0; }
    virtual bool canConvertTo(SymbolType *to) { return false; }
    virtual SymbolType* getType() { return this; }
    virtual void initType(SymbolType *t) {}
    virtual bool isModifiableLvalue() const { return false; }
    bool operator == (const string &n) const { return n == name; }
    bool operator != (const string &n) const { return n != name; }
    virtual bool operator == (SymbolType *t) const { return this == t; }
    virtual bool operator != (SymbolType *t) const { return !(this == t); }
};

class ScalarSymbol: public SymbolType{
public:
	int intValue;
    ScalarSymbol(const string &name, TYPES t = _VOID) : SymbolType(name,t) {}
    bool canConvertTo(SymbolType* to);
    bool isModifiableLvalue() const;
     virtual int byteSize() const;
};

class Block;

class SymTable : public Symbol{
public:
    vector<string> name;
    vector<Symbol*> sym_ptr;
    map <string, int> index;
    virtual int byteSize() const;
    SymTable() : name(0), sym_ptr(0) {}
    void print(int deep = 0) const;
    bool isExist(const string &name) const;
    void generateGlobals(AsmCode &code) const ;
    void generateCode(AsmCode &code) const ;
    void add_symbol(Symbol *s);
    bool exists(const string &name) const;;
    void push(SymTable *t);
    bool operator == (SymTable *t) const;
    bool operator != (SymTable *t) const;
    Symbol* find_symbol(const string &name) const;
    Symbol* find_type(const string &name) const;
    Symbol* back() const;
    int size() const;
};

class SymTableStack : public Symbol{
private:
    friend class Parser;
    vector <SymTable*> tables;

public:
    void add_symbol(Symbol* s);
    SymTable* top();
    bool exist_in_top(const string &name);
    void push(SymTable* t);
    void pop();
    void print(int deep = 0) const;
    Symbol* find_symbol(const string &name) const;
    Symbol* find_type(const string &name) const;
    int size() const;
};

class ExprNode;
extern ExprNode* init_var(SymbolType *type);

class VarSymbol : public SymbolType{
    ExprNode* value;
public:
    ExprNode *init;
    SymbolType *type;
     virtual int byteSize() const;
    SymbolType *getType();
    bool global;
    void generate(AsmCode &code) const;
    VarSymbol(string str, SymbolType *tp, ExprNode *i = 0);
    void print(int deep) const;
    void initType(SymbolType *t) { type = t; }
    void initVal(ExprNode* val){value = val;}
};

class TypedefSymbol :public SymbolType{
    SymbolType *type;
public:
    string typeName;
    SymbolType *getType();

    void print(int deep) const;
    TypedefSymbol(SymbolType* _type, string name):SymbolType(name), type(_type), typeName(_type->name){}
    

};

class ConstSymbolType : public SymbolType{
public:
    SymbolType *symbol;
    ExprNode * init;
    ConstSymbolType(SymbolType *symb, ExprNode *_init = nullptr, TYPES s_type = _VOID) : SymbolType("const_" + symb->name, s_type), symbol(symb),init(_init) {}
    string typeName() const;
    SymbolType* getType();
    virtual bool isStruct() { return symbol->isStruct(); }
};


class StructSymbol : public SymbolType{
public:
    SymTable* m_fields;

    StructSymbol(SymTable *st, string name) : SymbolType(name, _STRUCT), m_fields(st) {}
    virtual int byteSize() const;
    void print(int deep) const;
    void generateGlobals(AsmCode &code) const;
    void generateCode(AsmCode &code) const;
    string typeName() const;
    virtual bool isStruct() { return true; }
    bool canConvertTo(SymbolType* to);

    SymTable* getTable(){
        return m_fields;
    }
};


class ArraySymbol : public SymbolType{
public:
    int size;
    SymbolType* type;
    ArraySymbol(SymbolType *st, int sz, const string &name = "") : SymbolType("array"), size(sz), type(st) {}
	ArraySymbol(ArraySymbol* ar) : SymbolType("array"), size(ar->size), type(ar->type) {}

    string typeName() const;
    SymbolType* upType() {return type; }
    bool canConvertTo(SymbolType *to, Token* t = nullptr);
    bool operator == (SymbolType *s) const;
     virtual int byteSize() const;
    bool operator != (SymbolType *s) const;
    void initType(SymbolType *t) { type = t; }
};

class Block;

class FuncSymbol : public SymbolType{
public:
    SymTable *params;
    SymbolType *value;
    AsmLabelArg *endLabel;
    Block *body;
    bool isConst;
    bool operator == (SymbolType *s) const;
    bool isFunc() const;
    bool operator != (SymbolType *s) const;
    void print(int deep) const;
    SymbolType *getType();
    void generate(AsmCode &code,const string& name)const;
    FuncSymbol(const string &name, SymbolType *val, Block *block = 0) : SymbolType(name), value(val), body(block),endLabel(0) ,params(0) {}
    bool canConvertTo(SymbolType* to);
    void initType(SymbolType *t) {value = t; }
};

class PointerSymbol : public SymbolType{
public:
    SymbolType *pointer;
    PointerSymbol(SymbolType *ptr) : SymbolType(""), pointer(ptr) {}
    SymbolType* upType() { return pointer; }
    string typeName() const;
    bool canConvertTo(SymbolType* to);
    bool isModifiableLvalue() const;
    bool operator == (SymbolType *t) const;
    bool operator != (SymbolType *t) const;
     virtual int byteSize() const;
    void initType(SymbolType *t) { pointer = t; }
};

extern ScalarSymbol *IntType;
extern ScalarSymbol *FloatType;
extern ScalarSymbol *CharType;
extern ScalarSymbol *VoidType;
extern PointerSymbol *StringType;

extern map<SymbolType*, int> typePriority;
extern map<string, SymbolType*> operationTypeOperands;//костыль со стрингами
extern map<string, SymbolType*> operationReturningType;
