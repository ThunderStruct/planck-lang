
<img src="https://i.imgur.com/iXfjUbP.png" alt="Planck Logo" width="250">

# Planck Language

**Planck** is an esoteric, minimal language created as a hobby project as part of my _Compiler Design_ course during my undergrad. Recently branded and open-sourced just for fun!


## The Extended Backus-Naur Form Grammar for Planck

```ebnf
<program> ::= <statement_list>

<statement_list> ::= <statement> { <statement> }

<statement> ::= <output> 
              | <loop> 
              | <conditional> 
              | <method_call> 
              | <assignment>

<output> ::= "%" <identifier>

<loop> ::= "**" <expression> "->" <statement> "$"

<conditional> ::= <expression> "?" <statement>

<method_call> ::= "get" <identifier> "(" <identifier> ")"

<assignment> ::= <identifier> "=" <expression>

<expression> ::= <additive_expression> { <relop> <additive_expression> }

<additive_expression> ::= <term> { <addop> <term> }

<term> ::= <factor> { <mulop> <factor> }

<factor> ::= "(" <expression> ")" 
           | <identifier> 
           | <number>

<relop> ::= "<=" | "<" | ">" | ">=" | "==" | "!="

<addop> ::= "+" | "-"

<mulop> ::= "*" | "/"

<identifier> ::= [a-zA-Z_][a-zA-Z0-9_]*

<number> ::= [0-9]+ ("." [0-9]+)?
```

## Tokens

The tokens recognized by the Planck language are:

- **Identifiers**: Any variable name starting with a letter or underscore, followed by alphanumeric characters, underscores, periods, or special characters like `\#`, `$`, and `_`.
- **Numbers**: Numeric literals, including integers and floating-point numbers, written as `123` or `12.34`.
- **Operators**: Arithmetic and relational operators like `+`, `-`, `*`, `/`, `<`, `<=`, `>`, `>=`, `==`, and `!=`.
- **Assignment**: The assignment operator `=`.
- **Method Call**: The keyword `get`, used to call a method with syntax like `get x(y)`.
- **Output**: The `%` symbol, used to output the value of a variable.
- **Loops**: Denoted with the `**` symbol followed by an expression, then `->`, a statement, and ending with `$`.
- **Conditionals**: Denoted with a `?` after an expression and followed by a statement.
- **Delimiters**: Characters like parentheses `()`, braces `{}`, and brackets `[]`.

## Examples

1. **Assignment and Output**:

```
x = 10
% x
```

**Output**:

```
10
```


2. **Loop**:

```
** 5 -> % x $
```

**Explanation**:
This defines a loop that runs 5 times and outputs the value of `x`.

3. **Conditional**:

```
x = 5 ? % x
```

**Output**:

```
5
```

4. **Method Call**:

```
get x(y)
```

**Explanation**:
Calls the method `x` with the argument `y`.

## Planck Interpreter

The Planck interpreter is built purely using C++, and is split into 3 fundamental components: a custom lexer (Flex configuration is also implemented), a parser to evaluate expressions, and a symbol table to manage and track identifiers.

### Lexer

We use Flex to tokenize the input into various tokens like identifiers, numbers, operators, etc. The lexer handles both numeric and identifier-based inputs, as well as arithmetic and comparison operators.

### Parser

The parser uses _recursive descent_ techniques to break down the tokenized input according to Planck's EBNF grammar. It evaluates statements like assignments, loops, conditionals, and expressions, constructing an abstract syntax tree (AST) and managing control flow. It checks for syntax and semantic errors, ensuring valid execution.

### Symbol Table

The symbol table in Planck is implemented using a hash table. After implementing both the symbol and symbolTable classes, it was important to integrate the new data structure with the parser.

The symbol table insertion starts in `parser::expression_stmt`, where assignments are matched. When a variable is assigned, its name, line number, and value are stored as a symbol object and inserted into the table.

To check for variable declarations before usage, a simple `lookup()` call in the expression production rule ensures that variables exist before they are used. If not found, an error is raised.
