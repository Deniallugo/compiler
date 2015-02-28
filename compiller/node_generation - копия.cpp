//
//  node_generation.cpp
//  compiller
//
//  Created by Данил on 18.12.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "Node.h"
#include "Symbol.h"
#include"AsmCode.h"


void IntNode::generate(AsmCode &code){
    code.add(_PUSH, new AsmIntArg(token->Value));
}


string FloatNode::constName() const { return 0; }

void FloatNode::generate(AsmCode &code){
    code.add(_PUSH, new AsmMemoryArg(constName()));
}

void FloatNode::generateLvalue(AsmCode &code){}


void IdentifierNode::generate(AsmCode &code) {
    int size = var->byteSize();
    int steps = size / 4 + (size % 4 != 0);
    if (var->global)
        for (int i = 0; i < steps; i++)
            code.add(_PUSH, new AsmMemoryArg("dword ptr [var_" + var->name + " + " + to_string(4 * (steps - i - 1)) +"]"));
    else
        for (int i = 0; i < steps; i++)
            code.add(_PUSH, new AsmIndirectArg(_EBP, var->offset + 4 * (steps - i - 1)));
}

void Block::generate(AsmCode &code){
    for(int i = 0; i < size(); i++){
        body[i]->generate(code);
        if(body[i]->isExpression()){
            SymbolType *type = body[i]->getType();
            if(type && type->byteSize())
                code.add(_POP, _EAX);
        }
    }
}


void IdentifierNode::generateLvalue(AsmCode &code) {
	if (var->global)
		code.add(_PUSH, new AsmMemoryArg("var_" + var->name, true));
	else
		code.add(_MOV, _EAX, _EBP)
		.add(_MOV, _EBX, to_string(var->offset))
		.add(_ADD, _EAX, _EBX);
        code.add(_PUSH, _EAX);
}


void ArrNode::generate(AsmCode &code){
    generateLvalue(code);
	code.add(_PUSH, new AsmIndirectArg(_EAX))
		.add(_POP, _EAX)
		.add(_POP, _ECX)
		.add(_PUSH, new AsmRegArg(_EAX));

}

void ArrNode::generateLvalue(AsmCode &code){
    SymbolType *type = name->getType();
    if(dynamic_cast<ArraySymbol*>(type))
        name->generateLvalue(code);
    else if(dynamic_cast<PointerSymbol*>(type))
        name->generate(code);
    SymbolType *t = type->upType();
    for(int i = 0; i < args.size(); i++){
        args[i]->generate(code);
        code.add(_POP, _EAX)
        .add(_MOV, _EBX, to_string(t->byteSize()))
        .add(_IMUL, _EAX, _EBX)
        .add(_POP, _EBX)
        .add(_ADD, _EAX, _EBX)
        .add(_PUSH, _EAX);
        t = t->upType();
    }
}


void StringNode::generateData(AsmCode &code){


	code.add(_DB, new AsmMemoryArg("str" + to_string(index)), makeString("\"" + token->Value + "\""));
}

void StringNode::generate(AsmCode &code){
	code.add(_PUSH, new AsmMemoryArg("offset str" + to_string(index), true));
}



void FuncCallNode::generate(AsmCode& code){
	code.add(_SUB, _ESP, to_string(symbol->value->byteSize()));
	for (int i = args.size() - 1; i > -1; i--)
		args[i]->generate(code);
	code.add(_CALL, new AsmLabelArg("f_" + name->token->Value))
		.add(_ADD, _ESP, to_string(symbol->params->byteSize()));
}



void CharNode::generate(AsmCode &code){
		code.add(_PUSH, new AsmCharArg(token->Value));
	}

void IONode::generate(AsmCode& code){
	int size = 0;

	for (int i = args.size() - 1; i >= 0; i--) {
		size += args[i]->getType()->byteSize();
		size = (size / 4 + (size % 4 != 0)) * 4;

		args[i]->generate(code);
	}

	format->generate(code);

	size += format->getType()->byteSize();
	size = (size / 4 + (size % 4 != 0)) * 4 ;
	code.add(_CALL, new AsmMemoryArg("dword ptr __imp__printf"));

	code.add(_ADD, _ESP, to_string(size));
}

void FuncSymbol::generate(AsmCode &code, const string &str) const {
	code.add(new AsmLabelArg("f_" + str))
		.add(_PUSH, _EBP)
		.add(_MOV, _EBP, _ESP);
	body->generate(code);
	code.add(_MOV, _ESP, _EBP)
		.add(_POP, _EBP)
		.add(_RET, new AsmIntArg("0"));
}


