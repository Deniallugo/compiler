//
//  Symbol.cpp
//  compiller
//
//  Created by Данил on 26.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include <stdio.h>
#include "Symbol.h"
#include <stdlib.h>

#include <string>

using namespace std;


void Symbol :: print(int deep) const{
    cout << string(2 * deep, ' ') << name << endl;
}

string SymbolType :: typeName() const{
    return name;
}


int PointerSymbol::byteSize() const {
	return 4;
}
int StructSymbol::byteSize() const {
	return m_fields ? m_fields->byteSize() : 0;
}
int SymTable::byteSize() const {
	int b = 0;
	for (int i = 0; i < size(); i++)
		b += sym_ptr[i]->byteSize();
	return b;
}
int ArraySymbol::byteSize() const {
	return type->byteSize() * size;
}

int ScalarSymbol::byteSize() const {
	if (name == "void")
		return 0;
	if (name == "char")
		return 1;
	if (name == "int" || name == "float")
		return 4;
	throw MyException("Unknown basic type");
}
int VarSymbol::byteSize() const {
	return type->byteSize();
}
extern void print_node(int, ExprNode*);

SymbolType* VarSymbol :: getType(){
    return type;
}

VarSymbol :: VarSymbol(string str, SymbolType *tp, ExprNode *i) : SymbolType(str), type(tp), init(i) {
//    if(!init){
//        init = init_var(type);
//    }
}

void VarSymbol :: print(int deep) const{
    cout << string(2 * deep, ' ') << name;
    if(type->isFunc())
        type->print(deep);
    else
        cout << ' ' << type->typeName() << endl;

}

SymbolType::SymbolType(Token* tok){
    type = tok->Type;
}

bool ScalarSymbol :: canConvertTo(SymbolType *to){
    if(dynamic_cast<PointerSymbol*>(to) || dynamic_cast<FuncSymbol*>(to)  )
        return false;
    int a = typePriority[this];
    int b = typePriority[to];
    return a <= b;
}

bool ScalarSymbol :: isModifiableLvalue() const{
    return true;
}

string ConstSymbolType :: typeName() const{
    return "const " + symbol->typeName();
}
SymbolType* ConstSymbolType::getType(){
    return symbol->getType();
}

extern void func_print(int, Block*);

bool FuncSymbol :: isFunc() const{
    return true;
}

void FuncSymbol:: print(int deep) const{
    cout << string(2 * deep,' ') << value->typeName() << " function " << name << "(\n";
    params->print(deep + 1);
    cout <<string (deep + 1, ' ')<< ")" << endl;
    if(body)
        func_print(deep, body);
}

SymbolType* FuncSymbol :: getType(){
    return value;
}




bool FuncSymbol :: canConvertTo(SymbolType* to){
    return this->getType() == to;
}

string ArraySymbol :: typeName() const{
    char buff[10];

    //_itoa_s(size, buff, 10);
    string massage = "array[";
    massage += buff;
    return massage + "] of " + type->typeName();
}

bool ArraySymbol :: operator == (SymbolType* s) const{
    ArraySymbol *arr = dynamic_cast<ArraySymbol*>(s);
    if(!arr)
        return false;
    return size == arr->size && *type == arr->type;
}

bool ArraySymbol :: operator != (SymbolType *s) const{
    return !(*this == s);
}


bool ArraySymbol :: canConvertTo(SymbolType *to, Token* t){
//    if(*to == IntType)
//        return true;
//    PointerSymbol *ptr = dynamic_cast<PointerSymbol*>(to);
//    if(t->Value != "="){
//        return false;
//    }
    PointerSymbol *pointer = dynamic_cast<PointerSymbol*>(to);
    if(pointer)
        return (this->getType()->canConvertTo(pointer->getType()));
    return false;
//    return ptr && (*ptr->pointer == type);
}


bool FuncSymbol :: operator == (SymbolType *s) const{
    FuncSymbol *f = dynamic_cast<FuncSymbol*>(s);
    if(!f)
        return false;
    return params == f->params && type == f->type;
}


bool FuncSymbol :: operator != (SymbolType *s) const{
    return !(*this == s);
}


bool PointerSymbol :: operator == (SymbolType *s) const{
    PointerSymbol *p = dynamic_cast<PointerSymbol*>(s);
    if(!p)
        return false;
    SymbolType *type1 = pointer;
    SymbolType *type2 = p->upType();

    while(true){
        if(*type2 == "void" && dynamic_cast<ScalarSymbol*>(type1))
            return true;
        if(!(*type1 == type2))
            return false;
        SymbolType *t1 = type1->upType();
        SymbolType *t2 = type2->upType();
        if(!(t1 && t2))
            return !(t1 != 0 && t2 != 0);
        else
        {
            type1 = t1;
            type2 = t2;
        }
    }
}

