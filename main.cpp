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
#include "planck.lex.hpp"

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
    
    // Set up Flex to read from the input file
    FILE* inputFile = fopen(ifPath.c_str(), "r");
    if (!inputFile) {
        std::cerr << "\nError opening file! Terminating...\n";
        return 1;
    }
    yyin = inputFile;

    // Parse the file content
    try {
        parser::initTokens(true);
        parser::parseAndExecute();
    } catch (const std::string& msg) {
        std::cerr << msg << std::endl;
        fclose(inputFile);
        return 2;
    }

    fclose(inputFile);
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

        YY_BUFFER_STATE bufferState = yy_scan_string(line.c_str());
        
        // Parse and execute the line
        try {
            parser::initTokens(false);
            parser::parseAndExecute();
        } catch (const std::string& msg) {
            std::cerr << msg << std::endl;
        }

        yy_delete_buffer(bufferState);
        
        parser::printSymbolTable();
    }
}
