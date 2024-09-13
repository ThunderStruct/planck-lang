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
#include <iostream>

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
    skipWhitespace();
    
    if (isAtEnd()) return;

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
        case '-':
            if (match('>')) {
                addToken(token::terminal::BLOCK_START, "->");
            } else {
                addToken(token::terminal::MINUS, "-");
            }
            break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(token::terminal::SLASH, "/");
            }
            break;
        case '=':
            if (match('=')) {
                addToken(token::terminal::EQ, "==");
            } else {
                addToken(token::terminal::EQUAL, "=");
            }
            break;
        case '<':
            if (match('=')) {
                addToken(token::terminal::LTE, "<=");
            } else {
                addToken(token::terminal::LESS_THAN, "<");
            }
            break;
        case '>':
            if (match('=')) {
                addToken(token::terminal::GTE, ">=");
            } else {
                addToken(token::terminal::GREATER_THAN, ">");
            }
            break;
        case '(':
            addToken(token::terminal::LEFT_PAREN, "("); break;
        case ')':
            addToken(token::terminal::RIGHT_PAREN, ")"); break;
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
                addErrorToken(std::string(1, c));
            }
            break;
    }
}

void lexer::addToken(int id, const std::string& lexeme) {
    tokens.push_back(token(id, line, col, lexeme));
    col += lexeme.length();
}

void lexer::addErrorToken(const std::string& message) {
    std::cerr << "Lexical error at line " << line << ", column " << col << ": unrecognized token '" << message << "'\n";
    tokens.push_back(token(token::terminal::UNKNOWN, line, col, message));
    col += message.length();
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

    // Check for fractional part
    if (peek() == '.' && isdigit(peekNext())) {
        advance(); // Consume '.'
        while (isdigit(peek())) advance();
    }

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
    while (!isAtEnd()) {
        char c = peek();
        if (isspace(c)) {
            if (c == '\n') {
                line++;
                col = 0;
            }
            advance();
        } else {
            break;
        }
    }
}

