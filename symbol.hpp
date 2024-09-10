//
//  symbol.hpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 5/22/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

#ifndef symbol_hpp
#define symbol_hpp

#include <string>

enum symbolType {
    UNKNOWN, METHOD, VARIABLE
};

class symbol {
friend class symbolTable;
public:
    symbol(const char* s, symbolType t, int l);             // Initialize symbol with name s
    symbol(std::string s, symbolType t);                    // Initialize symbol with string name
    symbol();                                               // Default constructor
    ~symbol();                                              // Delete name and clear pointers
    
    const char* id();                                       // Return pointer to symbol name
    symbol* nextinlist();                                   // Next symbol in list
    symbol* nextinbucket();                                 // Next symbol in bucket
    bool initialized = false;                               // Flag for whether the symbol is initialized
    float val = 0.0f;                                       // Value of the variable

private:
    char* name = nullptr;                                   // Symbol name
    symbolType type = UNKNOWN;                              // Symbol type
    int lineNum = 0;                                        // Declaration line number
    symbol* list = nullptr;                                 // Next symbol in list
    symbol* next = nullptr;                                 // Next symbol in bucket
};

#endif /* symbol_hpp */
