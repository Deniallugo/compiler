//
//  AsmCode.h
//  compiller
//
//  Created by Данил on 06.12.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#ifndef __compiller__AsmCode__
#define __compiller__AsmCode__

#include <stdio.h>
#include <vector>
#include "scanner.h"
enum Commands{
    _MOV,
    _PUSH,
	_PUSHAD,
    _POP,
    _MUL,
    _IMUL,
    _DIV,
    _IDIV,
    _ADD,
    _SUB,
    _INC,
    _DEC,
    _RET,
    _NEG,
    _CDQ,
    _INVOKE,
    _SHR,
    _SHL,
    _AND,
    _OR,
    _XOR,
    _NOT,
    _CALL,
    _JMP,
    _CMP,
    _JE,
    _JNE,
    _SETG,
    _SETL,
    _SETGE,
    _SETLE,
    _SETE,
    _SETNE,
    _FDIVP,
    _FADDP,
    _FMULP,
    _FSUBP,
    _FLD,
    _FSTP,
    _FCHS,
    _FILD,
    _FISTP,
    _FLD1,
    _FCOMPP,
    _FNSTSW,
    _SAHF,
    _REAL4,
    _REAL8,
    _DD,
    _DB,
    _DQ
};

enum Registers{
    _EAX,
    _EBX,
    _ECX,
    _EDX,
    _EBP,
    _ESP,
    _CL,
    _AL,
	_DL,
    _BL,
    _AX
};

class AsmArgs{
public:
    virtual string generate()const = 0 ;
    virtual bool usedRegistr(Registers reg){return  false;}
    virtual void clearOffset() {}


};


class AsmStrArg : public  AsmArgs{
    string value;
public:
    AsmStrArg(string _value) : value(_value){}
    virtual string generate() const;

};

class AsmRegArg:public AsmArgs{
    Registers reg;
public:
    AsmRegArg(Registers _reg): reg(_reg){}
    string generate() const;
	string get_str() const;

	bool usedRegistr(Registers reg);
    friend class AsmIndirectArg;

};

class AsmMemoryArg: public AsmArgs{
    string name;
    bool lvalue;
public:
    AsmMemoryArg(string str, bool _value = false): name(str), lvalue(_value){}
    virtual string generate()const;
    void clearOffset(){	lvalue = false;}
};

class AsmLabelArg:public AsmArgs{
    string label;
public:
    AsmLabelArg(string _label):label(_label){}
  virtual  string generate()const;
    string get_name();

};

class AsmFloatArg: public AsmArgs{
    string value;
public:
    AsmFloatArg (string _value): value(_value){}
   virtual  string generate()const;

};
class AsmIntArg: public AsmArgs{
    string value;
public :
    AsmIntArg(string _value):value(_value){}
   virtual string generate() const;

};
class AsmCharArg : public AsmArgs{
	string value;
public:
	AsmCharArg(string _value) :value(_value){}
	virtual string generate() const;

};

class AsmIndirectArg : public AsmRegArg{
private:
    int offset;

public:
    AsmIndirectArg(Registers r, int of = 0) : AsmRegArg(r), offset(of) {}
    virtual
    string generate() const;
    bool usedRegister(Registers r) const;
    bool isMemory() const;
};
AsmStrArg* makeString(const string &name);
class AsmDup : public AsmArgs{
private:
	int count;

public:
	AsmDup(int c) : count(c) {}
	string generate() const;

};
class AsmInstr{
public:
    Commands cmd;

    AsmInstr(){}
	AsmInstr(Commands c):cmd(c){}
	virtual string get_str() const;
	virtual string generate() const;
    virtual bool usedRegister(Registers r) const { return false; }
    virtual bool changeStack() const { return false; }
    virtual bool isJump() const { return false; }
    virtual bool operateWith(AsmArgs* arg) const { return false; }
    virtual bool operationWith(AsmArgs* a) const { return false; }
};

class AsmUno:public AsmInstr{
    AsmArgs *arg1;
public:
    AsmUno(){}
    AsmUno(Commands c,AsmArgs* arg) : AsmInstr(c) ,arg1(arg) {}
    virtual string generate() const;
    AsmArgs* argument();
    bool changeStack()const;

};
class AsmDouble:public AsmInstr{
    AsmArgs *arg1;
    AsmArgs *arg2;
public:
    AsmDouble(){}
    AsmDouble(Commands c,AsmArgs *arg1, AsmArgs *arg2 ) : AsmInstr(c),arg1(arg1), arg2(arg2) {}
    virtual string generate() const;


};
class AsmIO:public AsmInstr{
    AsmArgs* arg;
    string command;
    AsmMemoryArg* format;
public:

    AsmIO(string m, AsmMemoryArg *f, AsmArgs* a);

    string generate() const;
    bool changeStack() const{return true;}



};

class AsmInstrLabel:public AsmInstr{
public:
    AsmLabelArg* label;

    AsmInstrLabel(AsmLabelArg *l) : label(l) {}
    virtual string generate() const;

};

class AsmCode{
    vector<AsmInstr*> commands;
public:
    AsmCode() : commands(0) {}
    int size() const;
    void fflush() const;
    void replace(int index, AsmInstr *cmd);
    void deleteRange(int from, int to);
    void insert(AsmInstr *cmd, int index);
    void fflush(ofstream  &out) const;
    void move(int from,  int to);
    AsmInstr* operator [] (int index) const;
    AsmCode& operator << (AsmInstr *command);
    AsmCode& add(Commands c);
    AsmCode& add(Commands c, AsmArgs *arg);
    AsmCode& add(Commands c, AsmArgs *arg1, AsmArgs *arg2);
    AsmCode& add(Commands c, Registers r);
    AsmCode& add(Commands c, Registers r1, Registers r2);
    AsmCode& add(Commands c, Registers r, string value);
    AsmCode& add(AsmLabelArg *label);
    AsmCode& add(string val, AsmMemoryArg *format, AsmArgs *arg = 0);

};

class Generator{
private:
    string file;
    AsmCode data;
    AsmCode code;
	public:
    friend class Parser;
    Generator(const string &name) : file(name) {
	}
    void generate();

};
#endif /* defined(__compiller__AsmCode__) */
