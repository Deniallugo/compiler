//
//  AsmCode.cpp
//  compiller
//
//  Created by Данил on 06.12.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "AsmCode.h"

static const string arr[] = {
    "mov", "push", "pop", "mul", "imul", "div", "idiv", "add", "sub", "inc", "dec", "ret", "neg", "cdq",
    "invoke", "shr", "shl", "and", "or", "xor", "not", "call", "jmp", "cmp", "je", "jne", "setg", "setl",
    "setge", "setle", "sete", "setne", "fdivp", "faddp", "fmulp", "fsubp", "fld", "fstp", "fchs", "fild",
    "fistp", "fld1", "fcompp", "fnstsw", "sahf", "real4", "real8", "dd", "db", "dq"
};

string AsmStrArg::gen_string()const{
    return  value;
}

string AsmRegArg::gen_str() const{
    string args[] = {"eax", "ebx", "ecx", "edx", "ebp","esp","cl","al","bl","ac"};
    return args[reg];
}


bool AsmRegArg::usedRegistr(Registers r){
    return reg==r;
}

string AsmMemoryArg::gen_str(){
    return name;
}

string AsmLabelArg::gen_str(){
    return label;
}
string AsmLabelArg::get_name(){
    return label;
}
string AsmInstrLabel::gen_str()const{
    return label->gen_str() + ":";
}
string AsmFloatArg::gen_str(){
    return to_string (value);
}

string AsmInstr::gen_str()const{
    return arr[cmd];
}

string AsmUno::gen_str()const{
    return arr[cmd] +(dynamic_cast<AsmIntArg*>(arg1) && cmd != _RET ? " dword " : " ")  +arg1->gen_string();
}

AsmArgs* AsmUno::argument(){
    return arg1;
}

bool AsmUno::changeStack() const {
    return cmd== _PUSH || cmd == _POP || cmd == _RET || cmd == _CALL;
}

string AsmDouble::gen_str()const{
    return cmd  < _REAL4
    ? arr[cmd] + " " + arg1->gen_string() + ", " + arg2->gen_string()
    : arg1->gen_string() + " " + arr[cmd] + " " + arg2->gen_string();}

int AsmCode::size() const{
    return commands.size();
}
AsmIO::AsmIO(string m, AsmMemoryArg *f, AsmArgs *a) : AsmInstr(_INVOKE), command(m), format(f) {
    args.push_back(a);
}
AsmIO::AsmIO(string m, AsmMemoryArg *f, vector<AsmArgs *>a) : AsmInstr(_INVOKE), command(m), format(f),args(a){}
string AsmIO::gen_str()const{return 0;}
void AsmCode::replace(int index, AsmInstr* cmd ){
    delete commands[index];
    commands[index] = cmd;
}
void AsmCode::insert(AsmInstr* cmd, int index){
    commands.insert(commands.begin() + index, cmd);
}
void AsmCode::move(int from, int to){
    AsmInstr* t = commands[from];
    commands.erase(commands.begin()+from);
    commands.insert(commands.begin() + to, t);
}

AsmInstr* AsmCode::operator[](int index) const{
    return commands[index];
}

AsmCode& AsmCode::add(Commands c){
    commands.push_back(new AsmInstr(c));
    return *this;
}

AsmCode& AsmCode::add(Commands c, AsmArgs *arg){
    commands.push_back(new AsmUno(c,arg));
    return *this;

}

AsmCode& AsmCode::add(Commands c, AsmArgs *arg1, AsmArgs *arg2){
    commands.push_back(new AsmDouble(c,arg1,arg2));
    return *this;

}

AsmCode& AsmCode::add(Commands c, Registers r){
    commands.push_back(new AsmUno(c,new AsmRegArg(r)));
    return *this;
}
AsmCode& AsmCode::add(Commands c, Registers r1, Registers r2){
    commands.push_back(new AsmDouble(c,new AsmRegArg(r1), new AsmRegArg(r2)));
    return *this;
}
AsmCode& AsmCode::add(Commands c, Registers r1, int r2){
    commands.push_back(new AsmDouble(c,new AsmRegArg(r1), new AsmIntArg(r2)));
    return *this;
}
AsmCode& AsmCode::add(AsmLabelArg *label){
    commands.push_back(new AsmInstrLabel(label));
    return *this;
}
AsmCode& AsmCode::add(string val, AsmMemoryArg *format, AsmArgs *arg){
    commands.push_back(new AsmIO(val,format,arg));
    return *this;
}

