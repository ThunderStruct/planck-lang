//
//  symbolTable.cpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 5/20/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

#include "symbolTable.hpp"
#include <iostream>
#include <cstring>

// Initialize a symbol table
symbolTable::symbolTable() {
    for (int i = 0; i < HT_SIZE; i++) ht[i] = 0;
    first = last = 0;
    count = 0;
}

// Clear a symbol table and return its symbol list
symbol* symbolTable::clear() {
    symbol* list = first;
    for (int i = 0; i < HT_SIZE; i++) ht[i] = 0;
    first = last = 0;
    count = 0;
    return list;
}

// Lookup name s
symbol* symbolTable::lookup(const char* s) {
    unsigned h = hash(s);
    return lookup(s, h);
}

// Lookup name s with hash value h
symbol* symbolTable::lookup(const char* s, unsigned h) {
    unsigned index = h % HT_SIZE;
    symbol* sym = ht[index];
    while (sym != 0) {
        if (std::strcmp(sym->id(), s) == 0) break;
        sym = sym->nextinbucket();
    }
    return sym;
}

// Insert symbol sym with hash h
symbol* symbolTable::insert(symbol* sym, unsigned h) {
    unsigned index = h % HT_SIZE;
    sym->next = ht[index];
    ht[index] = sym;
    if (count == 0) {
        first = last = sym;
    } else {
        last->list = sym;
        last = sym;
    }
    count++;
    return sym;
}

// Lookup and insert symbol sym
symbol* symbolTable::lookupInsert(symbol* sym) {
    unsigned h = hash(sym->id()); // Computed once
    symbol* temp = lookup(sym->id(), h); // Locate symbol first
    if (temp == 0) { // If not found
        sym = insert(sym, h); // Insert a new symbol
    } else {
        sym = temp;
    }
    return sym;
}

// Print the symbol table's content
void symbolTable::prettyPrint() {
    symbol *curr = first;
    std::cout << "TYPE\tNAME\tLINE_NUMBER\n";
    while (curr != nullptr) {
        std::cout << '\n' << curr->type << '\t' << curr->id() << '\t' << curr->lineNum;
        curr = curr->nextinlist();
    }
}

// Hashing function for the symbol name
unsigned symbolTable::hash(const char* s) {
    unsigned hval = 0;
    while (s[0] != 0 && s[1] != 0 && s[2] != 0 && s[3] != 0) {
        unsigned u = *((unsigned*) s);
        hval += u;
        s += 4;
    }
    if (s[0] == 0) return hval;
    hval += s[0];
    if (s[1] == 0) return hval;
    hval += s[1] << 8;
    if (s[2] == 0) return hval;
    hval += s[2] << 16;
    return hval;
}