void VarSymbol::generate(AsmCode &code) const {
    int size = type->byteSize();
    code.add(_DD, new AsmMemoryArg("var_" + name), new AsmDup(size / 4 + (size % 4 != 0)));
}

void SymTable::generateGlobals(AsmCode &code) const {
    for(int i = 0; i < size(); i++){
        VarSymbol *sym = dynamic_cast<VarSymbol*>(sym_ptr[i]);
        if(sym && !sym->type->isFunc())
            sym->generate(code);
    }
}


void SymTable::generateCode(AsmCode &code) const {
    for(int i = 0; i < size(); i++){
        VarSymbol* sym = dynamic_cast<VarSymbol*>(sym_ptr[i]);
        if (sym && dynamic_cast<FuncSymbol*>(sym->type))
            dynamic_cast<FuncSymbol*>(sym->type)->generate(code, sym->name);
    }
}

static void generateCmp(AsmCode & code, Commands cmd) {
	code.add(_POP, _EBX)
		.add(_POP, _EAX)
		.add(_XOR, _ECX, _ECX)
		.add(_CMP, _EAX, _EBX)
		.add(cmd, _CL)
		.add(_PUSH, _ECX);
}


static void generateOperation(AsmCode & code, Commands cmd, Registers reg = _EAX) {
	code.add(_POP, _EBX)
		.add(_POP, _EAX)

		.add(_XOR, _EDX, _EDX);

	if (cmd == _IDIV || cmd == _IMUL) code.add(cmd, _EBX);
	else code.add(cmd, _EAX, _EBX);

	code.add(_PUSH, reg);
}

static void generateOperationIncDec(SymbolType* type, AsmCode & code, Commands cmd) {
	code.add(_POP, _EAX);

	if (type->getType() == CharType) {
		code.add(_XOR, _EBX, _EBX)
			.add(_MOV, new AsmRegArg(_BL), new AsmIndirectArg(_EAX));

	}
	else {
		code.add(_MOV, new AsmRegArg(_EBX), new AsmIndirectArg(_EAX));
	}

	code.add(cmd, _EBX);

	if (type->getType() == CharType) {
		code.add(_MOV, new AsmIndirectArg(_EAX), new AsmRegArg(_BL))
			.add(_XOR, _EBX, _EBX)
			.add(_MOV, new AsmRegArg(_BL), new AsmIndirectArg(_EAX))
			.add(_PUSH, new AsmRegArg(_EAX));


	}
	else {
		code.add(_MOV, new AsmIndirectArg(_EAX), new AsmRegArg(_EBX));
	}

	code.add(_PUSH, _EBX);
}

static void generateOperationShift(AsmCode & code, Commands cmd) {
	code.add(_POP, _EAX)
		.add(_POP, _EBX)
		.add(_MOV, _ECX, _EAX)
		.add(cmd, _EBX, _CL)
		.add(_PUSH, _EBX);
}

static void generateOperationPointer(AsmCode & code, Commands cmd, int shift, bool rightPointerFlag) {
	Registers reg1 = _EAX, reg2 = _EBX;
	if (rightPointerFlag) {
		reg1 = _EBX;
		reg2 = _EAX;
	}

	code.add(_POP, _EAX)
		.add(_POP, _EBX)
		.add(_MOV, _ECX, to_string(shift))
		.add(_IMUL, reg1, _ECX)
		.add(cmd, reg2, reg1)
		.add(_PUSH, reg2);
}

static void generateOperationLogic(AsmCode & code, Commands cmd) {
	code.add(_POP, _EBX)
		.add(_POP, _EAX)

		.add(_CMP, _EAX, 0)
		.add(_MOV, _EAX, 0)
		.add(_SETNE, _AL)

		.add(_CMP, _EBX, 0)
		.add(_MOV, _EBX, 0)
		.add(_SETNE, _BL)

		.add(cmd, _EAX, _EBX)

		.add(_CMP, _EAX, 0)
		.add(_MOV, _EAX, 0)
		.add(_SETNE, _AL)
		.add(_PUSH, _EAX);
}

