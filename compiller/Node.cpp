//
#include "Node.h"
#include <fstream>


ScalarSymbol *IntType = new ScalarSymbol("int", _INTEGER);
ScalarSymbol *FloatType = new ScalarSymbol("float", _FLOAT);
ScalarSymbol * CharType = new ScalarSymbol("char", _CHAR);
ScalarSymbol *VoidType = new ScalarSymbol("void", _VOID);
PointerSymbol *StringType = new PointerSymbol(CharType);


map<SymbolType*, int> typePriority;
map<string, SymbolType*> operationTypeOperands;
map<string, SymbolType*> operationReturningType;





OpNode :: OpNode(Token *op) : ExprNode(op) {}

string OpNode :: opName() const{
    return token->Value ;
}

ExprNode *ExprNode :: makeTypeCoerce(ExprNode* expr, SymbolType *from, SymbolType *to, Token* t){
    vector<SymbolType*> s;
    ArraySymbol* ar = dynamic_cast<ArraySymbol*>(from);

    if ( ar != nullptr){
        if(!ar->canConvertTo(to,t)){
            throw MyException("Cannot perform conversion");

        }
    }
    else
        if(!from->canConvertTo(to))
            throw MyException("Cannot perform conversion");
    if(from == to || *from == to)
        return expr;
    if (!dynamic_cast<ScalarSymbol*>(from) || !dynamic_cast<ScalarSymbol*>(to))
        return expr;
    else {
        if(typePriority[to] - typePriority[from] == 1){

            s.push_back(to);
            return new CastNode(NULL, expr, &s);
        }
        s.push_back(FloatType);
        return new CastNode(NULL, makeTypeCoerce(expr, from, IntType), &s);
    }
}

BinOpNode :: BinOpNode(Token *t, ExprNode *l, ExprNode *r) : OpNode(t), left(l), right(r) {
    if((l == 0) || (r == 0))
        throw MyException("Lost operand", t->line, t->num);
}

void BinOpNode :: print(int deep) const{
    left->print(deep + 1);
    cout << string(deep * 2, ' ') << opName() << endl;
    right->print(deep + 1);
}


void BinOpNode :: print(ofstream *f, int deep){
    left->print(f, deep + 1);
    *f << string(deep * 2, ' ') << opName() << endl;
    right->print(deep + 1);
}




bool BinOpNode :: isLvalue() const{
    if (isEq(token, _OPERATION, ".") || isEq(token, _OPERATION, "->"))
        return right->isLvalue();

    else
        return isModifiableLvalue();
}


bool BinOpNode :: isModifiableLvalue() const{
    if (isAssing(token) || isEqual(token))
        return left->isModifiableLvalue();
    if (isEq(token, _OPERATION, ".") || isEq(token, _OPERATION, "->"))        return right->isModifiableLvalue();
    else    return false;

}

