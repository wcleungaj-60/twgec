#include "frontend/parser.h"
#include "ast.h"
#include "token.h"
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

Token Parser::getTokenWithIndex(int index) {
  std::queue<Token> newTokens;
  while (index > 0) {
    index--;
    newTokens.push(tokens.front());
    tokens.pop();
  }
  Token ret = tokens.front();
  while (!tokens.empty()) {
    newTokens.push(tokens.front());
    tokens.pop();
  }
  tokens = newTokens;
  return ret;
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

      auto funDefNode = parseFunDef();
      if (!funDefNode)
        return nullptr;
      moduleNode->funDefs.push_back(std::move(funDefNode));
    } else if (tokens.front().type == TokenType::SEMICOLON) {
      consume(TokenType::SEMICOLON);
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
      std::make_unique<MetadataNode>(strippedKey, std::move(exp), loc);
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
      std::make_unique<ConstDefNode>(key, std::move(exp), loc);
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
  if (!consume(TokenType::CLOSEPAR))
    return nullptr;
  if (!consume(TokenType::COLON))
    return nullptr;
  if (tokens.front().type == TokenType::BLOCK) {
    consume(TokenType::BLOCK);
    auto blockBody = parseBlockBody();
    if (!blockBody)
      return nullptr;
    funDefNode->blockBody = std::move(blockBody);
  } else {
    auto typedInstrSet = parseTypedInstrSet();
    if (!typedInstrSet)
      return nullptr;
    funDefNode->typedInstrSet = std::move(typedInstrSet);
  }
  return funDefNode;
}

std::unique_ptr<BlockNode> Parser::parseBlock() {
  consume(TokenType::BLOCK);
  std::string name = tokens.front().value;
  Location loc = tokens.front().location;
  if (!consume(TokenType::IDENTIFIER))
    return nullptr;
  std::unique_ptr<BlockNode> blockNode = std::make_unique<BlockNode>(name, loc);
  if (tokens.front().type == TokenType::ASSIGN) {
    consume(TokenType::ASSIGN);
    auto blockConstructor = parseInstruction();
    if (!blockConstructor)
      return nullptr;
    blockNode->blockConstructor = std::move(blockConstructor);
  } else {
    auto blockBody = parseBlockBody();
    if (!blockBody)
      return nullptr;
    blockNode->blockBody = std::move(blockBody);
  }
  return blockNode;
}

