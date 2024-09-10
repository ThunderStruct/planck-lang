//
//  token.hpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 4/18/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

#ifndef token_h
#define token_h

#include <string>

struct token {
    // Members
    int lineNumber = 0, colNumber = 0, id = 0;
    std::string lexeme;
    
    // Constructors
    token() {}
    token(int i, const std::string &val) : id(i), lexeme(val) {}
    token(int i, int lNum, int colNum, const std::string &val) : id(i), lineNumber(lNum), colNumber(colNum), lexeme(val) {}
    
    // Types Definition
    enum terminal {
        IDENTIFIER, NUMBER, GET, PERCENT_SIGN, DOUBLE_ASTERISK, QUESTION_MARK, DOLLAR_SIGN,
        PLUS, MINUS, STAR, SLASH,
        LESS_THAN, GREATER_THAN, EQUAL, LTE, GTE, EQ, NEQ,
        LEFT_PAREN, RIGHT_PAREN,
        LEFT_BRACE, RIGHT_BRACE,
        LEFT_BRACKET, RIGHT_BRACKET,
        HASH,
        BLOCK_START,
        UNKNOWN,
        EOF_TOKEN
    };
    
    enum non_terminal {
        statement_list, statement, output, loop, conditional, method_call,
        expression_stmt, expression, simple_expression, additive_expression,
        term, factor, relop, addop, mulop
    };
    
    // Operators
    bool operator==(const token &rhs) const {
        return (this->id == rhs.id &&
                this->lexeme == rhs.lexeme &&
                this->lineNumber == rhs.lineNumber &&
                this->colNumber == rhs.colNumber);
    }
    
    // Convert token to string for debugging
    std::string stringValue() const {
        return std::to_string(id) + ' ' + lexeme + ' ' + std::to_string(lineNumber) + ' ' + std::to_string(colNumber);
    }
};

#endif /* token_h */