static void generateOperationAssing(SymbolType * type, AsmCode & code, Commands cmd, Registers reg = _EAX) {
	code.add(_POP, _EBX)
		.add(_POP, _ECX);

	if (type->getType() == CharType) {
		code.add(_XOR, _EAX, _EAX)
			.add(_MOV, new AsmRegArg(_AL), new AsmIndirectArg(_ECX));

	}
	else {
		code.add(_MOV, new AsmRegArg(_EAX), new AsmIndirectArg(_ECX));
	}

	code.add(_XOR, _EDX, _EDX);

	if (cmd == _IDIV || cmd == _IMUL) code.add(cmd, _EBX);
	else code.add(cmd, _EAX, _EBX);
	if (type->getType() == CharType) {
	Registers byteReg = reg == _EAX ? _AL : _DL;
		code.add(_MOV, new AsmIndirectArg(_ECX), new AsmRegArg(byteReg))
			.add(_XOR, reg, reg)
			.add(_MOV, new AsmRegArg(byteReg), new AsmIndirectArg(_ECX));

	}
	else {
		code.add(_MOV, new AsmIndirectArg(_ECX), new AsmRegArg(reg));
	}

	code.add(_PUSH, reg);
}


void BinOpNode::generate(AsmCode & code) {
	SymbolType * leftType = left->getType();
	SymbolType * rightType = right->getType();

	ArraySymbol* leftTypeArray = dynamic_cast<ArraySymbol *>(leftType);
	ArraySymbol* rightTypeArray = dynamic_cast<ArraySymbol *>(rightType);
	PointerSymbol* leftTypePointer = dynamic_cast<PointerSymbol *>(leftType);
	PointerSymbol* rightTypePointer = dynamic_cast<PointerSymbol *>(rightType);



	if (isAssing(token)) {
		left->generateLvalue(code);
		right->generate(code);

	}
	else {
		left->generate(code);
		right->generate(code);
	}

	if (token->Value == "+"){
		if (leftTypePointer || rightTypeArray || leftTypeArray || rightTypePointer) {
			int shift;

			if (leftTypeArray) shift = leftTypeArray->getType()->offset;
			if (rightTypeArray) shift = rightTypeArray->getType()->offset;
			if (leftTypePointer) shift = leftTypePointer->getType()->offset;
			if (rightTypePointer) shift = rightTypePointer->getType()->offset;

			if (leftTypePointer) generateOperationPointer(code, _ADD, shift, false);
			else generateOperationPointer(code, _ADD, shift, true);

		}

		generateOperation(code, _ADD);

	}
	else if (token->Value == "-"){
		if (leftTypePointer || rightTypeArray || leftTypeArray || rightTypePointer) {
			int shift;

			if (leftTypeArray) shift = leftTypeArray->getType()->offset;
			if (rightTypeArray) shift = rightTypeArray->getType()->offset;
			if (leftTypePointer) shift = leftTypePointer->getType()->offset;
			if (rightTypePointer) shift = rightTypePointer->getType()->offset;

			if (leftTypePointer && !rightTypePointer) generateOperationPointer(code, _SUB, shift, false);


		}

		generateOperation(code, _SUB);

	}

	else if (token->Value == "*"){
		generateOperation(code, _IMUL);

	}

	else if (token->Value == "/"){

		generateOperation(code, _IDIV);

	}

	else if (token->Value == "%"){

		generateOperation(code, _IDIV, _EDX);

	}

	else if (token->Value == "&"){
		generateOperation(code, _AND);
	}

	else if (token->Value == "|"){
		generateOperation(code, _OR);
	}

	else if (token->Value == "^"){
		generateOperation(code, _XOR);

	}

	else if (token->Value == "<<"){
		generateOperationShift(code, _SHL);

	}

	else if (token->Value == ">>"){
		generateOperationShift(code, _SHR);

	}

	else if (token->Value == "=="){

		generateCmp(code, _SETE);

	}

	else if (token->Value == ">="){

		generateCmp(code, _SETGE);
	}

	else if (token->Value == "<="){
		generateCmp(code, _SETLE);
	}

	else if (token->Value == "!="){

		generateCmp(code, _SETNE);

	}

	else if (token->Value == ">"){
		generateCmp(code, _SETG);
	}

	else if (token->Value == "<"){

		generateCmp(code, _SETL);
	}

	else if (token->Value == "&&"){
		generateOperationLogic(code, _AND);

	}

	else if (token->Value == "||"){

		generateOperationLogic(code, _OR);
	}

	else if (token->Value == "="){
		code.add(_POP, _EBX)
			.add(_POP, _EAX);

		if (!(leftTypePointer || leftTypeArray) && leftType->getType() == CharType) {
			code.add(_MOV, new AsmIndirectArg(_EAX), new AsmRegArg(_BL))
				.add(_XOR, _EBX, _EBX)
				.add(_MOV, new AsmRegArg(_BL), new AsmIndirectArg(_EAX))
				.add(_PUSH, new AsmRegArg(_EAX));
		}
		else {
			code.add(_MOV, new AsmIndirectArg(_EAX), new AsmRegArg(_EBX));
		}

		code.add(_PUSH, _EBX);


	}

	else if (token->Value == "+="){
		if ((leftTypePointer || leftTypeArray)) {
			int shift;
			if (leftTypeArray) shift = leftTypeArray->getType()->offset;
			if (rightTypeArray) shift = rightTypeArray->getType()->offset;
			if (leftTypePointer) shift = leftTypePointer->getType()->offset;
			if (rightTypePointer) shift = rightTypePointer->getType()->offset;
			generateOperationPointer(code, _ADD, shift, false);
		}

		generateOperationAssing(leftType, code, _ADD);

	}

	else if (token->Value == "-="){
		generateOperationAssing(leftType, code, _SUB);

	}

	else if (token->Value == "*="){
		generateOperationAssing(leftType, code, _IMUL);

	}

	else if (token->Value == "/="){
		generateOperationAssing(leftType, code, _IDIV);

	}

	else if (token->Value == "%="){
		generateOperationAssing(leftType, code, _IDIV, _EDX);

	}

	else if (token->Value == "&="){
		generateOperationAssing(leftType, code, _AND);

	}

	else if (token->Value == "|="){

		generateOperationAssing(leftType, code, _OR);

	}

	else if (token->Value == "^="){
		generateOperationAssing(leftType, code, _XOR);

	}

	else if (token->Value == ","){
		code.add(_POP, _EAX)
			.add(_POP, _EBX)
			.add(_PUSH, _EAX);

	}

}


