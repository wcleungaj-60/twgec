#include "frontend/ast.h"
#include "utils/utils.h"
#include <iostream>

//------------ print ------------//
void ModuleNode::print(std::string title, int indent) {
  std::cout << "//----" << std::string(title.size(), '-') << "----//\n";
  std::cout << "//--- " << title << " ---//\n";
  std::cout << "//----" << std::string(title.size(), '-') << "----//\n";
  for (auto &metadata : metadatas)
    metadata->print(indent);
  for (auto &constDef : constDefs)
    constDef->print(indent);
  for (auto &funDef : funDefs)
    funDef->print(indent);
  for (auto &block : blocks)
    block->print(indent);
}

void MetadataNode::print(int indent) {
  std::cout << inden(indent) << "__" << key << "__ = " << *expNode << ";\n";
}

void FunDefNode::print(int indent) {
  std::cout << inden(indent) << "def " << identifier << "(";
  for (int i = 0; i < params.size(); i++) {
    std::cout << params[i];
    if (i != params.size() - 1)
      std::cout << ", ";
  }
  std::cout << ") : ";
  if (typedInstrSet)
    typedInstrSet->print(indent);
  if (blockBody) {
    std::cout << "block";
    blockBody->print(indent);
  }
}

void ConstDefNode::print(int indent) {
  std::cout << inden(indent) << "const " << key << " = " << *expNode << "\n";
}

void BlockNode::print(int indent) {
  std::cout << inden(indent) << "block " << identifier;
  if (blockBody)
    blockBody->print(indent);
  else {
    std::cout << " = ";
    blockConstructor->print(indent);
  }
}

void BlockBodyNode::print(int indent) {
  std::cout << " {\n";
  for (auto &metadata : metadatas)
    metadata->print(indent + 4);
  for (auto &typedInstrSet : typedInstrSets) {
    std::cout << inden(indent + 4);
    typedInstrSet->print(indent + 4);
  }
  std::cout << inden(indent) << "}\n";
}

