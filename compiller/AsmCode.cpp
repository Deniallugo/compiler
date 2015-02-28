//
//  AsmCode.cpp
//  compiller
//
//  Created by Данил on 06.12.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "AsmCode.h"

static const string arr[] = {
    "mov", "push","pushad", "pop", "mul", "imul", "div", "idiv", "add", "sub", "inc", "dec", "ret", "neg", "cdq",
    "invoke", "shr", "shl", "and", "or", "xor", "not", "call", "jmp", "cmp", "je", "jne", "setg", "setl",
    "setge", "setle", "sete", "setne", "fdivp", "faddp", "fmulp", "fsubp", "fld", "fstp", "fchs", "fild",
    "fistp", "fld1", "fcompp", "fnstsw", "sahf", "real4", "real8", "dd", "db", "dq"
};

string AsmStrArg::generate()const{
    return  value;
}

AsmStrArg* makeString(const string &name){
	string str(name);
	for (int i = 0; i < str.length() - 2; i++)
	if (str.substr(i, 2) == "\\n"){
		string s1(str.substr(0, i)), s2(str.substr(i + 2));
		str = s1 + "\", 0dh, 0ah";
		if (s2.length() > 1)
			str += ", \"" + s2;
	}
	str += ", 0";
	return new AsmStrArg(str);
}
string AsmDup::generate() const {
	return to_string(count) + " dup(0)";
}

string AsmRegArg::generate() const{
    string args[] = {"eax", "ebx", "ecx", "edx", "ebp","esp","cl","al","dl","bl","ax"};
    return args[reg];
}

string AsmRegArg::get_str() const{
	string args[] = { "eax", "ebx", "ecx", "edx", "ebp", "esp", "cl", "al", "bl", "ax" };
	return args[reg];
}

bool AsmRegArg::usedRegistr(Registers r){
    return reg==r;
}
string AsmIndirectArg::generate() const {
    return "[" + get_str() 
		+ " + " + to_string(offset) + "]"
		;
}



bool AsmIndirectArg::usedRegister(Registers r) const {
    return r == reg;
}

bool AsmIndirectArg::isMemory() const {
    return true;
}


string AsmMemoryArg::generate()const{
    return name;
}
void AsmCode::fflush(ofstream& out) const {
    for(int i = 0; i < commands.size(); i++)
        out << (dynamic_cast<AsmInstrLabel*>(commands[i]) ? "" : "\t") << commands[i]->generate() << endl;
}
void AsmCode::fflush() const {
	for (int i = 0; i < commands.size(); i++)
		cout << (dynamic_cast<AsmInstrLabel*>(commands[i]) ? "" : "\t") << commands[i]->generate() << endl;
}

string AsmLabelArg::generate() const{
    return label;
}
string AsmLabelArg::get_name(){
    return label;
}
string AsmInstrLabel::generate()const{
    return label->generate() + ":";
}
string AsmFloatArg::generate() const{
    return  (value);
}


string AsmIntArg::generate()const{
    return  (value);
}
string AsmCharArg::generate()const{
	return  ("'"+ value +"'");
}


string AsmInstr::get_str()const{
    return arr[cmd];
}
string AsmInstr::generate()const{
	return arr[cmd];
}

string AsmUno::generate()const{


    return arr[cmd] +(dynamic_cast<AsmIntArg*>(arg1) && cmd != _RET ? " dword ptr " : " ")  +arg1->generate();


}


AsmArgs* AsmUno::argument(){
    return arg1;
}

bool AsmUno::changeStack() const {
    return cmd== _PUSH || cmd == _POP || cmd == _RET || cmd == _CALL;
}

string AsmDouble::generate()const{
    return cmd  < _REAL4
    ? arr[cmd] + " " + arg1->generate() + ", " + arg2->generate()
    : arg1->generate() + " " + arr[cmd] + " " + arg2->generate();}

int AsmCode::size() const{
    return commands.size();
}
AsmIO::AsmIO(string m, AsmMemoryArg *f, AsmArgs *a) : AsmInstr(_INVOKE), command(m), format(f) ,arg(a){}
string AsmIO::generate()const{
		string common =  get_str() + " crt_" + command + ", " + "addr " + format->generate();
	if (arg)
		return  common + ", " + arg->generate();
	else
		return common;	


}

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

AsmCode& AsmCode::add(Commands c, Registers r1, string r2){
    commands.push_back(new AsmDouble(c, new AsmRegArg(r1), new AsmIntArg(r2)));
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


void Generator::generate(){
	ofstream out(file);
	out << (".686\n"
		   ".model flat\n"
		   
		   "includelib c:\\masm32\\lib\\msvcrt.lib\n"
		   "extern __imp__printf: proc\n"
		   
		   ".data\n"
		   );
    data.fflush(out);
	out<< (".code\n");
    code.fflush(out);
	out << ("end _start");
}

