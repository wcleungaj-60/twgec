#include "frontend/parser.h"
#include <queue>
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

/***
 * ModuleNode = (BlockNode | MetadataNode)*
 */
std::unique_ptr<ModuleNode> Parser::parse() {
  std::unique_ptr<ModuleNode> moduleNode = std::make_unique<ModuleNode>();
  while (!tokens.empty()) {
    if (auto blockNode = parseBlock())
      moduleNode->blocks.push_back(std::move(blockNode));
    else if (auto metadataNode = parseMetadata())
      moduleNode->metadatas.push_back(std::move(metadataNode));
    else
      return nullptr;
  }
  return moduleNode;
}

/**
 * MetadataNode = MetadataToken + AssignToken + StringToken + SemicolonToken
 */
std::unique_ptr<MetadataNode> Parser::parseMetadata() {
  std::string key = tokens.front().value;
  if (!consume(TokenType::METADATA) || !consume(TokenType::ASSIGN) ||
      tokens.empty())
    return nullptr;
  std::string value = tokens.front().value;
  if (!consume(TokenType::STRING) || !consume(TokenType::SEMICOLON))
    return nullptr;
  std::unique_ptr<MetadataNode> metadataNode =
      std::make_unique<MetadataNode>(key, value);
  return metadataNode;
}

/**
 * BLOCK + IDENTIFIFER + OPENCUR + [ACTIONS|CHECKS|TRIGGERS] + CLOSECUR
 */
std::unique_ptr<BlockNode> Parser::parseBlock() {
  if (!consume(TokenType::BLOCK) || tokens.empty())
    return nullptr;
  std::string name = tokens.front().value;
  if (!consume(TokenType::IDENTIFIER) || !consume(TokenType::OPENCUR)) {
    std::cerr
        << "Expected syntax: expected: blocks identifer {/*block details*/}\n";
    return nullptr;
  }
  std::unique_ptr<BlockNode> blockNode = std::make_unique<BlockNode>(name);
  while (tokens.front().type != TokenType::CLOSECUR) {
    if (tokens.front().type == TokenType::ACTIONS) {
      auto actionsNode = parseActions();
      if (!actionsNode) {
        std::cerr << "found in block \"" << name << "\"\n";
        return nullptr;
      }
      blockNode->actionsNode = std::move(actionsNode);
    }
    if (tokens.front().type == TokenType::CHECKS) {
      auto checksNode = parseChecks();
      if (!checksNode) {
        std::cerr << "found in block \"" << name << "\"\n";
        return nullptr;
      }
      blockNode->checksNode = std::move(checksNode);
    }
    if (tokens.front().type == TokenType::TRIGGERS) {
      auto triggersNode = parseTriggers();
      if (!triggersNode) {
        std::cerr << "found in block \"" << name << "\"\n";
        return nullptr;
      }
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
  if (!consume(TokenType::ACTIONS) || !consume(TokenType::OPENCUR)) {
    std::cerr << "Expected syntax: actions {/*You actions*/}\n";
    return nullptr;
  }
  std::unique_ptr<ActionsNode> actionsNode = std::make_unique<ActionsNode>();
  while (tokens.front().type != TokenType::CLOSECUR) {
    auto actionNode = parseAction();
    if (!actionNode) {
      std::cerr << "Expected syntax: identifier(dot identifier)* \"(\"NULL | "
                   "((arg comma)* arg)\")\" semicolon\n";
      return nullptr;
    }
    actionsNode->actions.push_back(std::move(actionNode));
  }
  if (!consume(TokenType::CLOSECUR)) {
    std::cerr << "Expected syntax: actions {/*You actions*/}\n";
    return nullptr;
  }
  return actionsNode;
}

/**
 * ActionNode = IDENTIFIFER + (DOT + IDENTIFIFER)* + OPENPAR + NULL | ARGS +
 * CLOSEPAR + SEMICOLON
 */
std::unique_ptr<ActionNode> Parser::parseAction() {
  std::unique_ptr<ActionNode> actionNode = std::make_unique<ActionNode>();
  std::string id = tokens.front().value;
  if (!consume(TokenType::IDENTIFIER))
    return nullptr;
  actionNode->identifier.push_back(id);
  while (tokens.front().type != TokenType::OPENPAR) {
    if (!consume(TokenType::DOT))
      return nullptr;
    id = tokens.front().value;
    if (!consume(TokenType::IDENTIFIER))
      return nullptr;
    actionNode->identifier.push_back(id);
  }
  consume(TokenType::OPENPAR);
  while (tokens.front().type != TokenType::CLOSEPAR) {
    if (!parseActionArgs(actionNode))
      return nullptr;
  }
  if (!consume(TokenType::CLOSEPAR) || !consume(TokenType::SEMICOLON))
    return nullptr;
  return actionNode;
}

bool Parser::parseActionArgs(std::unique_ptr<ActionNode> &actionNode,
                             bool foundNamed) {
  std::string firstToken = tokens.front().value;
  if (!consume(TokenType::IDENTIFIER)) {
    // NAMED_ARGS = IDENTIFIFER EQUAL STRING (COMMA NAMED_ARGS)*
    if (foundNamed)
      return false;
    if (!consume(TokenType::STRING))
      return false;
    actionNode->positional_args.push_back(firstToken);
    if (consume(TokenType::COMMA))
      return parseActionArgs(actionNode);
  } else {
    // POSITIONAL_ARGS = (STRING (COMMA POSITIONAL_ARGS)*) | (IDENTIFIFER EQUAL
    // STRING (COMMA NAMED_ARGS)*)
    if (!consume(TokenType::ASSIGN))
      return false;
    std::string secondToken = tokens.front().value;
    if (!consume(TokenType::STRING))
      return false;
    actionNode->named_args.push_back({firstToken, secondToken});
    if (consume(TokenType::COMMA))
      return parseActionArgs(actionNode, true);
  }
  return tokens.front().type == TokenType::CLOSEPAR;
}

/**
 * CHECKS + OPENCUR + CLOSECUR
 */
std::unique_ptr<ChecksNode> Parser::parseChecks() {
  if (!consume(TokenType::CHECKS) || !consume(TokenType::OPENCUR) ||
      !consume(TokenType::CLOSECUR)) {
    std::cerr << "Expected syntax: checks {/*You checks*/}\n";
    return nullptr;
  }
  std::unique_ptr<ChecksNode> checksNode = std::make_unique<ChecksNode>();
  return checksNode;
}

/**
 * TRIGGERS + OPENCUR + CLOSECUR
 */
std::unique_ptr<TriggersNode> Parser::parseTriggers() {
  if (!consume(TokenType::TRIGGERS) || !consume(TokenType::OPENCUR) ||
      !consume(TokenType::CLOSECUR)) {
    std::cerr << "Expected syntax: triggers {/*You triggers*/}";
    return nullptr;
  }
  std::unique_ptr<TriggersNode> triggersNode = std::make_unique<TriggersNode>();
  return triggersNode;
}