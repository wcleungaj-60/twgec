#include "codegen/codegen.h"
#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "tools/help.h"
#include "transform/transform.h"
#include <fstream>

bool loweringPipeline(const std::unique_ptr<ModuleNode> &moduleNode,
                      bool printAST = false) {
  if (printAST)
    moduleNode->print("AST Before Symbol Checking");
  if (!transform::symbolChecking(std::move(moduleNode)))
    return false;
  if (printAST)
    moduleNode->print("AST Before Arg Binding");
  if (!transform::argBinding(std::move(moduleNode)))
    return false;
  if (printAST)
    moduleNode->print("AST Before Block Inling");
  if (!transform::blockInling(std::move(moduleNode)))
    return false;
  if (printAST)
    moduleNode->print("AST Before Block Legalization");
  if (!transform::blockLegalization(std::move(moduleNode)))
    return false;
  if (printAST)
    moduleNode->print("AST Before Function Inlining");
  if (!transform::functionInling(std::move(moduleNode)))
    return false;
  if (printAST)
    moduleNode->print("AST Before Constant Folding");
  if (!transform::constantFolding(std::move(moduleNode)))
    return false;
  if (printAST)
    moduleNode->print("AST Before If Statement Propagation");
  if (!transform::ifStatementPropagation(std::move(moduleNode)))
    return false;
  if (printAST)
    moduleNode->print("AST Before Code Generation");
  return true;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Please run `twgec --help` for help.\n";
    return 1;
  }

  bool optPrintToken = false;
  bool optPrintAST = false;
  bool optHelp = false;
  std::string argFilePath = "";

  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "--print-token") {
      optPrintToken = true;
    } else if (std::string(argv[i]) == "--print-ast") {
      optPrintAST = true;
    } else if (std::string(argv[i]) == "--help") {
      optHelp = true;
    } else if (argFilePath == "") {
      argFilePath = argv[i];
    } else {
      std::cerr << "Unknown arugment \"" << argv[i]
                << "\" found.\nPlease run `twgec --help` for help.\n";
      return 1;
    }
  }

  if (optHelp) {
    std::cout << helpMessage;
    return 0;
  }

  std::ifstream inputFile(argFilePath);
  if (!inputFile.is_open()) {
    std::cerr << "Error: Could not open file " << argFilePath << "\n";
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
  if (optPrintToken)
    Lexer::print(tokens);
  // Parser
  Parser parser(tokens);
  std::unique_ptr<ModuleNode> moduleNode = parser.parse();
  if (moduleNode == nullptr) {
    std::cerr << "SyntaxError: Cannot convert the token in a valid AST\n";
    return 1;
  }
  // Lowering
  if (!loweringPipeline(moduleNode, optPrintAST))
    return 1;
  // Codegen
  codegen::CodeGenerator generator(moduleNode);
  generator.codegen("game.events");

  return 0;
}