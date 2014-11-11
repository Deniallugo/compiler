////
////  Parser2.cpp
////  compiller
////
////  Created by Данил on 01.11.14.
////  Copyright (c) 2014 Данил. All rights reserved.
////
//
//#include "Parser.h"
//
//
// bool Parser::isBaseType( Token* t){
//    return (t->Type == _FLOAT || t->Type == _INTEGER || t->Type == _CHAR);
//}
//
//static bool isVar( Token t){
//
//    return (t.Type == _IDENTIFIER);
//}
//
//void Parser::errorIf_sem(bool condition, string message, Token * token) {
//    if (condition) {
//        if (token == NULL) throw MyException(message, scan.Get()->line, scan.Get()->num);
//        else throw MyException(message, token->line, token->num);
//    }
//}
//
//void Parser::errorIf(bool isTrue, string error, Token* tk){
//    if( !isTrue){
//        if (tk != NULL) {
//            throw MyException(error);
//        }
//        throw MyException(error, tk);
//    }
//}
//
//static bool isType(Token * token, bool reset, bool finish = false) {
//    static int countBaseType;
//    static int countConst;
//
//    if (finish && countConst != 0 && countBaseType == 0) return false;
//    else if (finish) return true;
//
//    if (reset) countBaseType = 0;
//
//    if (token->Type == _KEYWORD) {
//
//        if( token->Value =="integer" || token->Value =="float" || token->Value =="char"){
//            if (countBaseType++ == 0)
//                return true ;
//            else
//                return false;
//
//
//        }   else if (token->Type == _INTEGER || token->Type == _FLOAT || token->Type == _CHAR){
//            countConst++;
//            return true;
//        }
//
//        else if (isEq(token, _OPERATION, "*")) {
//            if (countBaseType != 0) return true;
//        }
//        
//        return false;
//    }
//    return false;
//    
//}
//
//bool Parser::findPriority(Token * op, int priority) {
//    switch (priority) {
//        case 0: return isEq(op, _OPERATION, "||"); break;
//        case 1: return isEq(op, _OPERATION, "&&"); break;
//        case 2: return isEq(op, _OPERATION, "|"); break;
//        case 3: return isEq(op, _OPERATION, "^"); break;
//        case 4: return isEq(op, _OPERATION, "&"); break;
//        case 5: return isEq(op, _OPERATION, "==") || isEq(op, _OPERATION, "!="); break;
//        case 6: return isEq(op, _OPERATION, ">") || isEq(op, _OPERATION, "<") || isEq(op, _OPERATION, ">=") || isEq(op, _OPERATION,"<="); break;
//        case 7: return  isEq(op, _OPERATION,"<<") ||
//            isEq(op, _OPERATION,">>"); break;
//        case 8: return isEq(op, _OPERATION, "+") || isEq(op, _OPERATION, "-");  break;
//        case 9: return isEq(op, _OPERATION, "*") || isEq(op, _OPERATION,"/") || isEq(op, _OPERATION, "%"); break;
//    }
//    return false;
//}
//
//
//void Parser::ParseProgram(){
//    scan.Next();
//    m_top = statementList();
//    m_top->print();
//    
//}
//
//
//ExprNode * Parser::statementList() {
//    auto left = declarationList();
//
//    if (left == NULL) left = statement();
//    auto right = statement();
//
//    while (right != NULL) {
//        left = new StmtListNode(left, right);
//        right = statement();
//    }
//
//    return left;
//}
//
//ExprNode * Parser::declarationList() {
//    auto left = declaration();
//
//    if (left != NULL) {
//        auto right = declarationList();
//
//        if (right != NULL) return new StmtListNode(left, right);
//    }
//    
//    return left;
//}
//
//
//
//
//
//ExprNode * Parser::statement() {
//    auto stmt = declaration();
//
//    if (stmt == NULL) stmt = expressionStmt();
//
//    return stmt;
//}
//
//ExprNode * Parser::declaration() {
//    auto typeNode = typeSpecifier();
//    if (typeNode == NULL) return NULL;
//    //m_symbolBuffer->name;
//    SymbolType * type = new SymbolType(m_symbolBuffer->name);
//
//    auto init = initDeclaratorList(type);
//
//    //errorIf(!equalTokType(m_scan->getToken(), SEPARATOR, SEMICOLON), "Expected semicolon");
//    scan.Next();
//    m_constDecl = false;
//    return new DeclStmtNode(typeNode, init);
//}
//
//ExprNode * Parser::initDeclaratorList(SymbolType * type) {
//    auto left = initDeclarator(type);
//    auto op = scan.Get();
//
//    while (isEq(op, _SEPARATION, ",")) {
//        op = scan.GetNext();
//
//        auto right = initDeclaratorList(type);
//        errorIf(right == NULL, "Expected declarator after comma");
//
//        left = new DeclListNode(left, right);
//    }
//    
//    return left;
//}
//ExprNode * Parser::initDeclarator(SymbolType * type) {
//    auto decl = pointer(type);
//    auto op = scan.Get();
//
//    if (isEq(op, _SEPARATION, "=")) {
//        errorIf(decl == NULL, "No declarator");
//
//        scan.Next();
//
//        auto init = initializer();
//        errorIf(init == NULL, "Expected initializer after the assignment operator");
//
//        return new InitDeclNode(decl, init);
//    }
//    
//    return decl;
//}
//ExprNode * Parser::typeSpecifier() {
//    auto token = scan.Get();
//
//    if (isEq(token, _KEYWORD, "const")) {
//        scan.Next();
//        m_constDecl = true;
//
//        auto decl = typeSpecifier();
//        errorIf(decl == NULL,"Expected any type");
//
//        return new ConstTypeNode(decl);
//    }
//
//    if (isEq(token, _KEYWORD, "struct")) {
//        scan.Next();
//        return structSpecifier();
//    }
//
//    // other types
//    if (token->Type == _IDENTIFIER || isBaseType(token)) {
//        string type = scan.Get()->Value;
//
//        if (!m_symStack->exist_in_top(type)) return NULL;
//
//        scan.Next();
//        
//        return new TypeNode(type, m_symbolBuffer = m_symStack->find_type(type));
//    }
//    
//    return NULL;
//}
//
//
//ExprNode * Parser::structSpecifier() {
//    auto token = scan.Get();
//    auto nameToken = token;
//
//    string name;
//
//    if (token->Type == _IDENTIFIER) {
//        name = token->Value;
//        token = scan.GetNext();
//    }
//
//    if (isEq(token, _SEPARATION, "{")) {
//        scan.Next();
//
//       // errorIf_sem(!m_semOff && m_symStack->existsType(name), "Redefinition type - " + name, nameToken);
//        auto typeStruct = new StructSymbol(name);
//
//        // add struct type to symbols table and push in stack
//        m_symStack->add_symbol(typeStruct);
//
//        m_symStack->push(typeStruct->getTable());
//
//        auto decl = structDeclarationList(typeStruct);
//     //   errorIf(decl == NULL || m_symStack->getCurTable()->countVar() == 0, "Struct without fields");
//
//        token = scan.Get();
//        errorIf(!isEq(token, _SEPARATION, "}"), "Struct without right figure bracket");
//
//        m_symStack->pop();
//
//        scan.Next();
//        return new StructNode(name, decl, m_symbolBuffer = typeStruct);
//    }
//
//   // errorIf_sem(!m_semOff && !m_symStack->existsType(name), "Indefinite type - " + name, nameToken);
//    return new StructNode(name, NULL,  m_symStack->find_type(name));
//}
//ExprNode * Parser::structDeclarationList(Symbol * typeStruct) {
//    auto left = structDeclaration(typeStruct);
//
//    if (left != NULL) {
//        auto right = structDeclarationList(typeStruct);
//
//        if (right == NULL) return left;
//
//        return new DeclListNode(left, right);
//    }
//
//    return NULL;
//}
//
//
//
//ExprNode * Parser::structDeclaration(Symbol * typeStruct) {
//    auto typeNode = typeSpecifier();
//    if (typeNode == NULL) return NULL;
//
//    Symbol * type = m_symbolBuffer;
//
//    auto decl = structDeclaratorList(type);
//
//    errorIf(!isEq(scan.Get(), _SEPARATION, ";"), "Expected semicolon");
//    scan.Next();
//    return new DeclStmtNode(typeNode, decl);
//}
//
//
//ExprNode * Parser::structDeclaratorList(Symbol * type) {
//    auto left = pointer(type);
//    auto op = scan.Get();
//
//    while (isEq(op, _SEPARATION, ",")) {
//        scan.Next();
//
//        auto right = structDeclaratorList(type);
//        errorIf(right == NULL, "Expected declarator after comma");
//
//        left = new DeclListNode(left, right);
//        op = scan.Get();
//    }
//
//    return left;
//}
//
//
//
//ExprNode * Parser::pointer(Symbol * type) {
//    auto op = scan.Get();
//
//    if (isEq(op, _KEYWORD, "const")) {
//        scan.Next();
//        m_constDecl = true;
//
//        auto decl = pointer(type);
//        errorIf(decl == NULL, "Expected declarator");
//
//        return new ConstTypeNode(decl);
//    }
//
//    if (isEq(op, _OPERATION, "*")) {
//        scan.Next();
//
//        string str = ((m_constDecl) ? "const " : "");
//        type = new PointerSymbol((SymbolType*)type);
//
//        auto decl = pointer(type);
//        errorIf(decl == NULL, "Expected declarator");
//
//        return new PointerNode(decl);
//    }
//    
//    return directDeclarator(type);
//}
//
//ExprNode * Parser::directDeclarator(Symbol * type) {
//    auto token = scan.Get();
//    auto nameToken = token;
////    m_symStack->e
//    if (token->Type == _IDENTIFIER && (m_semOff || !m_symStack->exist_in_top(token->Value))) {
//        
//        errorIf_sem(!m_semOff && m_symStack->find_symbol(token->Value), "Redefinition variable");
//
//        // Arrays
//        auto op = scan.GetNext();
//
//        if (isEq(op, _SEPARATION, "[")) {
//            ExprNode * arr = new VarNode(nameToken->Value);
//            VarNode * varNode = dynamic_cast<VarNode *>(arr);
//
//            while (isEq(op, _SEPARATION, "[")) {
//                op = scan.GetNext();
//
//                auto length = conditionalExpr();
//                errorIf(length == NULL, "Expected expression");
//
//                string str = ((m_constDecl) ? "const " : "");
//                
//                type = new ArraySymbol((SymbolType* )type, atoi(length->token->Value.c_str()), "array of (" + str + type->name + ")");
//                varNode->setType(type);
//
//                op = scan.Get();
//                errorIf(!isEq(token, _SEPARATION, "]"), "Expected right square bracket");
//                op = scan.GetNext();
//
//                m_onlyListInit = true;
//                arr = new ArrayDeclNode(arr, length);
//            }
//            //m_symStack->add_symbol();
//            m_symStack->add_symbol(new Symbol(nameToken->Value));
////            m_symStack->addVar(nameToken->getText(), type, false);
//  //          varNode->setSymVar(m_symStack->findVar(nameToken->getText()));
//
//            return arr;
//        }
//
//        // Add var to current symbols table
//        m_symStack->add_symbol(new Symbol(nameToken->Value));
//
//        return new VarNode(token->Value, (SymbolType*)m_symStack->find_symbol(nameToken->Value));
//    } else if (isEq(token, _SEPARATION, ")")) {
//        scan.Next();
//        auto decl = directDeclarator(type);
//
//        errorIf(decl == NULL, "Expected declarator");
//
//        auto op = scan.Get();
//        errorIf(!isEq(op,_SEPARATION,")"), "Expected right round bracket");
//        
//        scan.Next();
//        return decl;
//    }
//    
//    return NULL;
//}
//
//
//
//
//ExprNode * Parser::initializer() {
//    auto op = scan.Get();
//
//    // initializer list
//    if (isEq(op, _SEPARATION, "{")) {
//        scan.Next();
//
//        m_onlyListInit = false;
//
//        std::vector<ExprNode *> inits;
//        initializerList(inits);
//        auto init = new InitListNode(inits);
//
//        op = scan.Get();
//        errorIf(!isEq(op, _SEPARATION, "}"), "Expected right figure bracket");
//
//        scan.Next();
//
//        // Add value initialization in SymbolVar
//        dynamic_cast<VarSymbol *>(m_symbolBuffer)->initVal(init);
//
//        return init;
//    }
//
//    errorIf_sem(!m_semOff && m_onlyListInit, "To initialize the array required figure brackets");
//
//    auto init = assignmentExpr();
//    
//    // Add value initialization in SymbolVar
//    dynamic_cast<VarSymbol *>(m_symbolBuffer)->initVal(init);
//    
//    return init;
//}
//
//
//void Parser::initializerList(std::vector<ExprNode *> & inits) {
//    auto init = initializer();
//    errorIf(init == NULL, "Expected expression");
//
//    inits.push_back(init);
//
//    auto op = scan.Get();
//
//    while (isEq(op, _SEPARATION, ",")) {
//        scan.Next();
//
//        initializerList(inits);
//        
//        op = scan.Get();
//    }
//}
//
//
//
//ExprNode * Parser::constantExpr() {
//    m_onlyConstExpr = true;
//    auto expr = conditionalExpr();
//
//    m_onlyConstExpr = false;
//    return expr;
//}
//
//ExprNode * Parser::expressionStmt() {
//    auto expr = expression();
//
//    if (expr == NULL) return expr;
//
//    errorIf(!isEq(scan.Get(), _SEPARATION, ";"), "Expected semicolon");
//    scan.Next();
//
//    return new ExprStmtNode(expr);
//}
//
//ExprNode * Parser::expression() {
//    auto left = assignmentExpr();
//    auto op = scan.Get();
//
//    while (isEq(op, _SEPARATION, ",")) {
//        scan.Next();
//
//        auto right = expression();
//        errorIf(right == NULL, "Enumeration without expression after comma");
//
//        left = new BinOpNode(op, left, right);
//
//        op = scan.Get();
//    }
//
//    return left;
//}
//
//ExprNode * Parser::assignmentExpr() {
//    auto left = conditionalExpr();
//    auto op = scan.Get();
//
//    while (isAssing(op)) {
//        scan.Next();
//
//        errorIf(left == NULL, "Assignment expression without left operand");
//
//        auto right = assignmentExpr();
//        errorIf(right == NULL, "Assignment expression without right operand");
//
//        left = new BinOpNode(op, left, right);
//
//        op = scan.Get();
//    }
//
//    return left;
//}
//
//ExprNode * Parser::conditionalExpr() {
//    auto condition = binaryOperExpr(0);
//    auto op = scan.Get();
//
//    if (isEq(op, _OPERATION, "?")) {
//        scan.Next();
//
//        errorIf(condition == NULL, "Conditional expression without condition part");
//
//        errorIf(isEq(scan.Get(), _SEPARATION, ":"), "Ternary expression without true branch");
//
//        auto condTrue = expression();
//        op = scan.Get();
//        errorIf(condTrue == NULL, "Ternary expression without true branch");
//
//        if (isEq(op, _SEPARATION, ":")) {
//            scan.Next();
//
//            auto condFalse = conditionalExpr();
//            errorIf(condFalse == NULL, "Ternary expression without false branch");
//
//            return new TernaryOpNode(op,condition, condTrue, condFalse);
//        }
//    }
//
//    return condition;
//}
//
//ExprNode * Parser::binaryOperExpr(int priority) {
//    ExprNode * left, * right;
//
//    if (priority == 9) left = unaryExpr();
//    else left = binaryOperExpr(priority + 1);
//
//    auto op = scan.Get();
//
//    while (findPriority(op, priority)) {
//        scan.Next();
//
//        if (priority == 9) right = unaryExpr();
//        else right = binaryOperExpr(priority + 1);
//        errorIf(right == NULL, "Binary expression without right operand");
//
//        left = new BinOpNode(op, left, right);
//
//        op = scan.Get();
//    }
//
//    return left;
//}
//
//ExprNode * Parser::unaryExpr() {
//    auto token = scan.Get();
//
//    if (isEq(token, _OPERATION, "++") || isEq(token, _OPERATION, "--")) {
//        scan.Next();
//
//        auto right = unaryExpr();
//        errorIf(right == NULL, "Prefix increment operation has no operand");
//
//        return new UnOpNode(token, right);
//    }
//
//    // Unary operations + - & * ! ~
//    if (isUnary(token)) {
//        scan.Next();
//
//        auto right = unaryExpr();
//        errorIf(right == NULL, "Unary operation without operand");
//
//        return new UnOpNode(token, right);
//    }
//
//    // Operation sizeof
//    /*    if (equalTokType(token, KEYWORD, KEYWORD_SIZEOF)) {
//     m_scan->nextToken();
//
//     if (equalTokType(m_scan->getToken(), SEPARATOR, BRACKET_ROUND_LEFT)) {
//     m_scan->nextToken();
//
//     // sizeof type
//     if (isType(m_scan->getToken(), true)) {
//
//     std::vector<Token *> type;
//     type.push_back(m_scan->getToken());
//     m_scan->nextToken();
//
//     while (!equalTokType(m_scan->getToken(), SEPARATOR, BRACKET_ROUND_RIGHT)) {
//     errorIf(!isType(m_scan->getToken(), false), "Indefinite type");
//
//     type.push_back(m_scan->getToken());
//     m_scan->nextToken();
//     }
//
//     errorIf(!isType(m_scan->getToken(), false, true), "Indefinite type");
//
//     m_scan->nextToken();
//     return new UnOpNode(token, new TypecastNode(type));
//
//     } else {
//     // sizeof expression
//     auto right = expression();
//
//     errorIf(!equalTokType(m_scan->getToken(), SEPARATOR, BRACKET_ROUND_RIGHT), "Was expected closed round bracket");
//
//     m_scan->nextToken();
//     return new UnOpNode(token, right);
//     }
//
//     } else {
//     // sizeof expression
//     auto right = unaryExpr();
//     errorIf(right == NULL, "Sizeof operation without argument");
//
//     return new UnOpNode(token, right);
//     }
//     }*/
//
//    return postfixExpr();
//}
//
//ExprNode * Parser::postfixExpr() {
//    auto left = primaryExpr();
//    auto op = scan.Get();
//
//    // Accessing arrays
//    if (isEq(op, _SEPARATION, "[")) {
//        errorIf(left == NULL, "Array without left operand");
//
//        do {
//            scan.Next();
//
//            auto right = expression();
//            errorIf(right == NULL, "Expected expession");
//
//            left = new ArrNode(left, right);
//            op = scan.Get();
//
//            errorIf(!isEq(op, _SEPARATION, "]"), "Was expected closed square bracket");
//            op = scan.GetNext();;
//
//        } while (isEq(op, _SEPARATION, "["));
//
//        return left;
//    }
//
//    // Function call
//    if (isEq(op, _SEPARATION, "(")) {
//        errorIf(m_onlyConstExpr, "Expected constant expression");
//
//        std::vector<ExprNode *> args;
//
//        op = scan.GetNext();;
//        if (!isEq(op, _SEPARATION, ")")) argsListExpr(args);
//
//        op = scan.Get();
//
//        errorIf(!isEq(op, _SEPARATION, ")"), "Was expected closed round bracket");
//        scan.Next();
//
//        return new CallNode(left, args);
//    }
//
//    // Handling to the fields
//    if (isEq(op, _OPERATION, ".") || isEq(op, _OPERATION, "->")) {
//        errorIf(left == NULL, "Operation without left operand");
//
//        Token * opRec;
//        ExprNode * right;
//
//        do {
//            opRec = scan.Get();
//            scan.Next();
//
//            right = primaryExpr();
//            errorIf(right == NULL, "Expected field");
//
//            left = new RecordNode(left, opRec, right);
//            op = scan.Get();
//
//        } while (isEq(op, _OPERATION, ".") || isEq(op, _OPERATION, "->"));
//
//        return left;
//    }
//
//    if (isEq(op, _OPERATION, "++") || isEq(op, _OPERATION, "--")) {
//
//        do {
//            left = new UnOpNode(op, left);
//            op = scan.GetNext();;
//
//        } while (isEq(op, _OPERATION, "++") || isEq(op, _OPERATION, "--"));
//    }
//
//    return left;
//}
//
//void Parser::argsListExpr(std::vector<ExprNode *> & args) {
//    args.push_back(assignmentExpr());
//
//    auto op = scan.Get();
//
//    while (isEq(op, _SEPARATION, ",")) {
//
//        op = scan.GetNext();;
//
//        errorIf(isEq(op, _SEPARATION, ")"), "No argument to the function after the comma");
//
//        args.push_back(assignmentExpr());
//        op = scan.Get();
//    }
//}
//
//ExprNode * Parser::primaryExpr() {
//    auto token = scan.Get();
//
//    if (token->Type == _IDENTIFIER) {
//        errorIf(m_onlyConstExpr, "Expected constant expression");
//
//        //  errorIf(!m_semOff && !m_symStack->existsVar(token->getText()), "Variable not is defined");
//
//        scan.Next();
//
//        return new VarNode( token->Value);
//    }
//
//    if (token->Type == _INTEGER || token->Type == _FLOAT || token->Type == _CHAR || token->Type == _STRING) {
//        scan.Next();
//        return new ConstNode(*token);
//    }
//
//    if (isEq(token, _SEPARATION, "(")) {
//        scan.Next();
//
//        // Typecasting
//        /* if (isType(scan.Get(), true)) {
//         errorIf(m_onlyConstExpr, "Expected constant expression");
//
//         return castExpr();
//
//         } else {*/
//        auto res = expression();
//
//        errorIf(!isEq(scan.Get(), _SEPARATION, ")"), "Was expected closed round bracket");
//        scan.Next();
//
//        return res;
//    }
//    //}
//
//    errorIf(token->Type != _END_OF_FILE, "Unexpected token - " + token->Value);
//    return NULL;
//}
//
// ExprNode * Parser::castExpr() {
// vector<SymbolType*> s_type;
// auto op = scan.GetNext();;
//
// while (!isEq(scan.Get(), _SEPARATION, ")")) {
// errorIf(!isType(op, false), "Indefinite type");
//
// s_type.push_back (new SymbolType(op));
//
// op = scan.GetNext();
// }
//
// errorIf(!isType(op, false, true), "Indefinite type");
//
// op = scan.GetNext();;
//
// // Another typecast
// if (isEq(scan.Get(), _SEPARATION, "(")) {
// 
// op = scan.GetNext();;
// 
// errorIf(!isType(op, true), "Indefinite type");
// 
// return new CastNode(op, castExpr(), &s_type);
// }
// 
// auto right = unaryExpr();
// errorIf(right == NULL, "Cast expression without right operand");
// 
// return new CastNode(op, right, &s_type);
// }
// 
//
//
//
//
//