SymbolType *BinOpNode :: getType(){
    SymbolType *leftType = left->getType();
    if ( dynamic_cast<TypedefSymbol*>(leftType))
        leftType = leftType->getType();

    SymbolType *rightType = right->getType();
    if ( dynamic_cast<TypedefSymbol*>(rightType))
        leftType = rightType->getType();
    string operation = token->Value;
    SymbolType *maxType = 0;
    if(operationTypeOperands.count(operation))
        maxType = operationTypeOperands[operation];
    else
        maxType = typePriority[leftType] > typePriority[rightType] ? leftType : rightType;
    PointerSymbol *l_pointer = dynamic_cast<PointerSymbol*>(leftType);
    PointerSymbol *r_pointer= dynamic_cast<PointerSymbol*>(rightType);
    ArraySymbol *l_array = dynamic_cast<ArraySymbol*>(leftType);
    ArraySymbol *r_array = dynamic_cast<ArraySymbol*>(rightType);
    StructSymbol * l_struct =dynamic_cast<StructSymbol*>(leftType);
    StructSymbol * r_struct = dynamic_cast<StructSymbol*>(rightType);
    //FuncSymbol * fun = dynamic_cast<FuncSymbol*>(right);
    if(isEqual(token)){

        if(l_struct && r_struct){
            if ((l_struct->getType() != r_struct->getType()))
                throw MyException("Expression must have integral type", token);
            else return IntType;
        }
        if(l_pointer && r_pointer){
            if(l_pointer->getType() != r_pointer->getType())
               throw MyException("Expression must have integral type", token);
            else return IntType;

        }
        if(
           ((l_pointer|| l_array) && (rightType != IntType)) ||
            ((r_pointer || r_array) && (leftType != IntType))
           //||!leftType->canConvertTo(IntType) || !rightType->canConvertTo(IntType)


           )
            throw MyException("Expression must have integral type", token);
        

        return IntType;

    }


    if(isAssing(token)){

        if (l_struct && r_struct){
            if(!isEq(token, _OPERATION, "="))
                throw MyException("Can't perform operation over two  structures",token);


            if(l_struct->getType() != r_struct->getType())
                throw MyException("Struct  must have integral type",token);
            return l_struct->getType();
        }
        if(!left->isModifiableLvalue())
            throw MyException("Expression must be a modifiable lvalue", token);
        if (((l_array||l_pointer)&&(r_array||r_pointer))
            && (isEq(token, _OPERATION, "=")||((isEq(token, _OPERATION, "-="))))){
            left = makeTypeCoerce(left,  leftType, rightType,token);
            return rightType;
        }
        if((l_pointer && rightType->canConvertTo(IntType)) &&!isEq(token, _OPERATION, "="))
            return l_pointer == 0 ? r_pointer : l_pointer;

        if(l_array  && rightType->canConvertTo(IntType) &&!isEq(token, _OPERATION, "=") )
            return new PointerSymbol(l_array == 0 ? r_array : l_array);


    }
    if (isEq(token, _OPERATION, ".")) {

        //            if(!dynamic_cast<StructSymbol*>(leftType))
        //                throw MyException("Left operand of . must be a structure", token);
        return rightType;
    }
    if (isEq(token, _OPERATION, "->")) {
        if(!l_pointer || !dynamic_cast<StructSymbol*>(l_pointer->upType()))
            throw MyException("Left operand of -> must be of pointer-to-structure type");
        return rightType;
    }
    if (isEq(token, _OPERATION, "-")) {
        if((l_pointer && r_pointer) || (l_array && r_array)){
            if( (l_pointer && r_pointer && (*l_pointer->pointer != r_pointer->pointer))
               || (l_array && r_array && (*l_array->type != r_array->type)))
                throw MyException("Operand types are incompatible", token);
            return IntType;
        }
        if((l_pointer && rightType->canConvertTo(IntType)))
            return l_pointer == 0 ? r_pointer : l_pointer;

        if(l_array  && rightType->canConvertTo(IntType) )
            return new PointerSymbol(l_array == 0 ? r_array : l_array);



    }

    if(isEq(token, _OPERATION, "+")) {
        if((l_pointer || l_array) && (r_pointer || r_array))
            throw MyException("Can't add two pointers");
        if((l_pointer && rightType->canConvertTo(IntType)))
            return l_pointer == 0 ? r_pointer : l_pointer;

        if(l_array  && rightType->canConvertTo(IntType) )
            return new PointerSymbol(l_array == 0 ? r_array : l_array);


    }


    if ( r_pointer|| l_pointer || l_array || r_array)
        throw MyException(" Operation not for pointer",token);


    if(leftType->isStruct() || rightType->isStruct())
        throw MyException("Can't perform operation with  structures", token);
    if(typePriority[maxType] > max(typePriority[leftType], typePriority[rightType]))
        throw MyException("Invalid type of operands", token);
    left = makeTypeCoerce(left, leftType, maxType);
    right = makeTypeCoerce(right, rightType, maxType);
    if (operationReturningType.count(operation))
        return operationReturningType[operation];
    else
        return maxType;

}

UnOpNode ::UnOpNode(Token *t, ExprNode *op) : OpNode(t), operand(op){};


void UnOpNode :: print(int deep) const{
    cout << string(deep * 2, ' ') << opName() << endl;
    operand->print(deep + 1);
}

void UnOpNode :: print(ofstream *f, int deep) const{
    *f << string(deep * 2, ' ') << opName() << endl;
    operand->print(f, deep + 1);
}

bool UnOpNode :: isLvalue() const{
    string operation = token->Value;
    return (operation == "!" || operation == "++" || operation == "--") && operand->isLvalue();
}

static bool TypeLValue(const UnOpNode *n){
    UnOpNode *node = const_cast<UnOpNode*>(n);
    return node->getType()->isModifiableLvalue();
}

bool UnOpNode :: isModifiableLvalue() const{
    string operation = token->Value;
    return (operation == "!" || operation == "++" ||operation == "*" || operation == "--") && TypeLValue(this);
}


