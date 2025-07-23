#ifndef AST_H
#define AST_H

#include "utils/location.h"
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class ValueNode {
public:
  // Variable
  Location loc;
  // Function
  ValueNode(Location loc) : loc(loc) {}
  virtual void print() const {}
  virtual std::unique_ptr<ValueNode> clone() const;
};

class ListValueNode : public ValueNode {
public:
  // Variable
  std::vector<std::unique_ptr<ValueNode>> items = {};
  // Function
  ListValueNode(Location loc) : ValueNode(loc) {}
  virtual void print() const {
    std::cout << "[";
    for (auto i = 0; i < items.size(); i++) {
      items[i]->print();
      if (i != items.size() - 1)
        std::cout << ",";
    }
    std::cout << "]";
  };
  std::unique_ptr<ValueNode> clone() const {
    auto newNode = std::make_unique<ListValueNode>(loc);
    for (auto &item : items)
      newNode.get()->items.push_back(item->clone());
    return newNode;
  };
};

class PointValueNode : public ValueNode {
public:
  // Variable
  int x;
  int y;
  // Function
  PointValueNode(int x, int y, Location loc) : ValueNode(loc), x(x), y(y) {}
  virtual void print() const { std::cout << "(" << x << "," << y << ")"; };
  std::unique_ptr<ValueNode> clone() const {
    return std::make_unique<PointValueNode>(x, y, loc);
  };
};

class StringValueNode : public ValueNode {
public:
  // Variable
  std::string value;
  // Function
  StringValueNode(std::string value, Location loc)
      : ValueNode(loc), value(value) {}
  void print() const { std::cout << "\"" << value << "\""; }
  std::unique_ptr<ValueNode> clone() const {
    return std::make_unique<StringValueNode>(value, loc);
  };
};

class IntValueNode : public ValueNode {
public:
  // Variable
  int value;
  // Function
  IntValueNode(int value, Location loc) : ValueNode(loc), value(value) {}
  void print() const { std::cout << value; }
  std::unique_ptr<ValueNode> clone() const {
    return std::make_unique<IntValueNode>(value, loc);
  };
};

class BoolValueNode : public ValueNode {
public:
  // Variable
  bool value;
  // Function
  BoolValueNode(bool value, Location loc) : ValueNode(loc), value(value) {}
  void print() const { std::cout << (value ? "true" : "false"); }
  std::unique_ptr<ValueNode> clone() const {
    return std::make_unique<BoolValueNode>(value, loc);
  };
};

class VariableValueNode : public ValueNode {
public:
  // Variable
  std::string value;
  // Function
  VariableValueNode(std::string value, Location loc)
      : ValueNode(loc), value(value) {}
  void print() const { std::cout << value; }
  std::unique_ptr<ValueNode> clone() const {
    return std::make_unique<VariableValueNode>(value, loc);
  };
};

class OperationNode {
public:
  // Variable
  Location loc;
  // Function
  OperationNode(Location loc) : loc(loc) {}
  virtual void print() const {}
  virtual std::unique_ptr<OperationNode> clone() const { return nullptr; }
};

class OperationPlusNode : public OperationNode {
public:
  // Function
  OperationPlusNode(Location loc) : OperationNode(loc) {}
  void print() const { std::cout << "+"; }
  std::unique_ptr<OperationNode> clone() const {
    return std::make_unique<OperationPlusNode>(loc);
  };
};

class OperationNoOpNode : public OperationNode {
public:
  // Function
  OperationNoOpNode(Location loc) : OperationNode(loc) {}
  void print() const { std::cout << ""; }
  std::unique_ptr<OperationNode> clone() const {
    return std::make_unique<OperationNoOpNode>(loc);
  };
};

class ExpressionNode {
public:
  // Variable
  bool isValue;
  std::unique_ptr<ValueNode> value;

  std::unique_ptr<ExpressionNode> lhs;
  std::unique_ptr<OperationNode> op;
  std::unique_ptr<ExpressionNode> rhs;

  Location loc;
  ExpressionNode(std::unique_ptr<ValueNode> value, Location loc)
      : value(std::move(value)), op(std::make_unique<OperationNoOpNode>(loc)),
        loc(loc), isValue(true) {}
  ExpressionNode(std::unique_ptr<ExpressionNode> lhs,
                 std::unique_ptr<OperationNode> op,
                 std::unique_ptr<ExpressionNode> rhs, Location loc)
      : lhs(std::move(lhs)), op(std::move(op)), rhs(std::move(rhs)), loc(loc),
        isValue(false) {}