void TypedInstrSetNode::print(int indent) {
  std::cout << type << " {\n";
  instrSet->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void InstrSetNode::print(int indent) {
  for (auto &compositeInstr : instructions)
    compositeInstr->print(indent);
}

void CompositeInstrNode::print(int indent) {
  if (instruction)
    instruction->print(indent);
  if (ifStatement)
    ifStatement->print(indent);
}

void BranchNode::print(int indent) {
  std::cout << inden(indent) << "if(" << *condition << ") {\n";
  trueBlock->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void InstructionNode::print(int indent) {
  std::cout << inden(indent) << identifier << "(";
  for (int i = 0; i < positional_args.size(); i++) {
    std::cout << *positional_args[i]->expNode;
    if (i != positional_args.size() - 1 || !named_args.empty())
      std::cout << ", ";
  }
  for (int i = 0; i < named_args.size(); i++) {
    std::cout << named_args[i].get()->key << " = ";
    std::cout << *named_args[i]->expNode;
    if (i != named_args.size() - 1)
      std::cout << ", ";
  }
  std::cout << ");\n";
}

//------------ clone ------------//
std::unique_ptr<MetadataNode> MetadataNode::clone() {
  return std::make_unique<MetadataNode>(key, expNode->clone(), loc);
}

std::unique_ptr<FunDefNode> FunDefNode::clone() {
  std::unique_ptr<FunDefNode> funDefNode;
  if (typedInstrSet) {
    funDefNode =
        std::make_unique<FunDefNode>(identifier, loc, typedInstrSet->clone());
  } else {
    funDefNode =
        std::make_unique<FunDefNode>(identifier, loc, blockBody->clone());
  }
  for (auto &param : params)
    funDefNode->params.push_back(param);
  return funDefNode;
}

std::unique_ptr<ConstDefNode> ConstDefNode::clone() {
  return std::make_unique<ConstDefNode>(key, expNode->clone(), loc);
}

std::unique_ptr<BlockNode> BlockNode::clone() {
  auto blockNode = std::make_unique<BlockNode>(identifier, loc);
  if (blockConstructor)
    blockNode->blockConstructor = blockConstructor->clone();
  if (blockBody)
    blockNode->blockBody = blockBody->clone();
  return blockNode;
}

std::unique_ptr<BlockBodyNode> BlockBodyNode::clone() {
  auto newNode = std::make_unique<BlockBodyNode>(loc);
  for (auto &typedInstr : typedInstrSets)
    newNode->typedInstrSets.push_back(typedInstr->clone());
  for (auto &metadata : metadatas)
    newNode->metadatas.push_back(metadata->clone());
  return newNode;
}

std::unique_ptr<TypedInstrSetNode> TypedInstrSetNode::clone() {
  auto newNode =
      std::make_unique<TypedInstrSetNode>(loc, type, instrSet->clone());
  return newNode;
}

std::unique_ptr<InstrSetNode> InstrSetNode::clone() {
  auto newNode = std::make_unique<InstrSetNode>(loc);
  for (auto &instr : instructions)
    newNode->instructions.push_back(instr->clone());
  return newNode;
}

std::unique_ptr<CompositeInstrNode> CompositeInstrNode::clone() {
  if (instruction)
    return std::make_unique<CompositeInstrNode>(loc, instruction->clone());
  else if (ifStatement)
    return std::make_unique<CompositeInstrNode>(loc, ifStatement->clone());
  return nullptr;
}

std::unique_ptr<BranchNode> BranchNode::clone() {
  return std::make_unique<BranchNode>(condition->clone(), trueBlock->clone(),
                                      loc);
}

std::unique_ptr<InstructionNode> InstructionNode::clone() {
  auto newNode = std::make_unique<InstructionNode>(identifier, loc);
  for (auto &namedArg : named_args)
    newNode.get()->named_args.push_back(namedArg.get()->clone());
  for (auto &positionalArg : positional_args)
    newNode.get()->positional_args.push_back(positionalArg.get()->clone());
  return newNode;
}

std::unique_ptr<NamedArgNode> NamedArgNode::clone() {
  auto newExp = std::move(expNode.get()->clone());
  return std::make_unique<NamedArgNode>(key, newExp, loc);
}

std::unique_ptr<PositionalArgNode> PositionalArgNode::clone() {
  auto newExp = std::move(expNode.get()->clone());
  return std::make_unique<PositionalArgNode>(newExp, loc);
}

std::unique_ptr<ExpressionNode> ExpressionNode::clone() {
  if (isValue)
    return std::make_unique<ExpressionNode>(value->clone(), loc);
  else
    return std::make_unique<ExpressionNode>(lhs->clone(), op, rhs->clone(),
                                            loc);
}

std::unique_ptr<ValueNode> ValueNode::clone() {
  if (auto *stringNode = dynamic_cast<StringValueNode *>(this))
    return stringNode->clone();
  if (auto *intNode = dynamic_cast<IntValueNode *>(this))
    return intNode->clone();
  if (auto *boolNode = dynamic_cast<BoolValueNode *>(this))
    return boolNode->clone();
  if (auto *listNode = dynamic_cast<ListValueNode *>(this))
    return listNode->clone();
  if (auto *varNode = dynamic_cast<VariableValueNode *>(this))
    return varNode->clone();
  if (auto *ptNode = dynamic_cast<PointValueNode *>(this))
    return ptNode->clone();
  if (auto *amNode = dynamic_cast<ActorMatchValueNode *>(this))
    return amNode->clone();
  std::cerr << "Compiler Implementation Error: Unsupported cloning . Found at "
            << loc << "\n";
  return nullptr;
}

std::unique_ptr<ValueNode> ListValueNode::clone() {
  auto newNode = std::make_unique<ListValueNode>(loc);
  for (auto &item : items)
    newNode.get()->items.push_back(item->clone());
  return newNode;
}

std::unique_ptr<ValueNode> PointValueNode::clone() {
  return std::make_unique<PointValueNode>(x->clone(), y->clone(), loc);
}

std::unique_ptr<ValueNode> ActorMatchValueNode::clone() {
  auto newNode = std::make_unique<ActorMatchValueNode>(loc);
  for (auto &namedArg : this->named_args)
    newNode->named_args.push_back(namedArg->clone());
  return newNode;
}

std::unique_ptr<ValueNode> StringValueNode::clone() {
  return std::make_unique<StringValueNode>(value, loc);
}

std::unique_ptr<ValueNode> IntValueNode::clone() {
  return std::make_unique<IntValueNode>(value, loc);
}

std::unique_ptr<ValueNode> BoolValueNode::clone() {
  return std::make_unique<BoolValueNode>(value, loc);
}

std::unique_ptr<ValueNode> VariableValueNode::clone() {
  return std::make_unique<VariableValueNode>(value, loc);
}

//------------ propagateExp ------------//
bool ModuleNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  for (auto &metadata : metadatas)
    ret &= metadata->propagateExp(varExpMap);
  for (auto &block : blocks)
    ret &= block->propagateExp(varExpMap);
  return ret;
}

bool MetadataNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return expNode->propagateExp(varExpMap);
}

