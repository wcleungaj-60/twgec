#include "frontend/parser.h"
#include <queue>
#include <vector>

bool Parser::consume(TokenType type, bool errorThrowing = true) {
  if (!tokens.empty() && tokens.front().type == type) {
    tokens.pop();
    return true;
  }
  if (errorThrowing)
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
  Location loc = tokens.front().location;
  std::unique_ptr<ModuleNode> moduleNode = std::make_unique<ModuleNode>(loc);
  while (!tokens.empty()) {
    if (tokens.front().type == TokenType::BLOCK) {
      if (auto blockNode = parseBlock())
        moduleNode->blocks.push_back(std::move(blockNode));
      else
        return nullptr;
    } else if (tokens.front().type == TokenType::METADATA) {
      if (auto metadataNode = parseMetadata())
        moduleNode->metadatas.push_back(std::move(metadataNode));
      else
        return nullptr;
    } else {
      std::cerr << "SyntaxError: Expecting block or metadata declaration at "
                << tokens.front().location << ". Found \'"
                << tokens.front().value << "\'\n";
      return nullptr;
    }
  }
  return moduleNode;
}

std::unique_ptr<MetadataNode> Parser::parseMetadata() {
  std::string key = tokens.front().value;
  Location loc = tokens.front().location;
  std::string strippedKey = key.substr(2, key.size() - 4);
  if (!consume(TokenType::METADATA) || !consume(TokenType::ASSIGN))
    return nullptr;
  std::unique_ptr<ValueNode> value = parseValue();
  if (!value)
    return nullptr;
  if (!consume(TokenType::SEMICOLON))
    return nullptr;
  std::unique_ptr<MetadataNode> metadataNode =
      std::make_unique<MetadataNode>(strippedKey, value, loc);
  return metadataNode;
}

std::unique_ptr<BlockNode> Parser::parseBlock() {
  consume(TokenType::BLOCK);
  std::string name = tokens.front().value;
  Location loc = tokens.front().location;
  if (!consume(TokenType::IDENTIFIER) || !consume(TokenType::OPENCUR))
    return nullptr;
  std::unique_ptr<BlockNode> blockNode = std::make_unique<BlockNode>(name, loc);
  while (tokens.front().type != TokenType::CLOSECUR) {
    if (tokens.front().type == TokenType::ACTIONS) {
      auto actionsNode = parseActions();
      if (!actionsNode)
        return nullptr;
      blockNode->actionsNode = std::move(actionsNode);
    } else if (tokens.front().type == TokenType::CHECKS) {
      auto checksNode = parseChecks();
      if (!checksNode)
        return nullptr;
      blockNode->checksNode = std::move(checksNode);
    } else if (tokens.front().type == TokenType::TRIGGERS) {
      auto triggersNode = parseTriggers();
      if (!triggersNode)
        return nullptr;
      blockNode->triggersNode = std::move(triggersNode);
    } else if(tokens.front().type == TokenType::METADATA) {
      if (auto metadataNode = parseMetadata())
        blockNode->metadatas.push_back(std::move(metadataNode));
      else
      return nullptr;
    } else {
      std::cerr << "SyntaxError: Expecting a triggers/checks/actions block or "
                   "metadata definition at "
                << tokens.front().location << ". Found \'"
                << tokens.front().value << "\'\n";
      return nullptr;
    }
  }
  if (!consume(TokenType::CLOSECUR))
    return nullptr;
  return blockNode;
}

