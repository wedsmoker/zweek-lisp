#pragma once
#include "zweek.hpp"
#include <vector>
#include <string>

namespace zweek {

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string src;
    size_t pos = 0;
    int line = 1;

    char peek() const;
    char advance();
    bool is_eof() const;
    
    void skip_whitespace();
    Token read_symbol();
    Token read_number();
    Token read_string();
};

} // namespace zweek
