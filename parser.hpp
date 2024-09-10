//
//  parser.hpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 4/16/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

#ifndef parser_hpp
#define parser_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "node.hpp"
#include "token.hpp"
#include "symbolTable.hpp"

class parser {
    
private:
    // PRODUCTION RULES PROTOTYPES
    static node* program();
    static node* statement_list();
    static node* statement();
    static node* output();
    static node* loop();
    static node* conditional();
    static node* method_call();
    static node* expression_stmt();
    static node* expression();
    static node* simple_expression();
    static node* additive_expression();
    static node* term();
    static node* factor();
    static node* relop();
    static node* addop();
    static node* mulop();
    
    // CONSTANTS
    static const std::string tokenStrings[26];
    static const std::vector<token::terminal> firstSet[15];
    
    // MEMBERS
    static int cursor;
    static token currentToken, prevToken;
    static bool debugMode;
    static node *head;
    static std::vector<token> programTokens;
    static symbolTable symTable;
    
    // METHODS
    static void match(token::terminal);
    static bool checkFirst(token::non_terminal);
    static void parseError(std::string, token);
    static void semanticError(symbol* sym, std::string id);
    static token getPrevToken(token);
    static void debugTraversal(node *);
    static void preorderTraversal(node *);
    static token next();
    
public:
    static void initTokens(bool reset);
    static void parse();
    static void parseAndExecute();
    static void traverse();
    static void printSymbolTable();
    static void debugOn(bool);
};

#endif /* parser_hpp */
