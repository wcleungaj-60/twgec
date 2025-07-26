#include "frontend/ast.h"
#include "utils/utils.h"
#include <iostream>

//------------ print ------------//

void ListValueNode::print() {
  std::cout << "[";
  for (auto i = 0; i < items.size(); i++) {
    items[i]->print();
    if (i != items.size() - 1)
      std::cout << ",";
  }
  std::cout << "]";
};

void PointValueNode::print() { std::cout << "(" << x << "," << y << ")"; };

void StringValueNode::print() { std::cout << "\"" << value << "\""; };

void IntValueNode::print() { std::cout << value; };

void BoolValueNode::print() { std::cout << (value ? "true" : "false"); };

void VariableValueNode::print() { std::cout << value; };

void OperationPlusNode::print() { std::cout << "+"; };

void OperationNoOpNode::print() { std::cout << ""; };

void ExpressionNode::print() {
  if (isValue) {
    value->print();
  } else {
    std::cout << "(";
    lhs->print();
    std::cout << " ";
    op->print();
    std::cout << " ";
    rhs->print();
    std::cout << ")";
  }
};

void ModuleNode::print(std::string title, int indent) {
  std::cout << "//--- " << title << " ---//\n";
  std::cout << inden(indent) << "ModuleNode: " << "\n";
  for (auto &metadata : metadatas)
    metadata->print(indent + 4);
  for (auto &constDef : constDefs)
    constDef->print(indent + 4);
  for (auto &funDef : funDefs)
    funDef.second->print(indent + 4);
  for (auto &block : blocks)
    block->print(indent + 4);
}

void MetadataNode::print(int indent) {
  std::cout << inden(indent) << "MetadataNode: " << key << " = ";
  expNode->print();
  std::cout << "\n";
}

void ConstDefNode::print(int indent) {
  std::cout << inden(indent) << "const " << key << " = ";
  expNode->print();
  std::cout << "\n";
}