bool BlockNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return blockBody->propagateExp(varExpMap);
}

bool BlockBodyNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  for (auto &metadata : metadatas)
    ret &= metadata->propagateExp(varExpMap);
  for (auto &typedInstr : typedInstrSets)
    ret &= typedInstr->propagateExp(varExpMap);
  return ret;
}

bool TypedInstrSetNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return instrSet->propagateExp(varExpMap);
}

bool InstrSetNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  for (auto &compositeInstr : instructions)
    ret &= compositeInstr->propagateExp(varExpMap);
  return ret;
}

bool CompositeInstrNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  if (instruction)
    return instruction->propagateExp(varExpMap);
  if (ifStatement)
    return ifStatement->propagateExp(varExpMap);
  return true;
}

bool BranchNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  ret &= trueBlock->propagateExp(varExpMap);
  ret &= condition->propagateExp(varExpMap);
  return ret;
}

bool InstructionNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  for (auto &namedArg : named_args)
    ret &= namedArg->propagateExp(varExpMap);
  for (auto &positionalArg : positional_args)
    ret &= positionalArg->propagateExp(varExpMap);
  return ret;
}

bool NamedArgNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return expNode->propagateExp(varExpMap);
}

bool PositionalArgNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return expNode->propagateExp(varExpMap);
}

bool ExpressionNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  if (isValue) {
    if (auto *varNode = dynamic_cast<VariableValueNode *>(value.get())) {
      if (varExpMap.find(varNode->value) == varExpMap.end())
        return false;
      auto &constExp = varExpMap[varNode->value];
      if (constExp->isValue) {
        value = constExp->value->clone();
      } else {
        value = nullptr;
        isValue = false;
        lhs = constExp->lhs->clone();
        rhs = constExp->rhs->clone();
        op = constExp->op;
      }
    } else if (auto *listNode = dynamic_cast<ListValueNode *>(value.get())) {
      for (auto &item : listNode->items)
        item->propagateExp(varExpMap);
    } else if (auto *actorMatchNode =
                   dynamic_cast<ActorMatchValueNode *>(value.get())) {
      for (auto &namedArg : actorMatchNode->named_args)
        namedArg->propagateExp(varExpMap);
    }
  } else {
    ret &= lhs->propagateExp(varExpMap);
    ret &= rhs->propagateExp(varExpMap);
  }
  return ret;
}

//------------ foldValue ------------//
bool ModuleNode::foldValue() {
  bool ret = true;
  for (auto &metadata : metadatas)
    ret &= metadata->foldValue();
  for (auto &block : blocks)
    ret &= block->foldValue();
  return ret;
}

bool MetadataNode::foldValue() { return expNode->foldValue(); }

bool BlockNode::foldValue() { return blockBody->foldValue(); }

bool BlockBodyNode::foldValue() {
  bool ret = true;
  for (auto &metadata : metadatas)
    ret &= metadata->foldValue();
  for (auto &typedInstr : typedInstrSets)
    ret &= typedInstr->foldValue();
  return ret;
}

bool TypedInstrSetNode::foldValue() { return instrSet->foldValue(); }

bool InstrSetNode::foldValue() {
  bool ret = true;
  for (auto &compositeInstr : instructions)
    ret &= compositeInstr->foldValue();
  return ret;
}

bool CompositeInstrNode::foldValue() {
  if (instruction)
    return instruction->foldValue();
  if (ifStatement)
    return ifStatement->foldValue();
  return true;
}

bool BranchNode::foldValue() {
  bool ret = true;
  ret &= trueBlock->foldValue();
  ret &= condition->foldValue();
  return ret;
}

bool InstructionNode::foldValue() {
  bool ret = true;
  for (auto &namedArg : named_args)
    ret &= namedArg->foldValue();
  for (auto &positionalArg : positional_args)
    ret &= positionalArg->foldValue();
  return ret;
}

