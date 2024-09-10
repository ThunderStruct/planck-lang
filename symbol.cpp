//
//  symbol.cpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 5/22/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

#include "symbol.hpp"
#include <cstring>

// Initialize symbol and copy id
symbol::symbol(const char* s, symbolType t, int l) {
    name = new char[std::strlen(s) + 1];
    std::strcpy(name, s);
    next = list = 0;
    type = t;
    lineNum = l;
    initialized = false;
}

// Initialize symbol with string and type
symbol::symbol(std::string s, symbolType t) {
    name = new char[s.length() + 1];
    std::strcpy(name, s.c_str());
    next = list = 0;
    type = t;
    initialized = false;
}

// Default constructor
symbol::symbol() {
    name = 0;
    next = list = 0;
    type = UNKNOWN;
    initialized = false;
}

// Delete name and clear pointers
symbol::~symbol() {
    delete[] name;
    name = 0;
    next = list = 0;
}

const char* symbol::id() { return name; }
symbol* symbol::nextinbucket() { return next; }
symbol* symbol::nextinlist() { return list; }

