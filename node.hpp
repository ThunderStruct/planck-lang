//
//  node.hpp
//  Planck Interpreter
//
//  Created by Mohamed Shahawy on 4/14/18.
//  Copyright © 2018 Mohamed Shahawy. All rights reserved.
//

#ifndef node_h
#define node_h

#include "token.hpp"

struct node {
    node *left, *right;
    token tkn;
    node() {}
    node(int id, int lineNum, int colNum, std::string lexeme) {
        tkn = token(id, lineNum, colNum, lexeme);
        left = nullptr; right = nullptr;
    }
    node(token tk) : tkn(tk) { left = nullptr; right = nullptr; }
    node(token tk, node *l, node *r) : tkn(tk), left(l), right(r) {}
};


#endif /* node_h */
