#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: zweek <input_file>" << std::endl;
        return 1;
    }

    std::string filepath = argv[1];
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // 1. Lex
    zweek::Lexer lexer(source);
    auto tokens = lexer.tokenize();

    // 2. Parse
    zweek::Parser parser(tokens);
    auto ast = parser.parse();

    // 3. Codegen
    zweek::Codegen codegen;
    std::string cpp_code = codegen.generate(ast);

    bool run_mode = false;
    for (int i = 2; i < argc; ++i) {
        if (std::string(argv[i]) == "--run") {
            run_mode = true;
            break;
        }
    }

    if (run_mode) {
        // Write to temp file
        std::ofstream out("_temp_zweek.cpp");
        out << cpp_code;
        out.close();

        // Compile
        // Note: Using system() is simple but has security/portability implications.
        // For this prototype, it's fine.
        std::string compile_cmd = "cl.exe /nologo /EHsc _temp_zweek.cpp /Fe:program.exe > NUL 2>&1";
        int compile_res = system(compile_cmd.c_str());

        if (compile_res == 0) {
            // Execute
            system("program.exe");
        } else {
            std::cerr << "Compilation failed." << std::endl;
            return 1;
        }
    } else {
        std::cout << cpp_code;
    }

    return 0;
}
