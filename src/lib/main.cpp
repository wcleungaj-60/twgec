#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "codegen/codegen.h"
#include "codegen/metadata.h"
#include <fstream>

int main(int argc, char *argv[]) {
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
  std::vector<Token> tokens;
  Token token = lexer.nextToken();

  while (token.type != TokenType::END) {
    tokens.push_back(token);
    token = lexer.nextToken();
  }

  std::cout << "Tokens:\n";
  for (const auto &t : tokens) {
    std::cout << t << "\n";
  }

  Parser parser(tokens);
  std::unique_ptr<ModuleNode> moduleNode = parser.parse();
  if (moduleNode != nullptr) {
    std::cout << "\nAST:\n";
    moduleNode->print();
  } else {
    std::cout << "Invalid AST\n";
  }
  MetadataLegalizer metadataLegalizer(moduleNode);
  CodeGen codegen(moduleNode, metadataLegalizer);
  codegen.codegen("game.events");

  return 0;
}