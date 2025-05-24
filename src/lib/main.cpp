#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << "\n";
        return 1;
    }

    std::string input((std::istreambuf_iterator<char>(inputFile)),
                      (std::istreambuf_iterator<char>()));
    inputFile.close();

    Lexer lexer(input);
    Token token = lexer.nextToken();

    while (token.type != TokenType::END) {
        std::cout << token << "\n";
        token = lexer.nextToken();
    }

    return 0;
}