bool NamedArgNode::foldValue() { return expNode->foldValue(); }

bool PositionalArgNode::foldValue() { return expNode->foldValue(); }

bool ExpressionNode::foldValue() {
  if (isValue) {
    bool ret = true;
    auto valuePoint = dynamic_cast<PointValueNode *>(value.get());
    auto valueActorMatch = dynamic_cast<ActorMatchValueNode *>(value.get());
    auto valueList = dynamic_cast<ListValueNode *>(value.get());
    if (valuePoint) {
      ret &= valuePoint->x->foldValue();
      ret &= valuePoint->y->foldValue();
    } else if (valueList) {
      for (auto &item : valueList->items) {
        ret &= item->foldValue();
      }
    } else if (valueActorMatch) {
      for (auto &namedArg : valueActorMatch->named_args)
        ret &= namedArg.get()->foldValue();
    }
    return ret;
  }
  if (!lhs->foldValue() || !lhs->isValue)
    return false;
  if (!rhs->foldValue() || !rhs->isValue)
    return false;
  auto isAnd = op == EXP_OP_TYPE_AND;
  auto isOr = op == EXP_OP_TYPE_OR;
  auto isEqual = op == EXP_OP_TYPE_EQUAL;
  auto isNotEqual = op == EXP_OP_TYPE_NOT_EQUAL;
  auto isLessThan = op == EXP_OP_TYPE_LESS_THAN;
  auto isLessThanEqual = op == EXP_OP_TYPE_LESS_THAN_EQUAL;
  auto isGreaterThan = op == EXP_OP_TYPE_GREATER_THAN;
  auto isGreaterThanEqual = op == EXP_OP_TYPE_GREATER_THAN_EQUAL;
  auto isAdd = op == EXP_OP_TYPE_ADD;
  auto isSub = op == EXP_OP_TYPE_SUB;
  auto isMul = op == EXP_OP_TYPE_MUL;
  auto isDiv = op == EXP_OP_TYPE_DIV;
  auto isMod = op == EXP_OP_TYPE_MOD;
  auto lhsStr = dynamic_cast<StringValueNode *>(lhs->value.get());
  auto rhsStr = dynamic_cast<StringValueNode *>(rhs->value.get());
  auto lhsInt = dynamic_cast<IntValueNode *>(lhs->value.get());
  auto rhsInt = dynamic_cast<IntValueNode *>(rhs->value.get());
  auto lhsBool = dynamic_cast<BoolValueNode *>(lhs->value.get());
  auto rhsBool = dynamic_cast<BoolValueNode *>(rhs->value.get());
  bool isFolded = false;
  if (lhsStr && rhsStr) {
    if (isAdd) {
      value = std::make_unique<StringValueNode>(lhsStr->value + rhsStr->value,
                                                lhsStr->loc);
      isFolded = true;
    } else if (isEqual) {
      value = std::make_unique<BoolValueNode>(lhsStr->value == rhsStr->value,
                                              lhsStr->loc);
      isFolded = true;
    } else if (isNotEqual) {
      value = std::make_unique<BoolValueNode>(lhsStr->value != rhsStr->value,
                                              lhsStr->loc);
      isFolded = true;
    }
  } else if (lhsInt && rhsInt) {
    if (isAdd) {
      value = std::make_unique<IntValueNode>(lhsInt->value + rhsInt->value,
                                             lhsInt->loc);
      isFolded = true;
    } else if (isSub) {
      value = std::make_unique<IntValueNode>(lhsInt->value - rhsInt->value,
                                             lhsInt->loc);
      isFolded = true;
    } else if (isMul) {
      value = std::make_unique<IntValueNode>(lhsInt->value * rhsInt->value,
                                             lhsInt->loc);
      isFolded = true;
    } else if (isDiv) {
      value = std::make_unique<IntValueNode>(lhsInt->value / rhsInt->value,
                                             lhsInt->loc);
      isFolded = true;
    } else if (isMod) {
      value = std::make_unique<IntValueNode>(lhsInt->value % rhsInt->value,
                                             lhsInt->loc);
      isFolded = true;
    } else if (isEqual) {
      value = std::make_unique<BoolValueNode>(lhsInt->value == rhsInt->value,
                                              lhsInt->loc);
      isFolded = true;
    } else if (isNotEqual) {
      value = std::make_unique<BoolValueNode>(lhsInt->value != rhsInt->value,
                                              lhsInt->loc);
      isFolded = true;
    } else if (isLessThan) {
      value = std::make_unique<BoolValueNode>(lhsInt->value < rhsInt->value,
                                              lhsInt->loc);
      isFolded = true;
    } else if (isLessThanEqual) {
      value = std::make_unique<BoolValueNode>(lhsInt->value <= rhsInt->value,
                                              lhsInt->loc);
      isFolded = true;
    } else if (isGreaterThan) {
      value = std::make_unique<BoolValueNode>(lhsInt->value > rhsInt->value,
                                              lhsInt->loc);
      isFolded = true;
    } else if (isGreaterThanEqual) {
      value = std::make_unique<BoolValueNode>(lhsInt->value >= rhsInt->value,
                                              lhsInt->loc);
      isFolded = true;
    }
  } else if (lhsBool && rhsBool) {
    if (isEqual) {
      value = std::make_unique<BoolValueNode>(lhsBool->value == rhsBool->value,
                                              lhsBool->loc);
      isFolded = true;
    } else if (isNotEqual) {
      value = std::make_unique<BoolValueNode>(lhsBool->value != rhsBool->value,
                                              lhsBool->loc);
      isFolded = true;
    } else if (isAnd) {
      value = std::make_unique<BoolValueNode>(lhsBool->value && rhsBool->value,
                                              lhsBool->loc);
      isFolded = true;
    } else if (isOr) {
      value = std::make_unique<BoolValueNode>(lhsBool->value || rhsBool->value,
                                              lhsBool->loc);
      isFolded = true;
    }
  }
  if (isFolded) {
    isValue = true;
    lhs = nullptr;
    rhs = nullptr;
  }
  return isFolded;
}

