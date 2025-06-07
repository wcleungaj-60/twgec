#include "frontend/parser.h"
#include <queue>
#include <vector>

bool Parser::consume(TokenType type, bool errorThrowing = true) {
  if (!tokens.empty() && tokens.front().type == type) {
    tokens.pop();
    return true;
  }
  if(errorThrowing)
      std::cerr << "SyntaxError: Expecting " << type << " at " 
        << tokens.front().location << ". Found \'" << tokens.front().value
        << "\'\n";
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
    if (tokens.front().type == TokenType::BLOCK) {
      if (auto blockNode = parseBlock())
        moduleNode->blocks.push_back(std::move(blockNode));
      else
        return nullptr;
    } else if(tokens.front().type == TokenType::METADATA) {
      if (auto metadataNode = parseMetadata())
        moduleNode->metadatas.push_back(std::move(metadataNode));
      else
        return nullptr;
    } else {
      std::cerr << "SyntaxError: Expecting block or metadata declaration at " 
        << tokens.front().location << ". Found \'" << tokens.front().value <<
        "\'\n";
      return nullptr;
    }
  }
  return moduleNode;
}

std::unique_ptr<MetadataNode> Parser::parseMetadata() {
  std::string key = tokens.front().value;
  std::string strippedKey = key.substr(2, key.size() - 4);
  if (!consume(TokenType::METADATA) || !consume(TokenType::ASSIGN))
    return nullptr;
  std::string value = tokens.front().value;
  if (!consume(TokenType::STRING) || !consume(TokenType::SEMICOLON))
    return nullptr;
  std::unique_ptr<MetadataNode> metadataNode =
      std::make_unique<MetadataNode>(strippedKey, value);
  return metadataNode;
}

std::unique_ptr<BlockNode> Parser::parseBlock() {
  consume(TokenType::BLOCK);
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

std::unique_ptr<ActionsNode> Parser::parseActions() {
  if (!consume(TokenType::ACTIONS) || !consume(TokenType::OPENCUR))
    return nullptr;
  std::unique_ptr<ActionsNode> actionsNode = std::make_unique<ActionsNode>();
  while (tokens.front().type != TokenType::CLOSECUR) {
    auto actionNode = parseAction();
    if (!actionNode)
      return nullptr;
    actionsNode->actions.push_back(std::move(actionNode));
  }
  if (!consume(TokenType::CLOSECUR))
    return nullptr;
  return actionsNode;
}

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
  if (!consume(TokenType::IDENTIFIER, /*errorThrowing*/ false)) {
    if (foundNamed) {
      std::cerr << "SyntaxError: Expecting an identifier at " 
        << tokens.front().location << ". Found \'" << tokens.front().value
        << "\'\n";
      return false;
    }
    if (!consume(TokenType::STRING))
      return false;
    actionNode->positional_args.push_back(firstToken);
    if (consume(TokenType::COMMA, /*errorThrowing*/ false))
      return parseActionArgs(actionNode);
  } else {
    if (!consume(TokenType::ASSIGN))
      return false;
    std::string secondToken = tokens.front().value;
    if (!consume(TokenType::STRING))
      return false;
    actionNode->named_args.push_back({firstToken, secondToken});
    if (consume(TokenType::COMMA, /*errorThrowing*/ false))
      return parseActionArgs(actionNode, true);
  }
  return tokens.front().type == TokenType::CLOSEPAR;
}

std::unique_ptr<ChecksNode> Parser::parseChecks() {
  if (!consume(TokenType::CHECKS) || !consume(TokenType::OPENCUR) || 
    !consume(TokenType::CLOSECUR))
    return nullptr;
  std::unique_ptr<ChecksNode> checksNode = std::make_unique<ChecksNode>();
  return checksNode;
}

std::unique_ptr<TriggersNode> Parser::parseTriggers() {
  if (!consume(TokenType::TRIGGERS) || !consume(TokenType::OPENCUR) ||
      !consume(TokenType::CLOSECUR))
    return nullptr;
  std::unique_ptr<TriggersNode> triggersNode = std::make_unique<TriggersNode>();
  return triggersNode;
}