#include "frontend/parser.h"
#include <queue>
#include <stdexcept>
#include <vector>

bool Parser::consume(TokenType type) {
  if (!tokens.empty() && tokens.front().type == type) {
    tokens.pop();
    return true;
  }
  return false;
}

Parser::Parser(const std::vector<Token> &inputTokens) {
  for (const auto &token : inputTokens) {
    tokens.push(token);
  }
}

std::unique_ptr<ModuleNode> Parser::parse() {
  std::unique_ptr<ModuleNode> moduleNode = std::make_unique<ModuleNode>();
  while (!tokens.empty()) {
    auto blockNode = parseBlock();
    if (!blockNode)
      return nullptr;
    moduleNode->nodes.push_back(std::move(blockNode));
  }
  return moduleNode;
}

/**
 * BLOCK + IDENTIFIFER + OPENCUR + [ACTIONS|CHECKS|TRIGGERS] + CLOSECUR
 */
std::unique_ptr<BlockNode> Parser::parseBlock() {
  if (!consume(TokenType::BLOCK) || tokens.empty())
    return nullptr;
  std::string name = tokens.front().value;
  if (!consume(TokenType::IDENTIFIER) || !consume(TokenType::OPENCUR))
    return nullptr;
  std::unique_ptr<BlockNode> blockNode = std::make_unique<BlockNode>(name);
  while (tokens.front().type != TokenType::CLOSECUR) {
    if (tokens.front().type == TokenType::ACTIONS) {
      auto actionsNode = parseActions();
      if (!actionsNode)
        return nullptr;
      blockNode->actionsNode = std::move(actionsNode);
    }
    if (tokens.front().type == TokenType::CHECKS) {
      auto checksNode = parseChecks();
      if (!checksNode)
        return nullptr;
      blockNode->checksNode = std::move(checksNode);
    }
    if (tokens.front().type == TokenType::TRIGGERS) {
      auto triggersNode = parseTriggers();
      if (!triggersNode)
        return nullptr;
      blockNode->triggersNode = std::move(triggersNode);
    }
  }
  if (!consume(TokenType::CLOSECUR))
    return nullptr;
  return blockNode;
}

/**
 * ACTIONS + OPENCUR + CLOSECUR
 */
std::unique_ptr<ActionsNode> Parser::parseActions() {
  if (!consume(TokenType::ACTIONS) || !consume(TokenType::OPENCUR) ||
      !consume(TokenType::CLOSECUR))
    return nullptr;
  std::unique_ptr<ActionsNode> actionsNode = std::make_unique<ActionsNode>();
  return actionsNode;
}

/**
 * CHECKS + OPENCUR + CLOSECUR
 */
std::unique_ptr<ChecksNode> Parser::parseChecks() {
  if (!consume(TokenType::CHECKS) || !consume(TokenType::OPENCUR) ||
      !consume(TokenType::CLOSECUR))
    return nullptr;
  std::unique_ptr<ChecksNode> checksNode = std::make_unique<ChecksNode>();
  return checksNode;
}

/**
 * TRIGGERS + OPENCUR + CLOSECUR
 */
std::unique_ptr<TriggersNode> Parser::parseTriggers() {
  if (!consume(TokenType::TRIGGERS) || !consume(TokenType::OPENCUR) ||
      !consume(TokenType::CLOSECUR))
    return nullptr;
  std::unique_ptr<TriggersNode> triggersNode = std::make_unique<TriggersNode>();
  return triggersNode;
}