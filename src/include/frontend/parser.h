#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"
#include <memory>
#include <queue>
#include <vector>

class Parser {
private:
  std::queue<Token> tokens;
  bool consume(TokenType type, bool errorThrowing);
  std::unique_ptr<BlockNode> parseBlock();
  std::unique_ptr<MetadataNode> parseMetadata();
  std::unique_ptr<ConstDefNode> parseConstDef();
  std::unique_ptr<FunDefNode> parseFunDef();
  std::unique_ptr<TypedInstrSetNode> parseTypedInstrSet();
  std::unique_ptr<InstrSetNode> parseInstrSet();
  std::unique_ptr<CompositeInstrNode> parseCompositeInstr();
  std::unique_ptr<BranchNode> parseBranch();
  std::unique_ptr<InstructionNode> parseInstruction();
  bool parseInstructionArgs(std::unique_ptr<InstructionNode> &,
                            bool foundNamed = false);
  std::unique_ptr<ValueNode> parseValue();
  std::unique_ptr<OperationNode> parseOperation();
  std::unique_ptr<ExpressionNode> parseExp();

public:
  Parser(const std::vector<Token> &inputTokens);
  std::unique_ptr<ModuleNode> parse();
};

#endif