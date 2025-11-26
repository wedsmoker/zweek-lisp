#include "lexer.hpp"
#include <cctype>
#include <iostream>

namespace zweek {

Lexer::Lexer(const std::string& source) : src(source) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!is_eof()) {
        skip_whitespace();
        if (is_eof()) break;

        char c = peek();
        if (c == '(') {
            tokens.push_back({TokenType::LPAREN, "(", line});
            advance();
        } else if (c == ')') {
            tokens.push_back({TokenType::RPAREN, ")", line});
            advance();
        } else if (c == '"') {
            tokens.push_back(read_string());
        } else if (isdigit(c) || (c == '-' && isdigit(src[pos + 1]))) {
            tokens.push_back(read_number());
        } else {
            tokens.push_back(read_symbol());
        }
    }
    tokens.push_back({TokenType::END_OF_FILE, "", line});
    return tokens;
}

char Lexer::peek() const {
    if (is_eof()) return '\0';
    return src[pos];
}

char Lexer::advance() {
    if (is_eof()) return '\0';
    return src[pos++];
}

bool Lexer::is_eof() const {
    return pos >= src.length();
}

void Lexer::skip_whitespace() {
    while (!is_eof()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        } else if (c == '\n') {
            line++;
            advance();
        } else if (c == ';') {
            // Comment: skip until newline
            while (!is_eof() && peek() != '\n') advance();
        } else {
            break;
        }
    }
}

Token Lexer::read_string() {
    std::string val = "\"";
    advance(); // skip opening quote
    while (!is_eof() && peek() != '"') {
        char c = advance();
        if (c != '\r') val += c;
    }
    if (!is_eof()) {
        advance(); // skip closing quote
        val += "\"";
    }
    return {TokenType::STRING, val, line};
}

Token Lexer::read_number() {
    std::string val;
    if (peek() == '-') val += advance();
    
    while (!is_eof() && isdigit(peek())) {
        val += advance();
    }
    if (!is_eof() && peek() == '.') {
        val += advance();
        while (!is_eof() && isdigit(peek())) {
            val += advance();
        }
    }
    return {TokenType::NUMBER, val, line};
}

Token Lexer::read_symbol() {
    std::string val;
    while (!is_eof()) {
        char c = peek();
        if (isalnum(c) || c == '_' || c == '+' || c == '-' || c == '*' || c == '/' || c == ':' || c == '!' || c == '=' || c == '<' || c == '>') {
            val += advance();
        } else {
            break;
        }
    }
    return {TokenType::SYMBOL, val, line};
}

} // namespace zweek
