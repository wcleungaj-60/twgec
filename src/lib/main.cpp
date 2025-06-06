#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "codegen/codegen.h"
#include "codegen/metadata.h"
#include <fstream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file> <argument>*\n";
    return 1;
  }

  std::ifstream inputFile(argv[1]);
  if (!inputFile.is_open()) {
    std::cerr << "Error: Could not open file " << argv[1] << "\n";
    return 1;
  }

  bool printToken = false;
  bool printAST = false;

  for(int i = 2; i < argc; i++) {
    if(std::string(argv[i]) == "--print-token")
      printToken = true;
    if(std::string(argv[i]) == "--print-ast")
      printAST = true;
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

  if(printToken){
    std::cout << "===== Print Tokens =====\n";
    for (const auto &t : tokens)
      std::cout << t << "\n";
  }

  Parser parser(tokens);
  std::unique_ptr<ModuleNode> moduleNode = parser.parse();
  if (moduleNode == nullptr) {
    std::cout << "Invalid AST\n";
    return 1;
  }
  if(printAST){
    std::cout << "===== Print AST =====\n";
    moduleNode->print();
  }

  MetadataLegalizer metadataLegalizer(moduleNode);
  CodeGen codegen(moduleNode, metadataLegalizer);
  codegen.codegen("game.events");

  return 0;
}