std::unique_ptr<BlockBodyNode> Parser::parseBlockBody() {
  Location loc = tokens.front().location;
  if (!consume(TokenType::OPENCUR))
    return nullptr;
  std::unique_ptr<BlockBodyNode> blockBodyNode =
      std::make_unique<BlockBodyNode>(loc);
  while (tokens.front().type != TokenType::CLOSECUR) {
    if (tokens.front().type == TokenType::ACTIONS ||
        tokens.front().type == TokenType::CHECKS ||
        tokens.front().type == TokenType::TRIGGERS) {
      auto typedInstrSetNode = parseTypedInstrSet();
      if (!typedInstrSetNode)
        return nullptr;
      blockBodyNode->typedInstrSets.push_back(std::move(typedInstrSetNode));
    } else if (tokens.front().type == TokenType::METADATA) {
      if (auto metadataNode = parseMetadata())
        blockBodyNode->metadatas.push_back(std::move(metadataNode));
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
  return blockBodyNode;
}

/**
 * Parser Typed Instruction Set:
 * Under this scope, `InstrSetNode`, `CompositeInstrNode`, `BranchNode`, and
 * `ForLoopNode` are the private class under this scope. `TypedInstructionSet`
 * will be used as either 1. FunDef or 2. Block Implmentation.
 */

std::unique_ptr<TypedInstrSetNode> Parser::parseTypedInstrSet() {
  Location loc = tokens.front().location;
  std::unique_ptr<TypedInstrSetNode> typedInstrSetNode =
      std::make_unique<TypedInstrSetNode>(loc);
  if (tokens.front().type == TokenType::ACTIONS) {
    consume(TokenType::ACTIONS);
    typedInstrSetNode->type = FUN_DEF_TYPE_ACTIONS;
  } else if (tokens.front().type == TokenType::CHECKS) {
    consume(TokenType::CHECKS);
    typedInstrSetNode->type = FUN_DEF_TYPE_CHECKS;
  } else if (tokens.front().type == TokenType::TRIGGERS) {
    consume(TokenType::TRIGGERS);
    typedInstrSetNode->type = FUN_DEF_TYPE_TRIGGERS;
  } else {
    std::cerr << "SyntaxError: Expecting a triggers/checks/actions as the "
                 "function type hint at "
              << tokens.front().location << ". Found \'" << tokens.front().value
              << "\'\n";
    return nullptr;
  }
  auto instrSetNode = parseInstrSet();
  if (!instrSetNode)
    return nullptr;
  typedInstrSetNode->instrSet = std::move(instrSetNode);
  return typedInstrSetNode;
}

std::unique_ptr<InstrSetNode> Parser::parseInstrSet() {
  Location loc = tokens.front().location;
  if (!consume(TokenType::OPENCUR))
    return nullptr;
  std::unique_ptr<InstrSetNode> instrSetNode =
      std::make_unique<InstrSetNode>(loc);
  while (tokens.front().type != TokenType::CLOSECUR) {
    auto compositeInstrNode = parseCompositeInstr();
    if (!compositeInstrNode)
      return nullptr;
    instrSetNode->instructions.push_back(std::move(compositeInstrNode));
  }
  if (!consume(TokenType::CLOSECUR))
    return nullptr;
  return instrSetNode;
}

std::unique_ptr<CompositeInstrNode> Parser::parseCompositeInstr() {
  Location loc = tokens.front().location;
  std::unique_ptr<CompositeInstrNode> compositeInstrNode =
      std::make_unique<CompositeInstrNode>(loc);
  if (tokens.front().type == TokenType::IDENTIFIER) {
    compositeInstrNode->instruction = std::move(parseInstruction());
    if (!compositeInstrNode->instruction)
      return nullptr;
  } else if (tokens.front().type == TokenType::IF) {
    compositeInstrNode->ifStatement = std::move(parseBranch());
    if (!compositeInstrNode->ifStatement)
      return nullptr;
  } else {
    std::cerr << "SyntaxError: Expecting an instruction or if-statement at "
              << tokens.front().location << ". Found \'" << tokens.front().value
              << "\'\n";
    return nullptr;
  }
  return compositeInstrNode;
}

std::unique_ptr<BranchNode> Parser::parseBranch() {
  Location loc = tokens.front().location;
  if (!consume(TokenType::IF) || !consume(TokenType::OPENPAR))
    return nullptr;
  auto condition = parseExp();
  if (!condition)
    return nullptr;
  if (!consume(TokenType::CLOSEPAR))
    return nullptr;
  auto trueBlock = parseInstrSet();
  if (!trueBlock)
    return nullptr;
  return std::make_unique<BranchNode>(std::move(condition),
                                      std::move(trueBlock), loc);
}

/**
 * Parser Instruction:
 * Instruction basically is a function call without any return value.
 * In global scope, instruction can be called as the block-typed function
 * application. In block scope, instruction can be called as the
 * actions/checks/triggers-typed function application.
 */

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
  bool isParsingNamedArg =
      foundNamed || (tokens.front().type == TokenType::IDENTIFIER &&
                     getTokenWithIndex(1).type == TokenType::ASSIGN);
  if (isParsingNamedArg) {
    if (!consume(TokenType::IDENTIFIER) || !consume(TokenType::ASSIGN))
      return false;
    std::unique_ptr<ExpressionNode> expNode = parseExp();
    if (!expNode)
      return false;
    std::unique_ptr<NamedArgNode> namedArgNode =
        std::make_unique<NamedArgNode>(identifierToken, expNode, loc);
    actionNode->named_args.push_back(std::move(namedArgNode));
    if (consume(TokenType::COMMA, /*errorThrowing*/ false))
      return parseInstructionArgs(actionNode, /*foundNamed*/ true);
  } else {
    std::unique_ptr<ExpressionNode> expNode = parseExp();
    if (!expNode)
      return false;
    std::unique_ptr<PositionalArgNode> posArgNode =
        std::make_unique<PositionalArgNode>(expNode, loc);
    actionNode->positional_args.push_back(std::move(posArgNode));
    if (consume(TokenType::COMMA, /*errorThrowing*/ false))
      return parseInstructionArgs(actionNode);
  }
  return tokens.front().type == TokenType::CLOSEPAR;
}

/**
 * Parser Expression:
 * ValueNode and OperationNode should only be accessible by the ExpNode
 * ExpNode is the node that encapsulate the value and operation
 */

std::unique_ptr<ExpressionNode> Parser::parseExp() {
  return parseExpLogicalOr();
}

std::unique_ptr<ExpressionNode> Parser::parseExpLogicalOr() {
  auto lhsExp = parseExpLogicalAnd();
  if (!lhsExp)
    return nullptr;
  if (!consume(TokenType::AND, /* errorThrowing */ false))
    return lhsExp;
  auto rhsExp = parseExpLogicalOr();
  if (!rhsExp)
    return nullptr;
  return std::make_unique<ExpressionNode>(std::move(lhsExp), EXP_OP_TYPE_AND,
                                          std::move(rhsExp), lhsExp->loc);
}

std::unique_ptr<ExpressionNode> Parser::parseExpLogicalAnd() {
  auto lhsExp = parseExpEquality();
  if (!lhsExp)
    return nullptr;
  if (!consume(TokenType::OR, /* errorThrowing */ false))
    return lhsExp;
  auto rhsExp = parseExpLogicalAnd();
  if (!rhsExp)
    return nullptr;
  return std::make_unique<ExpressionNode>(std::move(lhsExp), EXP_OP_TYPE_OR,
                                          std::move(rhsExp), lhsExp->loc);
}

std::unique_ptr<ExpressionNode> Parser::parseExpEquality() {
  auto lhsExp = parseExpRelational();
  if (!lhsExp)
    return nullptr;
  ExpOpType op = EXP_OP_TYPE_VOID;
  if (consume(TokenType::EQUAL, /* errorThrowing */ false))
    op = EXP_OP_TYPE_EQUAL;
  else if (consume(TokenType::NOT_EQUAL, /* errorThrowing */ false))
    op = EXP_OP_TYPE_NOT_EQUAL;
  else
    return lhsExp;
  auto rhsExp = parseExpEquality();
  if (!rhsExp)
    return nullptr;
  return std::make_unique<ExpressionNode>(std::move(lhsExp), op,
                                          std::move(rhsExp), lhsExp->loc);
}

std::unique_ptr<ExpressionNode> Parser::parseExpRelational() {
  auto lhsExp = parseExpAdditive();
  if (!lhsExp)
    return nullptr;
  ExpOpType op = EXP_OP_TYPE_VOID;
  if (consume(TokenType::LESS_THAN, /* errorThrowing */ false))
    op = EXP_OP_TYPE_LESS_THAN;
  else if (consume(TokenType::LESS_THAN_EQUAL, /* errorThrowing */ false))
    op = EXP_OP_TYPE_LESS_THAN_EQUAL;
  else if (consume(TokenType::GREATER_THAN, /* errorThrowing */ false))
    op = EXP_OP_TYPE_GREATER_THAN;
  else if (consume(TokenType::GREATER_THAN_EQUAL, /* errorThrowing */ false))
    op = EXP_OP_TYPE_GREATER_THAN_EQUAL;
  else
    return lhsExp;
  auto rhsExp = parseExpRelational();
  if (!rhsExp)
    return nullptr;
  return std::make_unique<ExpressionNode>(std::move(lhsExp), op,
                                          std::move(rhsExp), lhsExp->loc);
}

std::unique_ptr<ExpressionNode> Parser::parseExpAdditive() {
  auto lhsExp = parseExpMultiplicative();
  if (!lhsExp)
    return nullptr;
  ExpOpType op = EXP_OP_TYPE_VOID;
  if (consume(TokenType::ADD, /* errorThrowing */ false))
    op = EXP_OP_TYPE_ADD;
  else if (consume(TokenType::SUB, /* errorThrowing */ false))
    op = EXP_OP_TYPE_SUB;
  else
    return lhsExp;
  auto rhsExp = parseExpAdditive();
  if (!rhsExp)
    return nullptr;
  return std::make_unique<ExpressionNode>(std::move(lhsExp), op,
                                          std::move(rhsExp), lhsExp->loc);
}

std::unique_ptr<ExpressionNode> Parser::parseExpMultiplicative() {
  auto lhsExp = parseExpPrimivite();
  if (!lhsExp)
    return nullptr;
  ExpOpType op = EXP_OP_TYPE_VOID;
  if (consume(TokenType::MUL, /* errorThrowing */ false))
    op = EXP_OP_TYPE_MUL;
  else if (consume(TokenType::DIV, /* errorThrowing */ false))
    op = EXP_OP_TYPE_DIV;
  else if (consume(TokenType::MOD, /* errorThrowing */ false))
    op = EXP_OP_TYPE_MOD;
  else
    return lhsExp;
  auto rhsExp = parseExpMultiplicative();
  if (!rhsExp)
    return nullptr;
  return std::make_unique<ExpressionNode>(std::move(lhsExp), op,
                                          std::move(rhsExp), lhsExp->loc);
}

std::unique_ptr<ExpressionNode> Parser::parseExpPrimivite() {
  if (tokens.front().type == TokenType::OPENPAR) {
    if (!consume(TokenType::OPENPAR))
      return nullptr;
    auto expNode = parseExpLogicalOr();
    if (!consume(TokenType::CLOSEPAR))
      return nullptr;
    return expNode;
  } else if (auto valueNode = parseValue()) {
    return std::make_unique<ExpressionNode>(std::move(valueNode),
                                            (valueNode->loc));
  }
  return nullptr;
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
  if (consume(TokenType::POINT, false)) {
    consume(TokenType::OPENPAR);
    auto xExp = parseExp();
    if (!xExp)
      return nullptr;
    consume(TokenType::COMMA);
    auto yExp = parseExp();
    if (!yExp)
      return nullptr;
    consume(TokenType::CLOSEPAR);
    return std::make_unique<PointValueNode>(std::move(xExp), std::move(yExp),
                                            loc);
  }
  if (consume(TokenType::ACTOR_MATCH, false)) {
    std::unique_ptr<ActorMatchValueNode> actorMatchValueNode =
        std::make_unique<ActorMatchValueNode>(loc);
    consume(TokenType::OPENPAR);
    while (tokens.front().type != TokenType::CLOSEPAR) {
      if (!parseActorMatchArgs(actorMatchValueNode))
        return nullptr;
    }
    if (!consume(TokenType::CLOSEPAR))
      return nullptr;
    return actorMatchValueNode;
  }
  if (consume(TokenType::OPENSQR, false)) {
    std::unique_ptr<ListValueNode> listNode =
        std::make_unique<ListValueNode>(loc);
    bool firstItem = true;
    while (tokens.front().type != TokenType::CLOSESQR) {
      if (!firstItem)
        consume(TokenType::COMMA);
      std::unique_ptr<ExpressionNode> item = parseExp();
      if (!item)
        return nullptr;
      listNode->items.push_back(std::move(item));
      firstItem = false;
    }
    consume(TokenType::CLOSESQR);
    return listNode;
  }
  if (consume(TokenType::SUB, false)) {
    std::string value = tokens.front().value;
    if (consume(TokenType::INT))
      return std::make_unique<IntValueNode>(-std::stoi(value), loc);
  }
  std::cerr << "SyntaxError: Expecting a value (string/int/bool/point) at "
            << tokens.front().location << ". Found \'" << tokens.front().value
            << "\'\n";
  return nullptr;
}

// TODO: unify the instruction args and the actor Match args
bool Parser::parseActorMatchArgs(
    std::unique_ptr<ActorMatchValueNode> &actorMatchValueNode) {
  std::string identifierToken = tokens.front().value;
  Location loc = tokens.front().location;
  if (!consume(TokenType::IDENTIFIER) || !consume(TokenType::ASSIGN))
    return false;
  std::unique_ptr<ExpressionNode> expNode = parseExp();
  if (!expNode)
    return false;
  std::unique_ptr<NamedArgNode> namedArgNode =
      std::make_unique<NamedArgNode>(identifierToken, expNode, loc);
  actorMatchValueNode->named_args.push_back(std::move(namedArgNode));
  if (consume(TokenType::COMMA, /*errorThrowing*/ false))
    return parseActorMatchArgs(actorMatchValueNode);
  return tokens.front().type == TokenType::CLOSEPAR;
}