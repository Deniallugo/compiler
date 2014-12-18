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
    _BL,
    _AX
};

class AsmArgs{
public:
    virtual string gen_string()  const {return  0;}
    virtual bool usedRegistr(Registers reg){return  false;}
    virtual void clearOffset() {}


};


class AsmStrArg : public  AsmArgs{
    string value;
public:
    AsmStrArg(string _value) : value(_value){}
    string gen_string() const;

};

class AsmRegArg:public AsmArgs{
    Registers reg;
public:
    AsmRegArg(Registers _reg): reg(_reg){}
    string gen_str() const;
    bool usedRegistr(Registers reg);


};

class AsmMemoryArg: public AsmArgs{
    string name;
    bool lvalue;
public:
    AsmMemoryArg(string str, bool _value = false): name(str), lvalue(_value){}
    string gen_str();
    void clearOffset();
};

class AsmLabelArg: AsmArgs{
    string label;
public:
    AsmLabelArg(string _label):label(_label){}
    string gen_str();
    string get_name();

};

class AsmFloatArg: public AsmArgs{
    float value;
public:
    AsmFloatArg (float _value): value(_value){}
    string gen_str();

};
class AsmIntArg: public AsmArgs{
    int value;
public :
    AsmIntArg(int _value):value(_value){}
    string gen_str();

};

class AsmInstr{
public:
    Commands cmd;

    AsmInstr(){}
    AsmInstr(Commands c):cmd(c){}
    virtual string gen_str() const;
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
    virtual string gen_str() const;
    AsmArgs* argument();
    bool changeStack()const;

};
class AsmDouble:public AsmInstr{
    AsmArgs *arg1;
    AsmArgs *arg2;
public:
    AsmDouble(){}
    AsmDouble(Commands c,AsmArgs *arg1, AsmArgs *arg2 ) : AsmInstr(c),arg1(arg1), arg2(arg2) {}
    virtual string gen_str() const;


};
class AsmIO:public AsmInstr{
    vector<AsmArgs*> args;
    string command;
    AsmMemoryArg* format;
public:
    AsmIO(string m, AsmMemoryArg *f, vector<AsmArgs*> a);
    AsmIO(string m, AsmMemoryArg *f, AsmArgs* a);

    string gen_str() const;
    bool changeStack() const{return true;}



};

class AsmInstrLabel:public AsmInstr{
public:
    AsmLabelArg* label;

    AsmInstrLabel(AsmLabelArg *l) : label(l) {}
    virtual string gen_str() const;




};
class AsmCode{
    vector<AsmInstr*> commands;
public:
    AsmCode() : commands(0) {}
    int size() const;
    void fflush(ofstream  &out) const;
    void replace(int index, AsmInstr *cmd);
    void deleteRange(int from, int to);
    void insert(AsmInstr *cmd, int index);
    void move(int from,  int to);
    AsmInstr* operator [] (int index) const;
    AsmCode& operator << (AsmInstr *command);
    AsmCode& add(Commands c);
    AsmCode& add(Commands c, AsmArgs *arg);
    AsmCode& add(Commands c, AsmArgs *arg1, AsmArgs *arg2);
    AsmCode& add(Commands c, Registers r);
    AsmCode& add(Commands c, Registers r1, Registers r2);
    AsmCode& add(Commands c, Registers r, int value);
    AsmCode& add(AsmLabelArg *label);
    AsmCode& add(string val, AsmMemoryArg *format, AsmArgs *arg = 0);

};

#endif /* defined(__compiller__AsmCode__) */