std::unique_ptr<ActionsNode> Parser::parseActions() {
  Location loc = tokens.front().location;
  if (!consume(TokenType::ACTIONS) || !consume(TokenType::OPENCUR))
    return nullptr;
  std::unique_ptr<ActionsNode> actionsNode = std::make_unique<ActionsNode>(loc);
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
  std::string id = tokens.front().value;
  Location loc = tokens.front().location;
  std::unique_ptr<ActionNode> actionNode = std::make_unique<ActionNode>(loc);
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
  std::string identifierToken = tokens.front().value;
  Location loc = tokens.front().location;
  if (!consume(TokenType::IDENTIFIER, /*errorThrowing*/ false)) {
    if (foundNamed) {
      std::cerr << "SyntaxError: Expecting an identifier at "
                << tokens.front().location << ". Found \'"
                << tokens.front().value << "\'\n";
      return false;
    }
    std::unique_ptr<ValueNode> valueNode = parseValue();
    if (!valueNode)
      return false;
    std::unique_ptr<PositionalArgNode> posArgNode =
        std::make_unique<PositionalArgNode>(valueNode, loc);
    actionNode->positional_args.push_back(std::move(posArgNode));
    if (consume(TokenType::COMMA, /*errorThrowing*/ false))
      return parseActionArgs(actionNode);
  } else {
    if (!consume(TokenType::ASSIGN))
      return false;
    auto valueNode = parseValue();
    if (!valueNode)
      return false;
    std::unique_ptr<NamedArgNode> namedArgNode =
        std::make_unique<NamedArgNode>(identifierToken, valueNode, loc);
    actionNode->named_args.push_back(std::move(namedArgNode));
    if (consume(TokenType::COMMA, /*errorThrowing*/ false))
      return parseActionArgs(actionNode, true);
  }
  return tokens.front().type == TokenType::CLOSEPAR;
}

std::unique_ptr<ChecksNode> Parser::parseChecks() {
  Location loc = tokens.front().location;
  if (!consume(TokenType::CHECKS) || !consume(TokenType::OPENCUR) ||
      !consume(TokenType::CLOSECUR))
    return nullptr;
  std::unique_ptr<ChecksNode> checksNode = std::make_unique<ChecksNode>(loc);
  return checksNode;
}

std::unique_ptr<TriggersNode> Parser::parseTriggers() {
  Location loc = tokens.front().location;
  if (!consume(TokenType::TRIGGERS) || !consume(TokenType::OPENCUR) ||
      !consume(TokenType::CLOSECUR))
    return nullptr;
  std::unique_ptr<TriggersNode> triggersNode =
      std::make_unique<TriggersNode>(loc);
  return triggersNode;
}

std::unique_ptr<ValueNode> Parser::parseValue() {
  Location loc = tokens.front().location;
  std::string value = tokens.front().value;
  if (consume(TokenType::STRING, false))
    return std::make_unique<StringValueNode>(
        value.substr(1, value.length() - 2), loc);
  if (consume(TokenType::INT, false))
    return std::make_unique<IntValueNode>(std::stoi(value), loc);
  if (consume(TokenType::TRUE, false))
    return std::make_unique<BoolValueNode>(true, loc);
  if (consume(TokenType::FALSE, false))
    return std::make_unique<BoolValueNode>(false, loc);
  if (consume(TokenType::OPENPAR, false)) {
    std::string xLoc = tokens.front().value;
    consume(TokenType::INT);
    consume(TokenType::COMMA);
    std::string yLoc = tokens.front().value;
    consume(TokenType::INT);
    consume(TokenType::CLOSEPAR);
    return std::make_unique<PointValueNode>(std::stoi(xLoc), std::stoi(yLoc),
                                            loc);
  }
  if (consume(TokenType::OPENSQR, false)) {
    std::unique_ptr<ListValueNode> listNode =
        std::make_unique<ListValueNode>(loc);
    bool firstItem = true;
    while (tokens.front().type != TokenType::CLOSESQR) {
      if (!firstItem)
        consume(TokenType::COMMA);
      std::unique_ptr<ValueNode> item = parseValue();
      if (!item)
        return nullptr;
      listNode->items.push_back(std::move(item));
      firstItem = false;
    }
    consume(TokenType::CLOSESQR);
    return listNode;
  }
  std::cerr << "SyntaxError: Expecting a value (string/int/bool) at "
            << tokens.front().location << ". Found \'" << tokens.front().value
            << "\'\n";
  return nullptr;
}