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
    } else if (tokens.front().type == TokenType::CONST) {
      if (auto constDefNode = parseConstDef())
        moduleNode->constDefs.push_back(std::move(constDefNode));
      else
        return nullptr;
    } else if (tokens.front().type == TokenType::DEF) {
      if (auto funDefNode = parseFunDef()) {
        if (moduleNode->funDefs.count(funDefNode->identifier) != 0) {
          std::cerr << "SyntaxError: Redefinition of function \'"
                    << funDefNode->identifier << "\' at " << funDefNode->loc
                    << ".\n";
          return nullptr;
        }
        moduleNode->funDefs.insert(
            {funDefNode->identifier, std::move(funDefNode)});
      } else {
        return nullptr;
      }
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
  std::unique_ptr<ExpressionNode> exp = parseExp();
  if (!exp)
    return nullptr;
  if (!consume(TokenType::SEMICOLON))
    return nullptr;
  std::unique_ptr<MetadataNode> metadataNode =
      std::make_unique<MetadataNode>(strippedKey, exp, loc);
  return metadataNode;
}

std::unique_ptr<ConstDefNode> Parser::parseConstDef() {
  if (!consume(TokenType::CONST))
    return nullptr;
  std::string key = tokens.front().value;
  Location loc = tokens.front().location;
  if (!consume(TokenType::IDENTIFIER) || !consume(TokenType::ASSIGN))
    return nullptr;
  std::unique_ptr<ExpressionNode> exp = parseExp();
  if (!exp)
    return nullptr;
  if (!consume(TokenType::SEMICOLON))
    return nullptr;
  std::unique_ptr<ConstDefNode> constDefNode =
      std::make_unique<ConstDefNode>(key, exp, loc);
  return constDefNode;
}

std::unique_ptr<FunDefNode> Parser::parseFunDef() {
  consume(TokenType::DEF);
  std::string identifier = tokens.front().value;
  Location loc = tokens.front().location;
  if (!consume(TokenType::IDENTIFIER))
    return nullptr;
  while (tokens.front().type != TokenType::OPENPAR) {
    if (!consume(TokenType::DOT))
      return nullptr;
    identifier += "." + tokens.front().value;
    if (!consume(TokenType::IDENTIFIER))
      return nullptr;
  }
  consume(TokenType::OPENPAR);
  std::unique_ptr<FunDefNode> funDefNode =
      std::make_unique<FunDefNode>(identifier, loc);
  if (tokens.front().type == TokenType::IDENTIFIER)
    while (true) {
      std::string paramName = tokens.front().value;
      consume(TokenType::IDENTIFIER);
      funDefNode->params.push_back(paramName);
      if (tokens.front().type == TokenType::CLOSEPAR)
        break;
      consume(TokenType::COMMA);
    }
  if (!consume(TokenType::CLOSEPAR) || !consume(TokenType::OPENCUR))
    return nullptr;
  while (tokens.front().type != TokenType::CLOSECUR) {
    auto instructionNode = parseInstruction();
    if (!instructionNode)
      return nullptr;
    funDefNode->instructions.push_back(std::move(instructionNode));
  }
  if (!consume(TokenType::CLOSECUR))
    return nullptr;
  return funDefNode;
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
    } else if (tokens.front().type == TokenType::METADATA) {
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
    auto instructionNode = parseInstruction();
    if (!instructionNode)
      return nullptr;
    actionsNode->instructions.push_back(std::move(instructionNode));
  }
  if (!consume(TokenType::CLOSECUR))
    return nullptr;
  return actionsNode;
}

std::unique_ptr<ChecksNode> Parser::parseChecks() {
  Location loc = tokens.front().location;
  if (!consume(TokenType::CHECKS) || !consume(TokenType::OPENCUR))
    return nullptr;
  std::unique_ptr<ChecksNode> checksNode = std::make_unique<ChecksNode>(loc);
  while (tokens.front().type != TokenType::CLOSECUR) {
    auto instructionNode = parseInstruction();
    if (!instructionNode)
      return nullptr;
    checksNode->instructions.push_back(std::move(instructionNode));
  }
  if (!consume(TokenType::CLOSECUR))
    return nullptr;
  return checksNode;
}

