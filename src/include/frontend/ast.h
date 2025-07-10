#ifndef AST_H
#define AST_H

#include "utils/location.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class ValueNode {
public:
  // Variable
  Location loc;
  // Function
  ValueNode(Location loc) : loc(loc) {}
  virtual void print() const {}
  virtual std::unique_ptr<ValueNode> clone() const { return nullptr; }
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

class MetadataNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ValueNode> value;
  // Function
  MetadataNode(const std::string &key, std::unique_ptr<ValueNode> &value,
               Location loc)
      : key(key), value(std::move(value)), loc(loc) {}
  void print(int indent = 0) const;
};

class ChecksNode {
public:
  // Variable
  Location loc;
  // Function
  ChecksNode(Location loc) : loc(loc) {}
  void print(int indent = 0) const;
};

class TriggersNode {
public:
  // Variable
  Location loc;
  // Function
  TriggersNode(Location loc) : loc(loc) {}
  void print(int indent = 0) const;
};

class PositionalArgNode {
public:
  // Variable
  Location loc;
  std::unique_ptr<ValueNode> valueNode;
  // Function
  PositionalArgNode(std::unique_ptr<ValueNode> &valueNode, Location loc)
      : valueNode(std::move(valueNode)), loc(loc) {}
  std::unique_ptr<PositionalArgNode> clone();
};

class NamedArgNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ValueNode> valueNode;
  // Function
  NamedArgNode(const std::string &key, std::unique_ptr<ValueNode> &valueNode,
               Location loc)
      : key(key), valueNode(std::move(valueNode)), loc(loc) {}
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
};

class ModuleNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<BlockNode>> blocks;
  std::unordered_map<std::string, std::unique_ptr<AliasNode>> aliases;
  // Function
  ModuleNode(Location loc) : loc(loc) {}
  void print(int indent = 0) const;
};

static std::unique_ptr<ValueNode>
valueNodeClone(std::unique_ptr<ValueNode> &valueNode) {
  if (auto *stringNode = dynamic_cast<StringValueNode *>(valueNode.get()))
    return stringNode->clone();
  if (auto *intNode = dynamic_cast<IntValueNode *>(valueNode.get()))
    return intNode->clone();
  if (auto *boolNode = dynamic_cast<BoolValueNode *>(valueNode.get()))
    return boolNode->clone();
  if (auto *listNode = dynamic_cast<ListValueNode *>(valueNode.get()))
    return listNode->clone();
  return nullptr;
}

#endif