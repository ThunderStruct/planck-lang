//
//  main.cpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 4/16/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

//  CSCE4101 - Compiler Design
//  Spring 2018, Dr. Rafea
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "parser.hpp"
#include "lexer.hpp"

// Prototypes
void runInterpreter();

// Entry point
int main(int argc, const char * argv[]) {
    // Prompt user for file or interactive interpretation
    std::string choice;
    
    do {
        std::cout << "Would you like to (1) input a file path or (2) enter interactive mode? (1 or 2): ";
        std::getline(std::cin, choice);
    } while (choice != "1" && choice != "2");

    if (choice == "2") {
        runInterpreter();
        return 0;
    }

    // If user chose file path
    std::string ifPath;
    std::ifstream testFile;

    // Prompt user input
    std::cout << "Please enter the relative Planck file path: ";
    std::getline(std::cin, ifPath);
    
    // Validate input file path
    testFile.open(ifPath);
    if (!testFile.good()) {
        std::cerr << "\nInvalid file path! Terminating...\n";
        testFile.close();
        return 1;
    }
    testFile.close();

    // Read the file content
    std::ifstream inputFile(ifPath);
    std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Tokenize the file content
    lexer lex(fileContent);
    std::vector<token> programTokens = lex.scanTokens();

    // Parse and execute the file content
    try {
        parser::initTokens(programTokens, true);
        parser::parseAndExecute();
    } catch (const std::string& msg) {
        std::cerr << msg << std::endl;
        return 2;
    }

    parser::printSymbolTable();
    return 0;
}

// Function definitions
void runInterpreter() {
    std::string line;

    std::cout << "Interactive Planck Interpreter. Type your code below:\n";
    std::cout << "(Type 'exit' to quit)\n";
    
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line == "exit") break;

        // Tokenize the input line
        lexer lex(line);
        std::vector<token> programTokens = lex.scanTokens();

        // Parse and execute the input line
        try {
            parser::initTokens(programTokens, false);
            parser::parseAndExecute();
        } catch (const std::string& msg) {
            std::cerr << msg << std::endl;
        }

        // parser::printSymbolTable();
    }
}
