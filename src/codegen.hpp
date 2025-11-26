#pragma once
#include "zweek.hpp"
#include <string>

namespace zweek {

class Codegen {
public:
    std::string generate(const ASTNode& root);

private:
    std::string emit_expression(ASTPtr node);
    std::string emit_function(ASTPtr node);
    std::string emit_variable(ASTPtr node);
    std::string emit_call(ASTPtr node);
    std::string emit_block(const std::vector<ASTPtr>& nodes);
    
    std::string map_type(std::string zw_type);
};

} // namespace zweek
