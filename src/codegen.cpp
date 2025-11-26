#include "codegen.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>

namespace zweek {

std::string Codegen::generate(const ASTNode& root) {
    std::stringstream ss;
    ss << "#include <iostream>\n";
    ss << "#include <vector>\n";
    ss << "#include <string>\n";
    ss << "#include <memory>\n\n";
    
    for (const auto& child : root.children) {
        ss << emit_expression(child) << "\n";
    }
    return ss.str();
}

std::string Codegen::map_type(std::string zw_type) {
    if (zw_type == ":int") return "int";
    if (zw_type == ":float") return "double";
    if (zw_type == ":str") return "std::string";
    if (zw_type == ":bool") return "bool";
    if (zw_type == ":void") return "void";
    return zw_type; // Fallback
}

std::string Codegen::emit_expression(ASTPtr node) {
    if (node->type == NodeType::LITERAL) return node->value;
    if (node->type == NodeType::SYMBOL) return node->value;
    if (node->type == NodeType::FUNCTION_DEF) return emit_function(node);
    if (node->type == NodeType::VARIABLE_DECL) return emit_variable(node);
    if (node->type == NodeType::CALL) return emit_call(node);
    return "";
}

std::string Codegen::emit_function(ASTPtr node) {
    // Structure: (def name ((arg :type)...) :ret body...)
    // Children: [def, name, args_list, ret_type, body...]
    
    if (node->children.size() < 4) return "// Error: Invalid function def";
    
    std::string name = node->children[1]->value;
    ASTPtr args_list = node->children[2];
    std::string ret_type = map_type(node->children[3]->value);
    
    std::stringstream ss;
    ss << ret_type << " " << name << "(";
    
    // Process Args
    for (size_t i = 0; i < args_list->children.size(); ++i) {
        // Each arg is (name :type)
        ASTPtr arg_pair = args_list->children[i];
        if (arg_pair->children.size() == 2) {
            std::string arg_name = arg_pair->children[0]->value;
            std::string arg_type = map_type(arg_pair->children[1]->value);
            ss << arg_type << " " << arg_name;
            if (i < args_list->children.size() - 1) ss << ", ";
        }
    }
    
    ss << ") {\n";
    
    // Body
    for (size_t i = 4; i < node->children.size(); ++i) {
        std::string stmt = emit_expression(node->children[i]);
        
        // Implicit return logic
        bool is_control = (stmt.rfind("if", 0) == 0) || (stmt.rfind("while", 0) == 0) || (stmt.rfind("return", 0) == 0);
        if (i == node->children.size() - 1 && ret_type != "void" && !is_control) {
            stmt = "return " + stmt;
        }

        // Add semicolon if it looks like a statement and doesn't have one
        if (!stmt.empty() && stmt.back() != '}' && stmt.back() != ';') {
             stmt += ";";
        }
        ss << "    " << stmt << "\n";
    }
    ss << "}\n";
    return ss.str();
}

std::string Codegen::emit_variable(ASTPtr node) {
    // (let name val)
    if (node->children.size() < 3) return "// Error: Invalid let";
    std::string name = node->children[1]->value;
    std::string val = emit_expression(node->children[2]);
    return "auto " + name + " = " + val;
}

std::string Codegen::emit_call(ASTPtr node) {
    if (node->children.empty()) return "()";
    
    std::string func_name = node->children[0]->value;
    
    // Infix Operators
    if (func_name == "+" || func_name == "-" || func_name == "*" || func_name == "/" || 
        func_name == ">" || func_name == "<" || func_name == "==") {
        if (node->children.size() == 3) {
            return "(" + emit_expression(node->children[1]) + " " + func_name + " " + emit_expression(node->children[2]) + ")";
        }
    }
    
    // Print
    if (func_name == "print") {
        std::stringstream ss;
        ss << "std::cout";
        for (size_t i = 1; i < node->children.size(); ++i) {
            ss << " << " << emit_expression(node->children[i]);
        }
        ss << " << std::endl";
        return ss.str();
    }
    
    // Return
    if (func_name == "return") {
        if (node->children.size() > 1)
            return "return " + emit_expression(node->children[1]);
        else 
            return "return";
    }

    // If
    if (func_name == "if") {
        if (node->children.size() < 3) return "// Error: Invalid if";
        std::string cond = emit_expression(node->children[1]);
        std::string then_branch = emit_expression(node->children[2]);
        std::string else_branch = (node->children.size() > 3) ? emit_expression(node->children[3]) : "";
        
        std::stringstream ss;
        ss << "if (" << cond << ") {\n        " << then_branch << ";\n    }";
        if (!else_branch.empty()) {
            ss << " else {\n        " << else_branch << ";\n    }";
        }
        return ss.str();
    }

    // Standard Call
    std::stringstream ss;
    ss << func_name << "(";
    for (size_t i = 1; i < node->children.size(); ++i) {
        ss << emit_expression(node->children[i]);
        if (i < node->children.size() - 1) ss << ", ";
    }
    ss << ")";
    return ss.str();
}

} // namespace zweek
