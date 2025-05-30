#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"
#include <memory>
#include <queue>
#include <stdexcept>
#include <vector>

class Parser {
private:
  std::queue<Token> tokens;
  bool consume(TokenType type);
  std::unique_ptr<BlockNode> parseBlock();
  std::unique_ptr<MetadataNode> parseMetadata();
  std::unique_ptr<ActionsNode> parseActions();
  std::unique_ptr<ChecksNode> parseChecks();
  std::unique_ptr<TriggersNode> parseTriggers();

public:
  Parser(const std::vector<Token> &inputTokens);
  std::unique_ptr<ModuleNode> parse();
};

#endif