#pragma once
#include <string>
#include <vector>
#include <variant>
#include <memory>

namespace zweek {

// --- Lexer Types ---
enum class TokenType {
    LPAREN,     // (
    RPAREN,     // )
    SYMBOL,     // name, +, if, def
    NUMBER,     // 123, 4.5
    STRING,     // "hello"
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;
    int line;
};

// --- AST Types ---
enum class NodeType {
    PROGRAM,
    FUNCTION_DEF,
    VARIABLE_DECL, // let
    CALL,          // (func arg1 arg2)
    LITERAL,       // 123, "abc"
    SYMBOL         // x, y
};

struct ASTNode;
using ASTPtr = std::shared_ptr<ASTNode>;

struct ASTNode {
    NodeType type;
    std::string value; // For symbols/literals/names
    std::vector<ASTPtr> children;
    
    // Metadata for specific node types could be added here
    // e.g., return type for functions
    std::string data_type; 
};

} // namespace zweek
