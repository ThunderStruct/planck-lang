//
//  lexer.cpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 4/16/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

#include "lexer.hpp"
#include <cctype>
#include <unordered_map>

lexer::lexer(const std::string& source) : source(source) {}

std::vector<token> lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    return tokens;
}

bool lexer::isAtEnd() {
    return current >= source.length();
}

void lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '%': addToken(token::terminal::PERCENT_SIGN, "%"); break;
        case '*':
            if (match('*')) {
                addToken(token::terminal::DOUBLE_ASTERISK, "**");
            } else {
                addToken(token::terminal::STAR, "*");
            }
            break;
        case '?': addToken(token::terminal::QUESTION_MARK, "?"); break;
        case '$': addToken(token::terminal::DOLLAR_SIGN, "$"); break;
        case '+': addToken(token::terminal::PLUS, "+"); break;
        case '-': addToken(token::terminal::MINUS, "-"); break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) advance(); // Comment skip
            } else {
                addToken(token::terminal::SLASH, "/");
            }
            break;
        case '=': addToken(token::terminal::EQUAL, "="); break;
        case '<': addToken(token::terminal::LESS_THAN, "<"); break;
        case '>': addToken(token::terminal::GREATER_THAN, ">"); break;
        case '(': addToken(token::terminal::LEFT_PAREN, "("); break;
        case ')': addToken(token::terminal::RIGHT_PAREN, ")"); break;
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace
            break;
        case '\n':
            line++;
            col = 0;
            break;
        default:
            if (isdigit(c)) {
                number();
            } else if (isalpha(c)) {
                identifier();
            } else {
                addToken(token::terminal::UNKNOWN, std::string(1, c));
            }
            break;
    }
}

void lexer::addToken(int id, const std::string& lexeme) {
    tokens.push_back(token(id, line, col, lexeme));
    col += lexeme.length();
}

char lexer::advance() {
    current++;
    return source[current - 1];
}

bool lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

char lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

char lexer::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void lexer::number() {
    while (isdigit(peek())) advance();

    std::string numStr = source.substr(start, current - start);
    addToken(token::terminal::NUMBER, numStr);
}

void lexer::identifier() {
    while (isalnum(peek())) advance();

    std::string text = source.substr(start, current - start);

    // Check for keywords
    if (text == "get") {
        addToken(token::terminal::GET, text);
    } else {
        addToken(token::terminal::IDENTIFIER, text);
    }
}

void lexer::skipWhitespace() {
    while (isspace(peek())) {
        if (peek() == '\n') line++;
        advance();
    }
}
