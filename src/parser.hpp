#pragma once
#include "zweek.hpp"
#include <vector>

namespace zweek {

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    ASTNode parse();

private:
    const std::vector<Token>& tokens;
    size_t pos = 0;

    Token peek() const;
    Token advance();
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& err_msg);
    bool is_at_end() const;

    ASTPtr parse_expression();
    ASTPtr parse_list();
    ASTPtr parse_atom();
};

} // namespace zweek
