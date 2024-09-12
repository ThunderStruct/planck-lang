
<img src="https://i.imgur.com/iXfjUbP.png" alt="Planck Logo" width="200">

# Planck Language



## EBNF Grammar for Planck

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

The Planck interpreter is built using Flex as the lexer and a custom C++ parser to evaluate expressions and manage symbol tables.

### Lexer

We use Flex to tokenize the input into various tokens like identifiers, numbers, operators, etc. The lexer handles both numeric and identifier-based inputs, as well as arithmetic and comparison operators.

### Parser

The parser implements the EBNF grammar for Planck, breaking down each line into statements, expressions, and operators. It is responsible for evaluating expressions and managing the program’s execution flow.

### Symbol Table

A symbol table is maintained to store variables and their associated values. The symbol table is updated during assignments and used during method calls, loops, and conditionals.