void BinOpNode::generateLvalue(AsmCode& code){
	string value = token->Value;
	if (value == "+"){
		
			SymbolType *type = left->getType();
		if (dynamic_cast<ArraySymbol*>(type))
			left->generateLvalue(code);
		else if (dynamic_cast<PointerSymbol*>(type))
			left->generate(code);
		SymbolType *t = type->upType();
			right->generate(code);
		
			code.add(_POP, _EAX)
				.add(_MOV, _EBX, to_string(t->byteSize()))
				.add(_IMUL, _EAX, _EBX)
				.add(_POP, _EBX)
				.add(_ADD, _EAX, _EBX)
				.add(_PUSH, _EAX);
			t = t->upType();
		
		
	}
	else	if (isAssing(token)){ 
		generate(code);
		code.add(_POP, _EAX);
		left->generateLvalue(code);
	}
	else
		throw MyException("Compiler error");
}

void UnOpNode::generate(AsmCode & code) {
	SymbolType* type = operand->getType();


	string op = token->Value;

	if (op == "++" || op == "--" || op == "*" || op == "&") 
		operand->generateLvalue(code);
	else 
		operand->generate(code);

	if (token->Value == "*"){
		if (dynamic_cast<PointerSymbol*>(type))
			code.add(_POP, _EAX)
				.add(_MOV, new AsmRegArg(_EBX), new AsmIndirectArg(_EAX))
				.add(_PUSH, _EBX);
		else
			code.add(_POP, _EAX)
				.add(_MOV, new AsmRegArg(_EBX), new AsmIndirectArg(_EAX))
				.add(_PUSH, new AsmIndirectArg(_EBX));
		
	}



	else if (token->Value == "!"){

		code.add(_POP, _EAX)
			.add(_XOR, _EBX, _EBX)
			.add(_CMP, _EAX, 0)
			.add(_SETE, _BL)
			.add(_PUSH, _EBX);

	}


	else if (token->Value == "+"){

	}

	else if (token->Value == "-"){
		code.add(_POP, _EAX)
			.add(_NEG, _EAX)
			.add(_PUSH, _EAX);

	}

	else if (token->Value == "++"){
		generateOperationIncDec(type, code, _INC);

	}

	else if (token->Value == "--"){
		generateOperationIncDec(type, code, _DEC);


	}
}
void UnOpNode::generateLvalue(AsmCode & code) {
	string op = token->Value;

	if (op == "*" || op == "&") {
		operand->generateLvalue(code);
		return;
	}

}