std::unique_ptr<TriggersNode> Parser::parseTriggers() {
  Location loc = tokens.front().location;
  if (!consume(TokenType::TRIGGERS) || !consume(TokenType::OPENCUR))
    return nullptr;
  std::unique_ptr<TriggersNode> triggersNode =
      std::make_unique<TriggersNode>(loc);
  while (tokens.front().type != TokenType::CLOSECUR) {
    auto instructionNode = parseInstruction();
    if (!instructionNode)
      return nullptr;
    triggersNode->instructions.push_back(std::move(instructionNode));
  }
  if (!consume(TokenType::CLOSECUR))
    return nullptr;
  return triggersNode;
}

std::unique_ptr<InstructionNode> Parser::parseInstruction() {
  std::string identifier = tokens.front().value;
  Location loc = tokens.front().location;
  if (!consume(TokenType::IDENTIFIER))
    return nullptr;
  while (tokens.front().type != TokenType::OPENPAR) {
    if (!consume(TokenType::DOT))
      return nullptr;
    identifier += "." + tokens.front().value;
    if (!consume(TokenType::IDENTIFIER))
      return nullptr;
  }
  std::unique_ptr<InstructionNode> instructionNode =
      std::make_unique<InstructionNode>(identifier, loc);
  consume(TokenType::OPENPAR);
  while (tokens.front().type != TokenType::CLOSEPAR) {
    if (!parseInstructionArgs(instructionNode))
      return nullptr;
  }
  if (!consume(TokenType::CLOSEPAR) || !consume(TokenType::SEMICOLON))
    return nullptr;
  return instructionNode;
}

bool Parser::parseInstructionArgs(std::unique_ptr<InstructionNode> &actionNode,
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
    std::unique_ptr<ExpressionNode> expNode = parseExp();
    if (!expNode)
      return false;
    std::unique_ptr<PositionalArgNode> posArgNode =
        std::make_unique<PositionalArgNode>(expNode, loc);
    actionNode->positional_args.push_back(std::move(posArgNode));
    if (consume(TokenType::COMMA, /*errorThrowing*/ false))
      return parseInstructionArgs(actionNode);
  } else {
    if (!consume(TokenType::ASSIGN))
      return false;
    std::unique_ptr<ExpressionNode> expNode = parseExp();
    if (!expNode)
      return false;
    std::unique_ptr<NamedArgNode> namedArgNode =
        std::make_unique<NamedArgNode>(identifierToken, expNode, loc);
    actionNode->named_args.push_back(std::move(namedArgNode));
    if (consume(TokenType::COMMA, /*errorThrowing*/ false))
      return parseInstructionArgs(actionNode, /*foundNamed*/ true);
  }
  return tokens.front().type == TokenType::CLOSEPAR;
}

std::unique_ptr<ValueNode> Parser::parseValue() {
  Location loc = tokens.front().location;
  std::string value = tokens.front().value;
  if (consume(TokenType::STRING, false))
    return std::make_unique<StringValueNode>(
        value.substr(1, value.length() - 2), loc);
  if (consume(TokenType::INT, false))
    return std::make_unique<IntValueNode>(std::stoi(value), loc);
  if (consume(TokenType::IDENTIFIER, false))
    return std::make_unique<VariableValueNode>(value, loc);
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

std::unique_ptr<ExpressionNode> Parser::parseExp() {
  Location lhsLoc = tokens.front().location;
  std::unique_ptr<ValueNode> lhsValue = parseValue();
  if (!lhsValue)
    return nullptr;
  std::unique_ptr<ExpressionNode> lhsExp =
      std::make_unique<ExpressionNode>(std::move(lhsValue), lhsLoc);
  if (tokens.front().type != TokenType::PLUS)
    return lhsExp;
  Location opLoc = tokens.front().location;
  consume(TokenType::PLUS);
  std::unique_ptr<OperationNode> opNode =
      std::make_unique<OperationPlusNode>(opLoc);
  Location rhsLoc = tokens.front().location;
  std::unique_ptr<ExpressionNode> rhsExp = parseExp();
  if (!rhsExp)
    return nullptr;
  return std::make_unique<ExpressionNode>(std::move(lhsExp), std::move(opNode),
                                          std::move(rhsExp), lhsExp->loc);
}