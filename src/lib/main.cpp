#include "codegen/codegen.h"
#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "option/option.h"
#include "transform/transform.h"
#include <fstream>

int main(int argc, char *argv[]) {
  Option option = parseCommand(argc, argv);
  if (option.argFilePath == "")
    return 0;

  std::ifstream inputFile(option.argFilePath);
  if (!inputFile.is_open()) {
    std::cerr << "Error: Could not open file " << option.argFilePath << "\n";
    return 1;
  }

  std::string input((std::istreambuf_iterator<char>(inputFile)),
                    (std::istreambuf_iterator<char>()));
  inputFile.close();
  // Lexer
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.getTokens();
  if (Lexer::raiseLexicalError(tokens))
    return 1;
  if (option.printToken)
    Lexer::print(tokens);
  // Parser
  Parser parser(tokens);
  std::unique_ptr<ModuleNode> moduleNode = parser.parse();
  if (moduleNode == nullptr) {
    std::cerr << "SyntaxError: Cannot convert the token in a valid AST\n";
    return 1;
  }
  // Lowering
  if (!transform::loweringPipeline(moduleNode, option))
    return 1;
  // Codegen
  codegen::CodeGenerator generator(moduleNode);
  generator.codegen(option.outputFile);

  return 0;
}