SymbolType *UnOpNode::getType(){
    SymbolType *type = operand->getType();
    string operation = token->Value;

    if (isEq(token, _OPERATION,"!" )){//factor
        if(!dynamic_cast<PointerSymbol*>(type))
            throw MyException("Type of unary operation is not a pointer", token);

        return dynamic_cast<PointerSymbol*>(type)->pointer;
    }
    if (isEq(token, _OPERATION, "&" )){
        if(!operand->isLvalue())
            throw MyException("Expression must be a modifiable lvalue", token->line, token->num);
        return new PointerSymbol(type);
    }
    if(isEq(token, _OPERATION, "*" )){
       if( dynamic_cast<ArraySymbol*>(operand->getType()))
        return new PointerSymbol(type->getType());
        else
            throw MyException("Operand is not a pointer", token);

    }
    if (isEq(token, _OPERATION, "^" ))
        operand = makeTypeCoerce(operand, type, IntType);

    if (isEq(token, _OPERATION,"!" )){
        if(dynamic_cast<StructSymbol*>(type))
            throw MyException("No operator \"!\" matches these operands operand types are: !" + type->name, token);
    }

    if (isEq(token, _OPERATION,"++" ) || isEq(token, _OPERATION,"--" )){

        if(!operand->isModifiableLvalue())

            throw MyException("Expression must be a modifiable lvalue", token);
    }
    if (isEq(token, _OPERATION,"-")){
        if(!type->canConvertTo(FloatType))
            throw MyException("Expression must have arithmetical type", token);
    }

    return type;

}

PostfixUnaryOpNode :: PostfixUnaryOpNode(Token *t, ExprNode *op) : UnOpNode(t, op) {};

bool PostfixUnaryOpNode :: isLvalue() const{
    return false;
}

bool PostfixUnaryOpNode :: isModifiableLvalue() const{
    return false;
}


void PostfixUnaryOpNode :: print(int deep) const{
    operand->print(deep);
    cout << string(deep * 2, ' ') << opName() << endl;
}

void PostfixUnaryOpNode :: print(ofstream *f, int deep) const{
    operand->print(deep);
    *f << string(deep * 2, ' ') << opName() << endl;
}

IdentifierNode :: IdentifierNode(Token *t, VarSymbol *v) : ExprNode(t), var(v) {}

IdentifierNode :: IdentifierNode(const string & name, int line, int col) : ExprNode(new Token("identifer", _IDENTIFIER, name, "identifier :" + name, col, line)) {}

SymbolType* IdentifierNode :: getType(){
    return var->getType();
}

bool IdentifierNode :: isLvalue() const{
    return true;
}

bool IdentifierNode :: isModifiableLvalue() const{
    SymbolType *type = dynamic_cast<VarSymbol*>(var)->type;
    return !dynamic_cast<ConstSymbolType*>(type) && !dynamic_cast<FuncSymbol*>(type)
    && !dynamic_cast<StructSymbol *>(type) ;
}
void IdentifierNode :: print(int deep) const{
    cout << string(deep * 2, ' ') << token->Value << endl;
}

void IdentifierNode :: print(ofstream *f, int deep) const{
    *f << string(deep * 2, ' ') << token->Value << endl;
}

ArrNode :: ArrNode(ExprNode* arr,ExprNode* _arg): FunctionalNode(arr) ,arg(_arg) {}

bool ArrNode :: isLvalue() const{
    return true;
}
bool ArrNode :: isModifiableLvalue() const{
    SymbolType *type = name->getType();
    for(int i = 0; i < args.size(); i++)
        type = type->upType();
    return type->isModifiableLvalue();
}

SymbolType *ArrNode :: getType(){
    ArraySymbol *sym = dynamic_cast<ArraySymbol*>(name->getType());
    PointerSymbol* sym1 = dynamic_cast<PointerSymbol*>(name->getType());
    if(!sym && !sym1)
        throw MyException("Expression must be a pointer type");
    SymbolType *type = sym ? (SymbolType*)sym:(SymbolType*)sym1;
    for(int i = 0; i < args.size(); i++){
        type = type->upType();
        if(type == 0)
            throw MyException("Expression must be a pointer type", args[i]->token);
        if(!args[i]->getType()->canConvertTo(IntType))
            throw MyException("Expression must have integral type", args[i]->token);
        args[i] = makeTypeCoerce(args[i], args[i]->getType(), IntType);
    }
    return type;
}

void ArrNode :: print(int deep) const{
    name->print(deep);
    cout << string(deep * 2, ' ') << "[" << endl;
    printArgs(deep);
    cout << string(deep * 2, ' ') << "]" << endl;
}

