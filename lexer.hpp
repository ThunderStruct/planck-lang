//
//  lexer.hpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 4/16/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

// [DEPRECATED - we use Flex instead]

#ifndef lexer_hpp
#define lexer_hpp

#include <string>
#include <vector>
#include "token.hpp"


class lexer {
    
public:
    lexer(const std::string& source);
    std::vector<token> scanTokens();

private:
    std::string source;
    std::vector<token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
    int col = 0;

    bool isAtEnd();
    void scanToken();
    void addToken(int id, const std::string& lexeme);
    char advance();
    bool match(char expected);
    char peek();
    char peekNext();
    void number();
    void identifier();
    void skipWhitespace();
};

#endif /* lexer_hpp */
