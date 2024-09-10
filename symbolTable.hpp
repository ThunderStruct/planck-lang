//
//  symbolTable.hpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 5/20/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

#ifndef symbolTable_hpp
#define symbolTable_hpp

#include <stdio.h>
#include "symbol.hpp"

const unsigned HT_SIZE = 4098;                  // Hash Table Size

class symbolTable {
    
public:
    symbolTable();                              // Initialize table
    symbol* clear();                            // Clear symbol table
    symbol* lookup(const char* s);              // Lookup name s
    symbol* lookup(const char* s, unsigned h);  // Lookup s with hash h
    symbol* insert(symbol* sym, unsigned h);    // Insert s with hash h
    symbol* lookupInsert(symbol* sym);          // Lookup and insert s
    symbol* symlist() { return first; }         // List of symbols
    unsigned symbolsCount() { return count; }   // Symbol count
    void prettyPrint();                         // Prints the table's content

    
private:
    symbol* ht[HT_SIZE];                        // Hash table
    symbol* first;                              // First inserted symbol
    symbol* last;                               // Last inserted symbol
    unsigned count;                             // Symbol count
    
    unsigned hash(const char* s);               // Hashing function
};

#endif /* symbolTable_hpp */