void ArrNode :: print(ofstream *f, int deep) const{
    name->print(f, deep);
    *f << string(deep * 2, ' ') << "[" << endl;
    printArgs(f, deep);
    *f<< string(deep * 2, ' ') << "]" << endl;
}

TernaryOpNode :: TernaryOpNode(Token* op, ExprNode* c, ExprNode* l, ExprNode* r): BinOpNode(op, l, r), condition(c) {}

void TernaryOpNode :: print(int deep) const{
    string tab = string(deep * 2, ' ');
    cout << tab << "(" << endl;
    condition->print(deep + 1);
    cout << tab << ")" << endl;
    cout << tab << "?" << endl;
    left->print(deep + 1);
    cout << tab << ":" << endl;
    right->print(deep + 1);
}

void TernaryOpNode :: print(ofstream *f, int deep) const{
    string tab = string(deep * 2, ' ');
    *f << tab << "(" << endl;
    condition->print(f, deep + 1);
    *f << tab << ")" << endl;
    *f << tab << "?" << endl;
    left->print(f, deep + 1);
    *f << tab << ":" << endl;
    right->print(f, deep + 1);
}

FunctionalNode :: FunctionalNode(ExprNode *n) : name(n), args(0) {}

FunctionalNode :: FunctionalNode(Token *t, ExprNode *n, FuncSymbol *s) : ExprNode(t), name(n), symbol(s) {}

void FunctionalNode :: addArg(ExprNode* arg) {args.push_back(arg); }

void FunctionalNode :: printArgs(int deep) const{
    for (int i = 0; i < args.size(); i++){
        args[i]->print(deep + 1);
        if (i < args.size() - 1)
            cout << string(deep * 2, ' ') << ',' << endl;
    }
}

//TypeNode::TypeNode(string name, Symbol * type):DeclNode(type->getType()->type
//), m_name(name), m_type(type){}

SymbolType* FunctionalNode :: getType(){
    FuncSymbol *sym = dynamic_cast<FuncSymbol*>(symbol);
    if (sym == nullptr) throw MyException("Redefenition",token);
    int params_number = sym->params->size();
    if(params_number != args.size())
        throw MyException("Incorrect number of parameters", token);
    for(int i = 0; i< params_number; i++){
        SymbolType *realType = args[i]->getType();
        SymbolType *formalType = sym->params->sym_ptr[i]->getType();
        if(!realType->canConvertTo(formalType))
            throw MyException("Invalid type of parameter");
        args[i] = makeTypeCoerce(args[i], realType, formalType);
    }

//    return symbol->getType();
    return symbol;
}

void FunctionalNode :: printArgs(ofstream *f, int deep) const{
    for (int i = 0; i < args.size(); i++){
        args[i]->print(f, deep + 1);
        if (i < args.size() - 1)
            *f << string(deep * 2, ' ') << ',' << endl;

    }
}

void FunctionalNode :: print(ofstream *f, int deep) const{
    name->print(f, deep);
    *f << string(deep * 2, ' ') << "[" << endl;
    printArgs(f, deep);
    *f<< string(deep * 2, ' ') << "]" << endl;
}

void FunctionalNode :: print(int deep) const{
    name->print(deep);
    cout << string(deep * 2, ' ') << "(" << endl;
    printArgs(deep);
    cout << string(deep * 2, ' ') << ")" << endl;
}

CastNode :: CastNode(Token* op, ExprNode* oper, SymbolType* ts) : UnOpNode(op, oper),type(ts) {}

CastNode :: CastNode(Token *op, ExprNode *oper,  vector<SymbolType*> *ts) : UnOpNode(op, oper), s_type(ts) {
    type = s_type->back();
}


SymbolType *CastNode :: getType(){
    return type;

}

void CastNode :: print(int deep) const{
    cout << string(deep * 2, ' ') << type->typeName()<<endl;
    cout << string(deep * 2, ' ') << "(" << endl;
    operand->print(deep + 1);
    cout << string(deep * 2, ' ') << ")" << endl;
}

void CastNode :: print(ofstream *f, int deep) const{
    *f << string(deep * 2, ' ') << type->typeName();
    *f << string(deep * 2, ' ') << "(" << endl;
    operand->print(f, deep + 1);
    *f << string(deep * 2, ' ') << ")" << endl;
}


void Block :: print(int deep) const{
    string tab(deep * 2, ' ');
    if(body.size())
        cout << tab << "Statements:" << endl;
    for(int i = 0; i < body.size(); i++)
        body[i]->print(deep + 1);

    if(table->sym_ptr.size()){
        cout << tab << "Symbols Table:" << endl;
        table->print(deep + 1);
    }
    if(!table->sym_ptr.size() && !body.size())
        cout << tab << "<< block is empty >>" << endl;
}