void BlockNode::print(int indent) {
  std::cout << inden(indent) << "BlockNode: " << identifier << " {\n";
  for (auto &metadata : metadatas)
    metadata->print(indent + 4);
  if (checksNode)
    checksNode->print(indent + 4);
  if (triggersNode)
    triggersNode->print(indent + 4);
  if (actionsNode)
    actionsNode->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void ChecksNode::print(int indent) {
  std::cout << inden(indent) << "ChecksNode {\n";
  for (auto &ins : instructions)
    ins.get()->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void TriggersNode::print(int indent) {
  std::cout << inden(indent) << "TriggersNode {\n";
  for (auto &ins : instructions)
    ins.get()->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void ActionsNode::print(int indent) {
  std::cout << inden(indent) << "ActionsNode {\n";
  for (auto &ins : instructions)
    ins.get()->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void FunDefNode::print(int indent) {
  std::cout << inden(indent) << "def " << identifier << "(";
  for (int i = 0; i < params.size(); i++) {
    std::cout << params[i];
    if (i != params.size() - 1)
      std::cout << ", ";
  }
  std::cout << ") {\n";
  for (auto &ins : instructions)
    ins.get()->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void InstructionNode::print(int indent) {
  std::cout << inden(indent) << identifier << "(";
  for (int i = 0; i < positional_args.size(); i++) {
    positional_args[i]->expNode->print();
    if (i != positional_args.size() - 1 || !named_args.empty())
      std::cout << ", ";
  }
  for (int i = 0; i < named_args.size(); i++) {
    std::cout << named_args[i].get()->key << " = ";
    named_args[i]->expNode->print();
    if (i != named_args.size() - 1)
      std::cout << ", ";
  }
  std::cout << ")\n";
}

//------------ clone ------------//

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
  std::cerr << "Compiler Implementation Error: Unsupported cloning . Found at "
            << loc << "\n";
  return nullptr;
}

std::unique_ptr<ValueNode> ListValueNode::clone() {
  auto newNode = std::make_unique<ListValueNode>(loc);
  for (auto &item : items)
    newNode.get()->items.push_back(item->clone());
  return newNode;
};

std::unique_ptr<ValueNode> PointValueNode::clone() {
  return std::make_unique<PointValueNode>(x, y, loc);
};

std::unique_ptr<ValueNode> StringValueNode::clone() {
  return std::make_unique<StringValueNode>(value, loc);
};

std::unique_ptr<ValueNode> IntValueNode::clone() {
  return std::make_unique<IntValueNode>(value, loc);
};

std::unique_ptr<ValueNode> BoolValueNode::clone() {
  return std::make_unique<BoolValueNode>(value, loc);
};

std::unique_ptr<ValueNode> VariableValueNode::clone() {
  return std::make_unique<VariableValueNode>(value, loc);
};

std::unique_ptr<OperationNode> OperationNode::clone() {
  if (auto *plusNode = dynamic_cast<OperationPlusNode *>(this))
    return plusNode->clone();
  std::cerr << "Compiler Implementation Error: Unsupported cloning . Found at "
            << loc << "\n";
  return nullptr;
}

std::unique_ptr<OperationNode> OperationPlusNode::clone() {
  return std::make_unique<OperationPlusNode>(loc);
}

std::unique_ptr<OperationNode> OperationNoOpNode::clone() {
  return std::make_unique<OperationNoOpNode>(loc);
}

std::unique_ptr<ExpressionNode> ExpressionNode::clone() {
  if (isValue)
    return std::make_unique<ExpressionNode>(value->clone(), loc);
  else
    return std::make_unique<ExpressionNode>(lhs->clone(), op->clone(),
                                            rhs->clone(), loc);
};

std::unique_ptr<NamedArgNode> NamedArgNode::clone() {
  auto newExp = std::move(expNode.get()->clone());
  return std::make_unique<NamedArgNode>(key, newExp, loc);
}

std::unique_ptr<PositionalArgNode> PositionalArgNode::clone() {
  auto newExp = std::move(expNode.get()->clone());
  return std::make_unique<PositionalArgNode>(newExp, loc);
}

std::unique_ptr<InstructionNode> InstructionNode::clone() {
  auto newNode = std::make_unique<InstructionNode>(identifier, loc);
  for (auto &namedArg : named_args)
    newNode.get()->named_args.push_back(namedArg.get()->clone());
  for (auto &positionalArg : positional_args)
    newNode.get()->positional_args.push_back(positionalArg.get()->clone());
  return newNode;
}

std::unique_ptr<FunDefNode> FunDefNode::clone() {
  auto newNode = std::make_unique<FunDefNode>(identifier, loc);
  for (auto &param : params)
    newNode.get()->params.push_back(param);
  for (auto &instr : instructions)
    newNode.get()->instructions.push_back(instr.get()->clone());
  return newNode;
}

//------------ others ------------//

bool ExpressionNode::propagateVarToExp(
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
        op = constExp->op->clone();
      }
    }
  } else {
    ret &= lhs->propagateVarToExp(varExpMap);
    ret &= rhs->propagateVarToExp(varExpMap);
  }
  return ret;
}

bool ExpressionNode::foldValue() {
  if (isValue)
    return true;
  if (!lhs->foldValue() || !lhs->isValue)
    return false;
  if (!rhs->foldValue() || !rhs->isValue)
    return false;
  if (auto plusOp = dynamic_cast<OperationPlusNode *>(op.get()))
    if (auto lhsStr = dynamic_cast<StringValueNode *>(lhs->value.get()))
      if (auto rhsStr = dynamic_cast<StringValueNode *>(rhs->value.get())) {
        value = std::make_unique<StringValueNode>(lhsStr->value + rhsStr->value,
                                                  lhsStr->loc);
        isValue = true;
        lhs = nullptr;
        rhs = nullptr;
        return true;
      }
  return false;
}