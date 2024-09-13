//
//  parser.cpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 4/16/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

#include "parser.hpp"
#include <iostream>

// OBJECTS' DEFINITIONS
const std::string parser::tokenStrings[26] = {                  // USED FOR ERROR-REPORTING
    "id", "num",
    "get", "output", "loop", "conditional",
    "+/-", "*/\\",
    "<=", "<", ">", ">=", "==", "!=", "=",
    "[", "]", "(", ")", "{", "}",
    "%", "**", "?", "$", "->"
};

const std::vector<token::terminal> parser::firstSet[15] {        // USED FOR LOOKUP
    { token::terminal::GET, token::terminal::IDENTIFIER },       // first(statement)
    { token::terminal::PERCENT_SIGN },                           // first(output)
    { token::terminal::DOUBLE_ASTERISK },                        // first(loop)
    { token::terminal::QUESTION_MARK },                          // first(conditional)
    { token::terminal::IDENTIFIER, token::terminal::NUMBER },    // first(expression)
    { token::terminal::IDENTIFIER },                             // first(method_call)
    { token::terminal::PLUS, token::terminal::MINUS },           // first(addop)
    { token::terminal::STAR, token::terminal::SLASH },           // first(mulop)
    { token::terminal::LESS_THAN,
      token::terminal::GREATER_THAN, token::terminal::EQUAL },   // first(relop)
};

int parser::cursor = 0;
std::vector<token> parser::programTokens;
token parser::currentToken, parser::prevToken;
bool parser::debugMode = false;
node *parser::head = nullptr;
symbolTable parser::symTable;

// SUPPORT METHODS
void parser::initTokens(const std::vector<token>& tokens, bool reset) {
    programTokens = tokens;
    if (reset) {
        cursor = 0;
        currentToken = next();
        prevToken = token();
        head = nullptr;
        symTable = symbolTable();
    }
}

void parser::parse() {
    head = program();
    
    if (debugMode) {
        std::cout << "\nDEBUG Traversal Starting:\n===================\n";
        debugTraversal(head);
        std::cout << '\n';
    }
}

void parser::parseAndExecute() {
    currentToken = next();

    while (currentToken.id != token::terminal::EOF_TOKEN) {
        node* stmt = statement();
        if (stmt != nullptr && stmt->tkn.id == token::terminal::PERCENT_SIGN) {
            std::string varName = stmt->left->tkn.lexeme;
            symbol* sym = symTable.lookup(varName.c_str());
            if (sym != nullptr && sym->initialized) {
                std::cout << sym->id() << " = " << sym->val << std::endl;
            } else {
                std::cerr << "Error: Variable " << varName << " not found or not initialized." << std::endl;
            }
        }
        currentToken = next();
    }
}

token parser::next() {
    if (cursor < programTokens.size()) {
        return programTokens[cursor++];
    } else {
        return token(token::terminal::EOF_TOKEN, "");
    }
}

void parser::match(token::terminal expected) {
    if (debugMode) {
        std::cout << "Matched: " << currentToken.lexeme << '\n';
    }
    if (currentToken.id == expected) {
        prevToken = currentToken;
        currentToken = next();
    } else {
        parseError("Expected " + tokenStrings[expected], currentToken);
    }
}

void parser::printSymbolTable() {
    std::cout << "\nSymbol Table Printing Starting:\n================================\n";
    symTable.prettyPrint();
    std::cout << '\n';
}

void parser::traverse() {
    std::cout << "\nTraversal Starting:\n===================\n";
    preorderTraversal(head);
    std::cout << '\n';
}

void parser::debugTraversal(node* p) {
    if (p) {
        std::cout << p->tkn.lexeme;
        if (p->left) {
            std::cout << ", Left ";
            debugTraversal(p->left);
        }
        if (p->right) {
            std::cout << ", Right ";
            debugTraversal(p->right);
        }
    }
}

void parser::preorderTraversal(node* p) {
    if (p) {
        std::cout << p->tkn.lexeme;
        if (p->left) {
            std::cout << ", ";
            preorderTraversal(p->left);
        }
        if (p->right) {
            std::cout << ", ";
            preorderTraversal(p->right);
        }
    }
}

bool parser::checkFirst(token::non_terminal nonTerminal) {
    for (int i = 0; i < firstSet[nonTerminal].size(); i++) {
        if (firstSet[nonTerminal][i] == currentToken.id) {
            return true;
        }
    }
    return false;
}

token parser::getPrevToken(token tkn) {
    return prevToken;
}

void parser::parseError(std::string msg, token tkn) {
    std::string errMsg = "Error at line " + std::to_string(tkn.lineNumber) + ", " + std::to_string(tkn.colNumber) + ". Expected \"" + msg + "\" but found \"" + tkn.lexeme + "\"! \n";
    throw errMsg;
}

void parser::semanticError(symbol *sym, std::string id) {
    std::string errMsg = "Semantic error! \"" + id + "\" was not declared prior to being used!\n";
    throw errMsg;
}

void parser::debugOn(bool on) {
    debugMode = on;
}

// PRODUCTION RULES
node* parser::program() {
    return statement_list();
}

node* parser::statement_list() {
    node* left = statement();
    while (checkFirst(token::non_terminal::statement)) {
        token tk = currentToken;
        node* right = statement();
        left = new node(tk, left, right);
    }
    return left;
}

