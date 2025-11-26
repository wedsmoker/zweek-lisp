#include "parser.hpp"
#include <iostream>
#include <stdexcept>

namespace zweek {

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

ASTNode Parser::parse() {
    ASTNode root;
    root.type = NodeType::PROGRAM;
    
    while (!is_at_end()) {
        root.children.push_back(parse_expression());
    }
    return root;
}

Token Parser::peek() const {
    return tokens[pos];
}

Token Parser::advance() {
    if (!is_at_end()) pos++;
    return tokens[pos - 1];
}

bool Parser::is_at_end() const {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::consume(TokenType type, const std::string& err_msg) {
    if (peek().type == type) return advance();
    throw std::runtime_error(err_msg + " at line " + std::to_string(peek().line));
}

ASTPtr Parser::parse_expression() {
    if (peek().type == TokenType::LPAREN) {
        return parse_list();
    } else {
        return parse_atom();
    }
}

ASTPtr Parser::parse_atom() {
    Token t = advance();
    auto node = std::make_shared<ASTNode>();
    node->value = t.value;
    
    if (t.type == TokenType::NUMBER) node->type = NodeType::LITERAL;
    else if (t.type == TokenType::STRING) node->type = NodeType::LITERAL;
    else if (t.type == TokenType::SYMBOL) node->type = NodeType::SYMBOL;
    else throw std::runtime_error("Unexpected token: " + t.value);
    
    return node;
}

ASTPtr Parser::parse_list() {
    consume(TokenType::LPAREN, "Expect '('");
    
    auto node = std::make_shared<ASTNode>();
    node->type = NodeType::CALL; // Default to generic list/call
    
    // Check for empty list ()
    if (peek().type == TokenType::RPAREN) {
        advance();
        return node; 
    }

    // Peek at the first element to see if it's a keyword
    if (peek().type == TokenType::SYMBOL) {
        std::string head = peek().value;
        if (head == "def") node->type = NodeType::FUNCTION_DEF;
        else if (head == "let") node->type = NodeType::VARIABLE_DECL;
    }

    while (peek().type != TokenType::RPAREN && !is_at_end()) {
        node->children.push_back(parse_expression());
    }
    
    consume(TokenType::RPAREN, "Expect ')'");
    return node;
}

} // namespace zweek