//------------ hasUnresolvedValue ------------//
bool ModuleNode::hasUnresolvedValue() {
  bool ret = false;
  for (auto &metadata : metadatas)
    ret |= metadata->hasUnresolvedValue();
  for (auto &block : blocks)
    ret |= block->hasUnresolvedValue();
  return ret;
}

bool MetadataNode::hasUnresolvedValue() {
  return expNode->hasUnresolvedValue();
}

bool BlockNode::hasUnresolvedValue() { return blockBody->hasUnresolvedValue(); }

bool BlockBodyNode::hasUnresolvedValue() {
  bool ret = false;
  for (auto &metadata : metadatas)
    ret |= metadata->hasUnresolvedValue();
  for (auto &typedInstr : typedInstrSets)
    ret |= typedInstr->hasUnresolvedValue();
  return ret;
}

bool TypedInstrSetNode::hasUnresolvedValue() {
  return instrSet->hasUnresolvedValue();
}

bool InstrSetNode::hasUnresolvedValue() {
  bool ret = false;
  for (auto &compositeInstr : instructions)
    ret |= compositeInstr->hasUnresolvedValue();
  return ret;
}

bool CompositeInstrNode::hasUnresolvedValue() {
  if (instruction)
    return instruction->hasUnresolvedValue();
  if (ifStatement)
    return ifStatement->hasUnresolvedValue();
  return true;
}

bool BranchNode::hasUnresolvedValue() {
  bool ret = false;
  ret |= trueBlock->hasUnresolvedValue();
  ret |= condition->hasUnresolvedValue();
  return ret;
}

bool InstructionNode::hasUnresolvedValue() {
  bool ret = false;
  for (auto &namedArg : named_args)
    ret |= namedArg->hasUnresolvedValue();
  for (auto &positionalArg : positional_args)
    ret |= positionalArg->hasUnresolvedValue();
  return ret;
}

bool NamedArgNode::hasUnresolvedValue() {
  return expNode->hasUnresolvedValue();
}

bool PositionalArgNode::hasUnresolvedValue() {
  return expNode->hasUnresolvedValue();
}

bool ExpressionNode::hasUnresolvedValue() {
  if (isValue) {
    if (auto varNode = dynamic_cast<VariableValueNode *>(value.get())) {
      std::cerr << "Compilation Error: Found unresolvable variable `"
                << varNode->value << "` at " << varNode->loc << "\n";
      return true;
    }
    return false;
  } else {
    std::cerr << "Compilation Error: Found unresolvable expression `" << *this
              << "` at " << loc << "\n";
    return true;
  }
}