node* parser::statement() {
    if (currentToken.lexeme.empty()) {
        parseError("Unexpected statement", currentToken);
    }
    node* stmt = nullptr;
    switch (currentToken.id) {
        case token::terminal::PERCENT_SIGN:
            stmt = output();
            break;
        case token::terminal::DOUBLE_ASTERISK:
            stmt = loop();
            break;
        case token::terminal::QUESTION_MARK:
            stmt = conditional();
            break;
        case token::terminal::GET:
            stmt = method_call();
            break;
        case token::terminal::IDENTIFIER:
            stmt = expression_stmt();
            break;
        case token::terminal::BLOCK_START:
            currentToken = next();
            stmt = statement_list();
            break;
        case token::terminal::EOF_TOKEN:  // Handle EOF properly
            return nullptr;
        default:
            parseError("Unexpected statement", currentToken);
            break;
    }
    return stmt;
}

node* parser::output() {
    match(token::terminal::PERCENT_SIGN);
    token varToken = currentToken;

    if (varToken.id == token::terminal::IDENTIFIER) {
        symbol* sym = symTable.lookup(varToken.lexeme.c_str());
        if (sym != nullptr && sym->initialized) {
            std::cout << sym->val << std::endl;
        } else {
            std::cerr << "Error: Variable " << varToken.lexeme << " not found or not initialized." << std::endl;
        }
        match(token::terminal::IDENTIFIER);
    } else {
        parseError("Expected identifier after '%'", varToken);
    }

    return new node(varToken);
}

node* parser::loop() {
    match(token::terminal::DOUBLE_ASTERISK);
    node* condition = expression();
    match(token::terminal::BLOCK_START);
    node* stmt = statement();
    if (currentToken.id == token::terminal::DOLLAR_SIGN) {
        match(token::terminal::DOLLAR_SIGN);
    }
    return new node(prevToken, condition, stmt);
}

node* parser::conditional() {
    node* cond = expression();
    match(token::terminal::QUESTION_MARK);
    node* stmt = statement();
    return new node(prevToken, cond, stmt);
}

node* parser::method_call() {
    match(token::terminal::GET);
    token varToken = currentToken;

    symbol* sym = symTable.lookup(varToken.lexeme.c_str());
    if (sym == nullptr) {
        semanticError(nullptr, varToken.lexeme);
    }

    if (!sym->initialized) {
        semanticError(sym, varToken.lexeme);
    }

    match(token::terminal::IDENTIFIER);
    node* method = new node(varToken);
    
    return new node(varToken, method, nullptr);
}

node* parser::expression_stmt() {
    if (currentToken.id == token::terminal::IDENTIFIER) {
        token varToken = currentToken;
        match(token::terminal::IDENTIFIER);

        if (currentToken.id == token::terminal::EQUAL) {
            match(token::terminal::EQUAL);
            node* valueNode = expression();

            symbol* sym = symTable.lookup(varToken.lexeme.c_str());
            if (sym == nullptr) {
                sym = new symbol(varToken.lexeme, VARIABLE);
                symTable.lookupInsert(sym);
            }
            sym->initialized = true;

            if (valueNode->tkn.id == token::terminal::NUMBER) {
                sym->val = std::stof(valueNode->tkn.lexeme);
            }

            return new node(varToken, nullptr, valueNode);
        } else {
            return new node(varToken);
        }
    } else {
        node* expr = expression();
        return expr;
    }
}

node* parser::expression() {
    return simple_expression();
}

node* parser::simple_expression() {
    node* left = additive_expression();
    return left;
}

node* parser::additive_expression() {
    node* left = term();
    while (currentToken.id == token::terminal::PLUS || currentToken.id == token::terminal::MINUS) {
        node* el = addop();
        node* right = term();
        left = new node(el->tkn, left, right);
    }
    return left;
}

node* parser::term() {
    node* left = factor();
    while (currentToken.id == token::terminal::STAR || currentToken.id == token::terminal::SLASH) {
        node* el = mulop();
        node* right = factor();
        left = new node(el->tkn, left, right);
    }
    return left;
}

node* parser::factor() {
    node* left = nullptr;
    switch (currentToken.id) {
        case token::terminal::LEFT_PAREN:
            match(token::terminal::LEFT_PAREN);
            left = expression();
            match(token::terminal::RIGHT_PAREN);
            break;
        case token::terminal::IDENTIFIER:
            left = new node(currentToken);
            match(token::terminal::IDENTIFIER);
            break;
        case token::terminal::NUMBER:
            left = new node(currentToken);
            match(token::terminal::NUMBER);
            break;
        default:
            parseError("factor", currentToken);
    }
    return left;
}

node* parser::relop() {
    token prev = currentToken;
    switch (currentToken.id) {
        case token::terminal::LESS_THAN:
            match(token::terminal::LESS_THAN);
            break;
        case token::terminal::GREATER_THAN:
            match(token::terminal::GREATER_THAN);
            break;
        case token::terminal::EQUAL:
            match(token::terminal::EQUAL);
            break;
        default:
            parseError("relop", currentToken);
    }
    return new node(prev);
}

node* parser::addop() {
    token prev = currentToken;
    switch (currentToken.id) {
        case token::terminal::PLUS:
            match(token::terminal::PLUS);
            break;
        case token::terminal::MINUS:
            match(token::terminal::MINUS);
            break;
        default:
            parseError("addop", currentToken);
    }
    return new node(prev);
}

node* parser::mulop() {
    token prev = currentToken;
    switch (currentToken.id) {
        case token::terminal::STAR:
            match(token::terminal::STAR);
            break;
        case token::terminal::SLASH:
            match(token::terminal::SLASH);
            break;
        default:
            parseError("mulop", currentToken);
    }
    return new node(prev);
}