void func_print(int deep, Block *b){
    b->print(deep + 1);
}

void print_node(int deep, ExprNode *n){
    n->print(deep + 1);
}




void IfStatement :: print(int deep) const{
    string tab(deep * 2, ' ');
    cout << tab << "if" << endl << endl;
    condition->print(deep + 1);
    cout << endl << tab << "then" << endl << endl;
    if_branch->print(deep + 1);
    if(else_branch){
        cout << tab << "else" << endl << endl;
        else_branch->print(deep + 1);
    }
}


void ForStatement :: print(int deep) const{
    string tab(deep * 2, ' ');
    cout << tab << "for (;;) .." << endl;
    cout << tab << "First action:" << endl;
    if(first_cond)
        first_cond->print(deep + 1);
    else
        cout << tab <<"none" << endl;
    cout << tab << "Stop condition:" << endl;
    if(second_cond)
        second_cond->print(deep + 1);
    else
        cout << tab << "none" << endl;
    cout << tab << "Step:" << endl;
    if(third_cond)
        third_cond->print(deep + 1);
    else
        cout << tab << "none" << endl;
    cout << tab << "Body:" << endl;
    if(body)
        body->print(deep + 1);
    else
        cout << tab << "Block is empty" << endl;
}

static void print_cycle(int deep, ExprNode *condition, ExprNode *body){
    string tab(deep * 2, ' ');
    cout << tab << "Condition:" << endl;
    condition->print(deep + 1);
    cout << tab << "Body:" << endl;
    if(body)
        body->print(deep + 1);
    else
        cout << tab << "<< block is empty >>" << endl;
}

void WhileStatement :: print(int deep) const{
    cout << string(deep * 2, ' ') << "while () .." << endl;
    print_cycle(deep, condition, body);
}

void DoWhileStatement :: print(int deep) const{
    cout << string(deep * 2, ' ') << "do .. while()" << endl;
    print_cycle(deep, condition, body);
}

void ContinueStatement :: print(int deep) const{
    cout << string(deep * 2, ' ') << "continue" << endl;
}

void BreakStatement :: print(int deep) const{
    cout << string(deep * 2, ' ') << "break" << endl;
}


void ReturnStatement :: print(int deep) const{
    cout << string(deep * 2, ' ') << "return" << endl;
    if(value)
        value->print(deep + 1);
}


IntNode :: IntNode(Token *t) : ExprNode(t) {}

void IntNode :: print(int deep) const{
    cout << string(deep * 2, ' ') << token->Value << endl;
}

void IntNode :: print(ofstream *f, int deep) const{
    *f << string(deep * 2, ' ') << token->Value << endl;
}

SymbolType *IntNode :: getType(){
    return IntType;
}

FloatNode :: FloatNode(Token *t) : ExprNode(t) {}

void FloatNode :: print(int deep) const{
    cout << string(deep * 2, ' ') << token->Value << endl;
}

void FloatNode :: print(ofstream *f, int deep) const{
    *f << string(deep * 2, ' ') << token->Value << endl;
}

SymbolType *FloatNode :: getType(){
    return FloatType;
}


CharNode :: CharNode(Token *t) : ExprNode(t) {}

SymbolType *CharNode :: getType(){
    return CharType;
}

void CharNode :: print(int deep) const{
    cout << string(deep * 2, ' ') << '\'' << token->Value << '\'' << endl;
}

void CharNode :: print(ofstream *f, int deep) const{
    *f << string(deep * 2, ' ') << '\'' << token->Value << '\'' << endl;
}

StringNode :: StringNode(Token *t) : ExprNode(t) {}

SymbolType* StringNode :: getType(){
    return StringType;
}

void StringNode :: print(int deep) const{
    cout << string(deep * 2, ' ') << '"' << token->Value << '"' << endl;
}

void StringNode :: print(ofstream *f, int deep) const{
    *f << string(deep * 2, ' ') << '"' << token->Value << '"' << endl;
}

SymbolType* IONode::getType(){
    return 0;
}

void IONode::print(int deep) const {

    string tab(deep * 2, ' ');
    cout << tab << token->Value << "(" << endl;
    format->print(deep + 1);
    if (args.size() > 0)
    {
        cout << tab << ',' << endl;
        printArgs(deep + 1);
    }
    cout << tab << ")" << endl;
}