bool PointerSymbol :: canConvertTo(SymbolType *to){
    if(*to == IntType)
        return true;
    PointerSymbol *pointer = dynamic_cast<PointerSymbol*>(to);
    if(pointer)
        return true;
    return false;
}


bool PointerSymbol :: isModifiableLvalue() const{
    return true;
}

bool PointerSymbol :: operator != (SymbolType *s) const{
    return !(*this == s);
}
string PointerSymbol :: typeName() const{
    return "pointer to " + pointer->typeName();
}

bool StructSymbol :: canConvertTo(SymbolType *to){
    StructSymbol* _struct = dynamic_cast<StructSymbol*>(to);
    
    if (!_struct || (*m_fields != _struct->m_fields))
        return false;
    return true;
}


string StructSymbol :: typeName() const{
    return "struct " + name;
}


void StructSymbol :: print(int deep) const{
    cout << string(2 * deep, ' ') << "struct " << name << endl;
    if (m_fields)
        m_fields->print(deep + 1);
}

void SymTable :: print(int deep) const{
    for(int i = 0; i < name.size(); i++){
        cout << string(2 * deep, ' ');
        sym_ptr[i]->print(deep + 1);
    }
}

Symbol* SymTable :: find_symbol(const string &name) const{

    return index.count(name) == 1 ? sym_ptr[index.at(name)] : 0;
}

Symbol* SymTable :: find_type(const string &name) const{
    Symbol *symb = find_symbol(name);
    if(dynamic_cast<VarSymbol*>(symb))
        return 0;
    else
        return symb;
}

Symbol* SymTable :: back() const{
    return sym_ptr.back();
}


int SymTable :: size() const{
    return sym_ptr.size();
}

bool SymTable :: exists(const string& name) const{
    return find_symbol(name) != 0;
}


void SymTable :: add_symbol(Symbol *s){
    name.push_back(s->name);
    sym_ptr.push_back(s);
    index[s->name] = name.size() - 1;
	
	if (VarSymbol *var = dynamic_cast<VarSymbol*>(s)){
		SymbolType* type = var->getType();
		if (dynamic_cast<ArraySymbol*>(type))
			s->offset = -(offset +
			type->byteSize()
			- type->upType()->byteSize());
		else
			s->offset = -offset
			 +index[s->name] * s->byteSize()
			;
		s->offset -= s->byteSize();
	//	if (sym_ptr.size() == 1){
		//	var->global = true;
		//}
		//else
			//var->global = false;
	}
	
}

bool SymTable :: isExist(const string &name) const{
    return find_symbol(name) != 0;
}

bool SymTable :: operator == (SymTable* table) const{
    if(sym_ptr.size() != table->sym_ptr.size())
        return false;
    for(int i = 0; i < sym_ptr.size(); i++)
        if(*dynamic_cast<VarSymbol*>(sym_ptr[i])->type != dynamic_cast<VarSymbol*>(table->sym_ptr[i])->type)
            return false;
    return true;
}


bool SymTable :: operator != (SymTable* table) const{
    return !(*this == table);
}

SymTable* SymTableStack :: top(){
    if(tables.empty())
        return 0;
    return tables.back();
}

void SymTableStack :: add_symbol(Symbol* s){
    top()->add_symbol(s);
}

void SymTableStack :: push(SymTable *t){
    tables.push_back(t);
}

bool SymTableStack :: exist_in_top(const string &name){
    return top()->find_symbol(name) != 0;
}

void SymTableStack :: pop(){
    tables.pop_back();
}

void SymTableStack :: print(int deep) const{
    static const string line = "\n   ----=====================================================----\n";
    for(int i = 1; i < tables.size(); i++){
        tables[i]->print(deep);
        cout << line;
    }
}
SymbolType* TypedefSymbol::getType(){
    return type;
}
void TypedefSymbol::print(int deep)const{
    cout << "typedef " <<type->typeName() <<" with " << name<<"\n";
    
}
Symbol* SymTableStack :: find_symbol(const string &name) const{
    Symbol* sym = 0;
    for (int i = tables.size() - 1; i >= 0 && !sym; i--)
        sym = tables[i]->find_symbol(name);
    return sym;
}

Symbol* SymTableStack :: find_type(const string &name) const{
    Symbol* sym = 0;
    for (int i = tables.size() - 1; i >= 0 && !sym; i--)
        sym = tables[i]->find_type(name);
    return sym;
}

int SymTableStack :: size() const{
    return tables.size();
}