  bool replaceVarValue(
      std::map<std::string, std::unique_ptr<ValueNode>> &callerMap) {
    bool ret = true;
    if (isValue) {
      if (auto *varNode = dynamic_cast<VariableValueNode *>(value.get())) {
        if (callerMap.find(varNode->value) == callerMap.end())
          return false;
        value = callerMap[varNode->value]->clone();
      }
    } else {
      ret &= lhs->replaceVarValue(callerMap);
      ret &= rhs->replaceVarValue(callerMap);
    }
    return ret;
  }

  void print() const {
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
  }
  std::unique_ptr<ExpressionNode> clone() const {
    if (isValue)
      return std::make_unique<ExpressionNode>(value->clone(), loc);
    else
      return std::make_unique<ExpressionNode>(lhs->clone(), op->clone(),
                                              rhs->clone(), loc);
  };

  bool foldValue() {
    if (isValue)
      return true;
    if (!lhs->foldValue() || !lhs->isValue)
      return false;
    if (!rhs->foldValue() || !rhs->isValue)
      return false;
    if (auto plusOp = dynamic_cast<OperationPlusNode *>(op.get()))
      if (auto lhsStr = dynamic_cast<StringValueNode *>(lhs->value.get()))
        if (auto rhsStr = dynamic_cast<StringValueNode *>(rhs->value.get())) {
          value = std::make_unique<StringValueNode>(
              lhsStr->value + rhsStr->value, lhsStr->loc);
          isValue = true;
          lhs = nullptr;
          rhs = nullptr;
          return true;
        }
    return false;
  }
};

class MetadataNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;
  // Function
  MetadataNode(const std::string &key, std::unique_ptr<ExpressionNode> &expNode,
               Location loc)
      : key(key), expNode(std::move(expNode)), loc(loc) {}
  void print(int indent = 0) const;
};

class PositionalArgNode {
public:
  // Variable
  Location loc;
  std::unique_ptr<ExpressionNode> expNode;
  // Function
  PositionalArgNode(std::unique_ptr<ExpressionNode> &expNode, Location loc)
      : expNode(std::move(expNode)), loc(loc) {}
  std::unique_ptr<PositionalArgNode> clone();
};

class NamedArgNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;
  // Function
  NamedArgNode(const std::string &key, std::unique_ptr<ExpressionNode> &expNode,
               Location loc)
      : key(key), expNode(std::move(expNode)), loc(loc) {}
  std::unique_ptr<NamedArgNode> clone();
};

class InstructionNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::vector<std::unique_ptr<PositionalArgNode>> positional_args;
  std::vector<std::unique_ptr<NamedArgNode>> named_args;
  // Function
  InstructionNode(std::string identifier, Location loc)
      : identifier(identifier), loc(loc) {}
  void print(int indent = 0) const;
  std::unique_ptr<InstructionNode> clone();
};

class ActionsNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<InstructionNode>> instructions;
  // Function
  ActionsNode(Location loc) : loc(loc) {}
  void print(int indent = 0) const;
};

class ChecksNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<InstructionNode>> instructions;
  // Function
  ChecksNode(Location loc) : loc(loc) {}
  void print(int indent = 0) const;
};

class TriggersNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<InstructionNode>> instructions;
  // Function
  TriggersNode(Location loc) : loc(loc) {}
  void print(int indent = 0) const;
};

class BlockNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::unique_ptr<ChecksNode> checksNode;
  std::unique_ptr<TriggersNode> triggersNode;
  std::unique_ptr<ActionsNode> actionsNode;
  // Function
  BlockNode(const std::string &id, Location loc) : identifier(id), loc(loc) {}
  void print(int indent = 0) const;
};

class AliasNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::vector<std::string> params;
  std::vector<std::unique_ptr<InstructionNode>> instructions;
  // Function
  AliasNode(const std::string &id, Location loc) : identifier(id), loc(loc) {}
  void print(int indent = 0) const;
  std::unique_ptr<AliasNode> clone();
};

class ModuleNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<BlockNode>> blocks;
  std::map<std::string, std::unique_ptr<AliasNode>> aliases;
  // Function
  ModuleNode(Location loc) : loc(loc) {}
  void print(std::string title, int indent = 0) const;
};

#endif