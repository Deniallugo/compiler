//
////
////  Parser.cpp
////  compiller
////
////  Created by Данил on 11.11.14.
////  Copyright (c) 2014 Данил. All rights reserved.
////
//
//#include <stdio.h>
//#include "Parser.h"
//
//bool Parser::isBaseType( Token* t){
//    return (t->Type == _FLOAT || t->Type == _INTEGER || t->Type == _CHAR);
//}
//
//
//void Parser::errorIf(bool isTrue, string error, Token* tk){
//    if( isTrue){
//        if (tk != NULL) {
//            throw MyException(error);
//        }
//        throw MyException(error, tk);
//    }
//}
//
//
//Parser :: Parser(Scanner &l) : scan(l), parsingFunc(0), struct_counter(0){
//
//    scan.Next();
//    symStack = new SymTableStack();
//    priorityTable["["] = priorityTable["]"] =
//    priorityTable["("] = priorityTable[")"] = priorityTable["."] = 15;
//
//    priorityTable["--"] = priorityTable["++"]  = priorityTable["!"] = 14;
//
//    priorityTable["%"] = priorityTable["/"] = priorityTable["*"] = 13;
//
//    priorityTable["-"] = priorityTable["+"] = 12;
//
//    priorityTable["<<"] = priorityTable[">>"] = 11;
//
//    priorityTable["<="] = priorityTable[">="] =
//    priorityTable[">"] = priorityTable["<"] = 10;
//
//    priorityTable["!="] = priorityTable["=="] = 9;
//
//    priorityTable["&"] = 8;
//
//    priorityTable["^"] = 7;
//
//    priorityTable["|"] = 6;
//
//    priorityTable["&&"] = 5;
//
//    priorityTable["||"] = 4;
//
//    priorityTable["?"] = 3;
//    priorityTable[":"] = 0;
//
//    priorityTable["^="] = priorityTable["|="] = priorityTable["&="] =
//    priorityTable["%="] = priorityTable["/="] = priorityTable["*="] =
//    priorityTable["-="]  = priorityTable["+="] = priorityTable["="] = 2;
//
//    priorityTable[","] = 1;
//
//    right["&"] = right["!"]  = right["^="] =
//    right["|="] = right["&="] = right["%="] = right["/="] =
//    right["*="] = right["-="] = right["+="] = right["="] = true;
//
//    unary["!"] = unary["*"] = unary["--"] = unary["++"] =
//    unary["&"] = unary["+"] = unary["-"] = true;
//
//    typePriority[CharType] = 3;
//    typePriority[IntType] = 2;
//    typePriority[FloatType] = 1;
//
//    operationTypeOperands["%"] = IntType;
//    operationTypeOperands["&"] = IntType;
//    operationTypeOperands["|"] = IntType;
//    operationTypeOperands["^"] = IntType;
//    operationTypeOperands["~"] = IntType;
//    operationTypeOperands[">>"] = IntType;
//    operationTypeOperands["<<"] = IntType;
//
//    operationReturningType["%"] = IntType;
//    operationReturningType["&"] = IntType;
//    operationReturningType["|"] = IntType;
//    operationReturningType["!"] = IntType;
//    operationReturningType[">>"] = IntType;
//    operationReturningType["<<"] = IntType;
//    operationReturningType["&&"] = IntType;
//    operationReturningType["||"] = IntType;
//    operationReturningType["^"] = IntType;
//    operationReturningType["~"] = IntType;
//    operationReturningType["=="] = IntType;
//    operationReturningType["!="] = IntType;
//    operationReturningType["<"] = IntType;
//    operationReturningType["<="] = IntType;
//    operationReturningType[">"] = IntType;
//    operationReturningType[">="] = IntType;
//
//    SymTable* basic_def = new SymTable();
//    basic_def->add_symbol(IntType);
//    basic_def->add_symbol(FloatType);
//    basic_def->add_symbol(CharType);
//    basic_def->add_symbol(VoidType);
//
//    global_field.table = new SymTable(*basic_def);
//    Block* bl = new Block(new SymTable());
//    symStack->push(basic_def);
//    blocks.push(new Block(basic_def));
//    blocks.push(bl);
//    symStack->push(bl->table);
//}
//
//
//
//void Parser :: ParseProgram(){
//    while (!scan.isEnd()){
//        if (scan.Get()->Type == _CONST || scan.Get()->Type == _STRUCT || dynamic_cast<SymbolType*>(symStack->find_symbol(scan.Get()->Value)) ||scan.Get()->Value == "double" )
//            ParseDeclaration();
//        else
//            global_field.body.push_back(ParseExpr());
//    }
////    if (!main_func) {
////        throw MyException("There аrе no entry point \n");
////    }
//
//
//}
//
//
//ExprNode* Parser  :: ParseDeclaration(SymbolType* sym_type){
//    if (isEq(scan.Get(), _SEPARATION, ";")){
//        scan.Next();
//        return nullptr;
//    }
//
//   
//    SymbolType* t_symbol;
//    if (!sym_type) {
//         t_symbol = ParseType();
//    }else
//    t_symbol = sym_type;
//
//    VarSymbol* var = nullptr;
//    BinOpNode *node = nullptr;
//    while(true){
//        Token *token;
//
//        token = scan.GetNext();
//        if(isEq(token, _SEPARATION, ";" )&& t_symbol->isStruct())
//            break;
//
//        if (isEq(scan.Get(), _SEPARATION,"("))
//            var =  ParseComplexDeclaration(t_symbol);
//        else
//            var = ParseIdentifier(t_symbol);
//
//        if(symStack->tables.back()->find_symbol(var->name) == 0)
//            symStack->add_symbol(var);
//
//        if(isEq(scan.Get(), _OPERATION, "=")){
//            Token *asgn = scan.Get();
//            scan.Next();
//           ExprNode* assign_operand = ParseExpr(priorityTable[","] + 1);
//
//            //   parsingFunc->body->AddStatement(assign)
//           node = new BinOpNode(asgn, new IdentifierNode(token, var), assign_operand);
//           node->getType();
//            blocks.top()->AddStatement(node);
//
//        }
//        if(isEq(scan.Get() ,_SEPARATION, ";") || isEq(scan.Get() ,_SEPARATION, "{"))
//            break;
//        errorIf(!isEq(scan.Get(), _SEPARATION, ","), "Comma Expected");
//    }
//        if(isEq (scan.Get(),_SEPARATION,"{")){
//            // symStack->push(new SymTable());
//
//            FuncSymbol *func = dynamic_cast<FuncSymbol*>(var->type);
//            errorIf(!func, "Unexpected brace", scan.Get());
//            errorIf((symStack->tables.size() != 2), "Can not define the function in the block", scan.Get());
//            parsingFunc = func;
//            func->body = ParseBlock();
//            parsingFunc = 0;
//            CheckReturn(func);
//            if(func->name == "main")
//                main_func = 1;
//            scan.Next();
//
//        }
//    else
//    scan.Next();
//    return node;
//}
//
//SymbolType * Parser :: ParseType(bool param){
//
//    Token *token = scan.Get();
//    SymbolType *type = nullptr;
//    bool Const = false;
//
//    while(token->Value == "const"){
//        Const = true;
//        token = scan.GetNext();
//    //    ParseDeclaration();
//    }
//    if (token->Value == "double"){
//        token = new Token("float", _FLOAT, "float", "float", token->num, token->line);
//
//    }
//
//
//    if(token->Value == "struct")
//        type = ParseStruct(param);
//    else {
//        type = dynamic_cast<SymbolType*>(symStack->find_type(token->Value));
//
//        errorIf(type == nullptr, "Unknown type", token);
//    }
//    if(Const){
//        type = new ConstSymbolType(type);
//    }
//    return type;
//}
//
//
//
//StructSymbol* Parser :: ParseStruct(bool param){
//    Token *token =scan.GetNext();
//
//    string name;
//    if(token->Type == _IDENTIFIER){
//        name = token->Value;
//        token =scan.GetNext();
//    } else
//        name = "unnamed struct " + to_string((long double)struct_counter++);
//
//    StructSymbol *struct_symbol = dynamic_cast<StructSymbol*>(symStack->find_symbol(name));
//    errorIf(param && !struct_symbol, "Unknown struct type");
//
//    if(param && !struct_symbol){
//        errorIf(isEq(token, _SEPARATION, "{"), "Using the definition type are not allowed", token);
//        throw MyException("Expected type's name", token);
//    }
//
//    if(!struct_symbol){
//        struct_symbol = new StructSymbol(0, name);
//        if(name.length() > 0)
//            symStack->add_symbol(struct_symbol);
//    }
//
//    if(isEq(token, _SEPARATION, "{")){
//        errorIf(param, "Using the definition type are not allowed", token);
//        errorIf(struct_symbol->m_fields, "Struct redefinetion", token);
//        struct_symbol->m_fields = ParseStructBlock();
//        //symStack->push(struct_symbol->m_fields);
////        token =scan.GetNext();
////        while(!isEq(token, _SEPARATION, "}")){
////            SymbolType *type = ParseType();
////            token = scan.GetNext();
////            while(!isEq(token, _SEPARATION, ";")){
////                VarSymbol * var = ParseIdentifier(type);
////                errorIf(dynamic_cast<FuncSymbol *>(var), "No statements and function in struct", token);
////                struct_symbol->m_fields->add_symbol(var);
////                token = scan.Get();
////                if(isEq(token, _SEPARATION, ","))
////                    token =scan.GetNext();
////            }
////            token =scan.GetNext();
////        }
//       // symStack->pop();
//    }
//    return struct_symbol;
//}
//
//
//FunctionalNode* Parser :: ParseFuncCall(ExprNode *r){
//    FunctionalNode* f = new FunctionalNode(r->token, r, dynamic_cast<FuncSymbol*>(r->getType()));
//    VarSymbol* s = dynamic_cast<VarSymbol*>(symStack->find_symbol(r->token->Value));
//    FuncSymbol* f1 = dynamic_cast<FuncSymbol* >(s->type);
//    errorIf(!f1, "not define function",scan.Get());
//    scan.Next();
//    Token* t = scan.Get();
//    int i = 0;
//    errorIf ((t->Value == ")" && f1->params->size() != 0)
//             ,"No define function with this Args",t);
//    while(!isEq(t, _SEPARATION, ")")){
//
//        ExprNode* s = ParseExpr(priorityTable[","] + 1);
//        errorIf ((s != nullptr && f1->params->size() == 0)
//                 ,"No define function with this Args",t);
//        errorIf(i > f1->params->size() &&(s->getType() != f1->params->sym_ptr[i]->getType()), "No define function with this Args",t);
//        f->addArg(s);
//        t = scan.Get();
//        errorIf(scan.isEnd(), "Expected closing bracket after function argument list",scan.Get());
//        if(isEq(t,  _SEPARATION, ",")){
//            scan.Next();
//            t = scan.Get();
//        }
//        i++;
//    }
//    scan.Next();
//    return f;
//}
//
//
//
//ExprNode* Parser :: ParseExpr(int priority){
//    if (priority > 15)
//        return ParseFactor();
//    ExprNode *left = ParseExpr(priority + 1);
//    ExprNode *root = left;
//    Token *op = scan.Get();
//
//
//    if(scan.isEnd() ||
//       isEq(op, _SEPARATION, "}") ||
//       isEq(op, _SEPARATION, ")") ||
//       isEq(op, _SEPARATION, "]") ||
//       isEq(op, _OPERATION, ":") ||
//       isEq(op, _SEPARATION, ";") ||
//       isEq(op, _SEPARATION, "{")){
//        //root->getType();
//        return root;
//    }
////    if (
////        isEq(op, _SEPARATION, ",")){
////        scan.Next();
////
////        return root;
////    }
//        errorIf(((op->Type != _OPERATION && op->Value != ","))
//                &&( op->Value !="(")
//                &&(!isEq(op, _SEPARATION,"["))
//                , "Invalid expression. Operation expected",op);
//        if(priorityTable[op->Value] < priority)
//            return root;
//
//    while(!scan.isEnd() && ((op->Type == _OPERATION && (priorityTable[op->Value] >= priority) && op->Value != "}") || op->Value =="(" || op->Value == "[")){
//        string oper = op->Value;
//
//        if(oper == "("){
//            root = ParseFuncCall(root);
//        }
//        else if(oper == "["){
//            root = ParseArrIndex(root);
//            break;
//        }
//        else if(oper == "--" ||oper == "++"){
//            root = new PostfixUnaryOpNode(op, root);
//            scan.Next();
//        }
//        else if(oper == "?"){
//            scan.Next();
//            ExprNode* l = ParseExpr();
//            if(scan.Get()->Value != ":")
//                throw MyException("Invalid ternary operator", scan.Get());
//            scan.Next();
//            ExprNode* r = ParseExpr();
//            root = new TernaryOpNode(op, root, l, r);
//        } else if(oper == "." || oper == "->")
//            root = ParseMember(root);
//        else
//        {
//            if(isEq(op, _SEPARATION, "]"))
//                break;
//            scan.Next();
//            root = new BinOpNode(op, root, ParseExpr(priority));
//         
//
//        }
//        op = scan.Get();
//    }
//
//    root->getType();
//    return root;
//}
//
//
//
//ExprNode* Parser :: ParseFactor(){
//    ExprNode *root = nullptr;
//    Token *token = scan.Get();
//    if(isEq(token, _SEPARATION, ";"))
//        return 0;
//    switch (token->Type){
//        case _INTEGER:
//            root = new IntNode(token);
//            break;
//
//        case _FLOAT: 
//            root = new FloatNode(token);
//            break;
//
//        case _IDENTIFIER:
//        {
//            Symbol *sym = symStack->find_symbol(token->Value);
//            if(!sym && parsingFunc)
//                sym = parsingFunc->params->find_symbol(token->Value);
//            string exc = "Identifier \"" + token->Value + "\" not defined";
//            //errorIf(!sym, exc.c_str(), token);
//            //errorIf(!dynamic_cast<VarSymbol*>(sym) && !dynamic_cast<FuncSymbol*>(sym), "Unknown symbol", token);
//            //errorIf(!sym, "Identifier is undefined", token);
//
//            root = new IdentifierNode(token, dynamic_cast<VarSymbol*>(sym));
//            if(isEq(scan.GetNext() ,_SEPARATION ,"("))
//                root = ParseFuncCall(root);
//            //scan.Next();
//            return root;
//        }
//
//        case _CHAR:
//            root = new CharNode(token);
//            break;
//
//        case _STRING:
//            root = new StringNode(token);
//            break;
//
//        case _KEYWORD:
//        {
//            string kw = token->Value;
//            if(kw == "char" || kw == "int" || kw == "float"){
//                scan.Next();
//                string typeName = kw ;
//                errorIf(isEq(scan.Get(), _SEPARATION, "("), "Expected open bracket '('", token);
//                root = new CastNode(token, ParseExpr(), typeName);
//                errorIf((isEq(scan.Get(), _SEPARATION, ")") && !scan.isEnd()), "Expected closing bracket", token);
//            }
//            else
//                throw MyException("You can use keywords just such as char, int and float in expressions");
//        }
//            break;
//
//
//
//
//        case _OPERATION:
//            if(unary[token->Value]){
//                scan.Next();
//                root = new UnOpNode(token, ParseExpr(priorityTable["--"]));
//            } else
//                throw MyException("Wrong expression", token);
//            break;
//
//        case _SEPARATION :
//            if(isEq(scan.Get(), _SEPARATION, "(")){
//                scan.Next();
//                root = ParseExpr();
//                if(!isEq(scan.Get(), _SEPARATION, ")"))
//                    throw MyException("Expected closing bracket");
//            }
//            else
//                throw MyException("Wrong expression", token);
//            break;
//    }
//    if (!(token->Type == _OPERATION && unary[token->Value]))
//        scan.Next();
//    root->getType();
//    return root;
//}
//
//
//ArrNode* Parser :: ParseArrIndex(ExprNode *root){
//    ArrNode *r = new ArrNode(root);
//    Token *t = scan.Get();
//    while(isEq(t, _SEPARATION, "[")){
//        scan.Next();
//        ExprNode *index = ParseExpr();
//        dynamic_cast<ArrNode*>(r)->addArg(index);
//        t = scan.Get();
//        errorIf(!isEq(t, _SEPARATION, "]") ,"Expected bracket close after array index", t);
//        scan.Next();
//        t = scan.Get();
//    }
//    return r;
//}
//
//
//ExprNode* Parser :: ParseMember(ExprNode* left){
//    SymbolType *type = left->getType();
//    StructSymbol *struct_type = 0;
//    if(dynamic_cast<PointerSymbol*>(type))
//        struct_type = dynamic_cast<StructSymbol*>(dynamic_cast<PointerSymbol*>(type)->pointer);
//    else
//        struct_type = dynamic_cast<StructSymbol*>(type);
//    errorIf(!struct_type, "Left operand of . or -> must be a structure", scan.Get());
//    Token* opTok = scan.Get();
//    Token* token =scan.GetNext();
//    errorIf(token->Type != _IDENTIFIER , "Right operand of . or -> must be a identifier", token);
//    string fieldName = token->Value;
//    if (!struct_type->m_fields->exists(fieldName))
//        fieldName = '%' + fieldName;
//    errorIf(!struct_type->m_fields->exists(fieldName), "Undefined field in structure",token);
//    scan.Next();
//    ExprNode* right = new IdentifierNode(token, dynamic_cast<VarSymbol*>(struct_type->m_fields->find_symbol(fieldName)));
//    return new BinOpNode(opTok, left, right);
//
//}
//
//VarSymbol* Parser :: ParseComplexDeclaration(SymbolType* start_type){
//    VarSymbol *var_sym = ParseDirectDeclaration();
//    if(isEq(scan.Get(), _SEPARATION, "("))
//        start_type = createFunction("", start_type);
//    else if(isEq(scan.Get(), _SEPARATION, "["))
//        start_type = ParseArrayDimension(start_type);
//    //	if(scan.Get() == cl_bracket)
//    //		scan.Next();
//    if(!var_sym->type)
//        var_sym->type = start_type;
//    else {
//        while(var_sym->type->upType())
//            var_sym->type = var_sym->type->upType();
//        var_sym->type->initType(start_type);
//    }
//    return var_sym;
//}
//
//
//
//VarSymbol* Parser :: ParseDirectDeclaration(){
//    VarSymbol *var_sym = 0;
//    SymbolType *type = 0;
//    while(isEq(scan.Get(), _OPERATION, "*") )
//        type = new PointerSymbol(type);
//    if(isEq(scan.Get(), _SEPARATION, "("))
//        var_sym = ParseDirectDeclaration();
//    else {
//        errorIf(scan.Get()->Type != _IDENTIFIER, "Identifier expected", scan.Get());
//        errorIf(symStack->find_symbol(scan.Get()->Value), "Redefinition", scan.Get());
//        var_sym = new VarSymbol(scan.Get()->Value, 0);
//        scan.Next();
//    }
//    if(isEq(scan.Get(), _SEPARATION, "("))
//        type = createFunction("", type);
//    else if(isEq(scan.Get(), _SEPARATION, "["))
//        type = ParseArrayDimension(type);
//
//    if(!var_sym->type)
//        var_sym->type = type;
//    else {
//        while(var_sym->type->upType())
//            var_sym->type = var_sym->type->upType();
//        var_sym->type->initType(type);
//    }
//    if(isEq(scan.Get(), _SEPARATION, ")"))
//        scan.Next();
//    return var_sym;
//}
//
//SymbolType* Parser :: ParseArrayDimension(SymbolType *type, bool param){
//    vector <int> index;
//    auto token = scan.Get();
//
//    while(isEq(token, _SEPARATION, "[")){
//        token =scan.GetNext();
//
//        int size = token->Type == _INTEGER ? atoi(token->Value.c_str()) : 0;
//        errorIf(size <= 0, "Array size must be int number higher than zero", token);
//        errorIf(size >= 1e5, "Array size exceeds the allowable", token);
//        index.push_back(size);
//        token =scan.GetNext();
//        errorIf(!isEq(token, _SEPARATION, "]"), "Closing bracket expected  after the array index", token);
//        token =scan.GetNext();
//    }
//
//    for(int i = index.size() - 1; i >= 0; i--)
//        type = new ArraySymbol(type, index[i]);
//    return type;
//}
//
//void Parser :: print_declaration(int deep) const{
//    symStack->print(deep);
//}
//
//void Parser :: print() const{
//
//    if(global_field.body.size())
//        cout << ' ' << "Statements:" << endl;
//    for(int i = 0; i < global_field.body.size(); i++)
//        global_field.body[i]->print(1);
//
//    if(global_field.table->sym_ptr.size()){
//        cout << ' ' << "Symbols Table:" << endl;
//        for(int i = 4; i < global_field.table->name.size(); i++){
//            cout << ' ';
//            global_field.table->sym_ptr[i]->print(1);
//        }
//    }
//
//    if(!global_field.table->sym_ptr.size() && !global_field.body.size())
//        cout << ' ' << "<< block is empty >>" << endl;
//}
//
//void Parser :: ParseParam(){
//    SymbolType *type = ParseType(true);
//    Token *token =scan.GetNext();
//    errorIf(type->name == "void" && isEq(token, _OPERATION, "*") ,"Parameter can not be of type void", token);
//    while(isEq(token, _OPERATION, "*")){
//        type = new PointerSymbol(type);
//        token =scan.GetNext();
//    }
//    if(isEq(token, _SEPARATION, "("))
//        type = dynamic_cast<SymbolType*>(ParseComplexDeclaration(type));
//    string name = token->Type == _IDENTIFIER ? token->Value : "";
//    if(name.length() > 0){
//        errorIf(symStack->top()->isExist(name), "Redefinition", token);
//        token =scan.GetNext();
//    }
//
//    if(isEq(token, _SEPARATION, "["))
//        type = ParseArrayDimension(type);
//    symStack->add_symbol(new VarSymbol(name, type));
//}
//
//void Parser :: ParseArgList(){
//    Token *token =scan.GetNext();
//    while(!isEq(token, _SEPARATION, ")")){
//        ParseParam();
//        token = scan.Get();
//        if(isEq(token, _SEPARATION, ","))
//            token =scan.GetNext();
//        else
//            errorIf(!isEq(token, _SEPARATION, ")"), "Closing bracket expected", token);
//    }
//    scan.Next();
//}
//
//FuncSymbol* Parser :: createFunction(const string &name, SymbolType *type, bool ConstFun){
//    FuncSymbol *func = new FuncSymbol(name, type);
//    func->params = new SymTable();
//    symStack->push(func->params);
//    func->isConst = ConstFun;
//    ParseArgList();
//    symStack->pop();
//    return func;
//}
//
//VarSymbol * Parser :: ParseIdentifier(SymbolType *type, bool param){
//
//    VarSymbol *result = 0;
//
//    Token *token = scan.Get();
//    bool Const = false;
//    while(isEq(token, _OPERATION, "*")){
//        type = new PointerSymbol(type);
//        token =scan.GetNext();
//    }
//    Token* name = token;
//
//    if(isEq(token, _SEPARATION, "("))
//        return ParseComplexDeclaration(type);
//
//    while(isEq(token, _OPERATION, "*")|| token->Type == _CONST){
//        type = new PointerSymbol(type);
//        if(token->Type == _CONST)
//            Const = true;
//        token =scan.GetNext();
//    }
//   // else{ token = scan.GetNext();}
//
//    if (token->Type == _IDENTIFIER){
//        name = token;
//    }
//    errorIf(name->Type != _IDENTIFIER, "Identifier expected", token);
//     token = scan.GetNext();
//    if(!isEq(token, _SEPARATION, "(")){
//        errorIf(type->name == "void", "Incomplete type is invalid", token);
//        if(isEq(token, _SEPARATION, "["))
//            type = ParseArrayDimension(type);
//    } else{
//        VarSymbol* buf = new VarSymbol(name->Value,type) ;
//
//                if((symbolBuffer = symStack->tables.back()->find_symbol(name->Value)) != 0  ){
//                    buf = dynamic_cast<VarSymbol*>(symbolBuffer);
//                    FuncSymbol* func  = dynamic_cast<FuncSymbol*>(buf->type);
//                    errorIf(!CheckArgs(func), "Redefinition",token);
//                    errorIf(type != func->value, "banned over-the return value :" + func->name ,token    );
//                    result = new VarSymbol(name->Value, func);
//                   // errorIf(!(isEq(token, _SEPARATION, ",")|| isEq(token, _SEPARATION, ";") || isEq(token, _OPERATION, "=") ||isEq(token, _SEPARATION, "{")),
//                         //   "Semicolon expected", scan.Get());
//                    return result;
//
//
//
//                }
//        else
//            type = createFunction(name->Value, type,Const);
//
//    }
//    if(Const)
//        type = new ConstSymbolType(type);
//
//
//    result = new VarSymbol(name->Value, type);
//  //  token = scan.GetNext();
//
//    errorIf(symStack->tables.back()->find_symbol(name->Value) != 0, "Redefinition", scan.Get());
//    errorIf(!(isEq(scan.Get(), _SEPARATION, ",")|| isEq(scan.Get(), _SEPARATION, ";") || isEq(scan.Get(), _OPERATION, "=") ||isEq(scan.Get(), _SEPARATION, "{")),
//            "Semicolon expected", scan.Get());
//    return result;
//}
//bool Parser::CheckArgs(FuncSymbol *func1){
//    symStack->push(new SymTable());
//    ParseArgList();
//
//     if(func1->params->size() != symStack->size())
//         return 0;
//    for( int i = 0 ; i < func1->params->name.size();i++){
//
//        if(func1->params->sym_ptr[i]->getType() != symStack->top()->sym_ptr[i]->getType())
//            return 0;
//    }
//    symStack->pop();
//
//    return 1;
//}
//
//void Parser  :: CheckReturn(FuncSymbol *func
//                            ){
//    ReturnStatement *ret = nullptr;
//    SymbolType *real_type = func->value->getType();
//    int size = func->body->body.size();
//    for(int i = 0; i < size && !ret; i++)
//        ret = dynamic_cast<ReturnStatement*>(func->body->body[i]);
//    if(!ret || !ret->value ){
//
//        errorIf((func->name != "main" && real_type->name != "void") , "Wrong return type", scan.Get());
//        return;
//    }
//    SymbolType *a = ret->value->getType();
//    errorIf(!a->canConvertTo(real_type), "Wrong return type", scan.Get());
//}
//
//Block* Parser :: ParseBlock(){
//
//    Block *block = new Block(new SymTable());
//    symStack->push(block->table);
//    blocks.push(block);
//    if(isEq(scan.Get(), _SEPARATION, "{")){
//        Token *token =scan.GetNext();
//        while(!isEq(scan.Get(), _SEPARATION, "}") && !scan.isEnd()){
//            if((token->Value == "const" || token->Value == "struct" || symStack->find_type(token->Value)||token->Value == ";"
//                )
//               &&(!dynamic_cast<FuncSymbol*>(symStack->find_type(token->Value))))
//            {
//               ParseDeclaration();
//                
//
//            }
//            else {
//                //ParseDeclaration(dynamic_cast<SymbolType*>(symStack->find_type(token->Value)));
//                block->AddStatement(ParseStatement());
//                //isCanUseBreak = false;
//                scan.Next();
//            }
//            token = scan.Get();
//        }
//    }
//    else
//    {
//        Token *token = scan.Get();
//        if(token->Value == "const" || token->Value == "struct" || symStack->find_type(token->Value))
//            ParseDeclaration();
//        else {
//            block->AddStatement(ParseStatement());
//            //scan.Next();
//            symStack->pop();
//            return block;
//        }
//    }
//    errorIf (!isEq(scan.Get(), _SEPARATION, "}"),"Unexpected brace", scan.Get());
//    blocks.pop();
//    symStack->pop();
//    return block;
//}
//
//SymTable* Parser::ParseStructBlock(){
//    SymTable* structBlock = new SymTable();
//    symStack->push(structBlock);
//    Token *token =scan.GetNext();
//    while(!isEq(scan.Get(), _SEPARATION, "}")){
//        ParseDeclaration();
//        VarSymbol* var = dynamic_cast<VarSymbol*> (symStack->top()->back());
//        errorIf(dynamic_cast<FuncSymbol*>(var->type), "No statements and function in struct", scan.Get());
//        errorIf(isEq(scan.Get(), _SEPARATION, ";")&& scan.isEnd(), "Unexpected brace",scan.Get());
//
//    }
//    symStack->pop();
//    return structBlock;
//}
//
//    
//ForStatement* Parser :: ParseFor(){
//    errorIf(symStack->size() < 2, "Can not use \"for\" at the global field", scan.Get());
//    ExprNode *first = 0, *second = 0, *third = 0;
//    errorIf((!isEq(scan.GetNext(), _SEPARATION, "(")), "Opening bracket expected", scan.Get());
//    scan.Next();
//
//    first = symStack->find_type(scan.Get()->Value) ? ParseDeclaration() :ParseExpr();
//    //errorIf(!isEq(scan.Get(), _SEPARATION, ";"), "Semicolon expected", scan.Get());
//    scan.Next();
//    second = ParseExpr();
//    errorIf(!isEq(scan.Get(), _SEPARATION, ";"), "Semicolon expected", scan.Get());
//    scan.Next();
//    if(!isEq(scan.Get(), _SEPARATION, ")"))
//        third = ParseExpr();
//    errorIf(!isEq(scan.Get(), _SEPARATION, ")"), "Closing bracket expected", scan.Get());
//    scan.Next();
//    Block *body = ParseBlock();
//    scan.Next();
//  //  isCanUseBreak = false;
//    return new ForStatement(first, second, third, body);
//}
//
//IfStatement *Parser :: ParseIf(){
//    errorIf(symStack->size() < 2, "Can not use \"if\" at the global field", scan.Get());
//    errorIf(!isEq(scan.GetNext(), _SEPARATION, "("), "Opening bracket expected", scan.Get());
//    scan.Next();
//    ExprNode *condition = ParseExpr();
//    errorIf(!isEq(scan.Get(), _SEPARATION, ")"), "Closing bracket expected", scan.Get());
//    scan.Next();
//    Block *if_branch = ParseBlock();
//    Block *else_branch = 0;
//    scan.Next();
//    if(scan.Get()->Value == "else"){
//        scan.Next();
//        else_branch = ParseBlock();
//        scan.Next();
//    }
//    return new IfStatement(condition, if_branch, else_branch);
//}
//
//WhileStatement* Parser :: ParseWhile(){
//    errorIf(symStack->size() < 2, "Can not use \"while\" at the global field", scan.Get());
//    errorIf(!isEq(scan.GetNext(), _SEPARATION, "("), "Opening bracket expected", scan.Get());
//    scan.Next();
//    ExprNode *condition = ParseExpr();
//    errorIf(!isEq(scan.Get(), _SEPARATION, ")"), "Closing bracket expected", scan.Get());
//    scan.Next();
//    Block *body = ParseBlock();
//    scan.Next();
//   //  isCanUseBreak = false;
//    return new WhileStatement(condition, body);
//
//}
//
//DoWhileStatement* Parser :: ParseDoWhile(){
//    errorIf(symStack->size() < 2, "Can not use \"do-while\" at the global field", scan.Get());
//    scan.Next();
//    Block *body = ParseBlock();
//    errorIf(scan.GetNext()->Value != "while", "While expected", scan.Get());
//    errorIf(!isEq(scan.GetNext(), _SEPARATION, "("), "Opening bracket expected", scan.Get());
//    scan.Next();
//    ExprNode *condition = ParseExpr();
//    errorIf(!isEq(scan.Get(), _SEPARATION, ")"), "Closing bracket expected", scan.Get());
//    errorIf(!isEq(scan.GetNext(), _SEPARATION, ";"), "Semicolon expected", scan.Get());
//    scan.Next();
////    isCanUseBreak = false;
//    return new DoWhileStatement(condition, body);
//}
//
//ExprNode* Parser :: ParseStatement(){
//    Token *token = scan.Get();
//    ExprNode* Statement;
//    if(token->Value == "if")
//        return ParseIf();
//    if(token->Value == "for"){
//        Statement = ParseFor();
//        return Statement;
//
//    }
//    if(token->Value == "while"){
//
//        Statement = ParseWhile();
//        return Statement;
//
//    }
//    if(token->Value == "do"){
//        Statement = ParseDoWhile();
//        return Statement;
//    }
////    if(isEq(token, _SEPARATION, "("))
////        return ParseBlock();
//    if(token->Value == "return" || token->Value == "break" || token->Value == "continue")
//        return ParseJumpStatement();
//    else
//        return ParseExpr();
//
//
//}
//
//Statement* Parser :: ParseJumpStatement(){
//    Statement *jump = 0;
//    if(scan.Get()->Value =="return"){
//        ExprNode *arg = (!isEq(scan.GetNext(),_SEPARATION,";")) ? ParseExpr() : 0;
//        jump = new ReturnStatement(arg);
//        errorIf(!parsingFunc, "Unexpected return statement", scan.Get());
//        scan.Next();
//        return jump;
//    }else if (scan.Get()->Value =="break"){
//        errorIf(!blocks.top()->isCanUseBreak, "You can't use break", scan.Get());
//
//        jump = new BreakStatement();
//        }
//    else  if(scan.Get()->Value =="continue"){
//        errorIf(!blocks.top()->isCanUseBreak, "You can't use continue",scan.Get());
//        jump = new ContinueStatement();
//        }
//
//
//    
//    errorIf(!isEq(scan.GetNext() ,_SEPARATION,";"), "Semicolon expected", scan.Get());
//    scan.Next();
//    return jump;
//}
