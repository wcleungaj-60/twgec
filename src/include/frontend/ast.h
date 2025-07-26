#ifndef AST_H
#define AST_H

#include "utils/location.h"
#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <vector>

class ValueNode {
public:
  // Variable
  Location loc;

  // Constructor
  ValueNode(Location loc) : loc(loc) {}

  // Function
  virtual void print() {}
  virtual std::unique_ptr<ValueNode> clone();
};

class ListValueNode : public ValueNode {
public:
  // Variable
  std::vector<std::unique_ptr<ValueNode>> items = {};

  // Constructor
  ListValueNode(Location loc) : ValueNode(loc) {}

  // Function
  void print();
  std::unique_ptr<ValueNode> clone();
};

class PointValueNode : public ValueNode {
public:
  // Variable
  int x;
  int y;

  // Constructor
  PointValueNode(int x, int y, Location loc) : ValueNode(loc), x(x), y(y) {}

  // Function
  void print();
  std::unique_ptr<ValueNode> clone();
};

class StringValueNode : public ValueNode {
public:
  // Variable
  std::string value;

  // Constructor
  StringValueNode(std::string value, Location loc)
      : ValueNode(loc), value(value) {}

  // Function
  void print();
  std::unique_ptr<ValueNode> clone();
};

class IntValueNode : public ValueNode {
public:
  // Variable
  int value;

  // Constructor
  IntValueNode(int value, Location loc) : ValueNode(loc), value(value) {}

  // Function
  void print();
  std::unique_ptr<ValueNode> clone();
};

class BoolValueNode : public ValueNode {
public:
  // Variable
  bool value;

  // Constructor
  BoolValueNode(bool value, Location loc) : ValueNode(loc), value(value) {}

  // Function
  void print();
  std::unique_ptr<ValueNode> clone();
};

class VariableValueNode : public ValueNode {
public:
  // Variable
  std::string value;

  // Constructor
  VariableValueNode(std::string value, Location loc)
      : ValueNode(loc), value(value) {}

  // Function
  void print();
  std::unique_ptr<ValueNode> clone();
};

class OperationNode {
public:
  // Variable
  Location loc;

  // Constructor
  OperationNode(Location loc) : loc(loc) {}

  // Function
  virtual void print() {}
  virtual std::unique_ptr<OperationNode> clone();
};

class OperationPlusNode : public OperationNode {
public:
  // Constructor
  OperationPlusNode(Location loc) : OperationNode(loc) {}

  // Function
  void print();
  std::unique_ptr<OperationNode> clone();
};

class OperationNoOpNode : public OperationNode {
public:
  // Constructor
  OperationNoOpNode(Location loc) : OperationNode(loc) {}

  // Function
  void print();
  std::unique_ptr<OperationNode> clone();
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

  // Constructor
  ExpressionNode(std::unique_ptr<ValueNode> value, Location loc)
      : value(std::move(value)), op(std::make_unique<OperationNoOpNode>(loc)),
        loc(loc), isValue(true) {}
  ExpressionNode(std::unique_ptr<ExpressionNode> lhs,
                 std::unique_ptr<OperationNode> op,
                 std::unique_ptr<ExpressionNode> rhs, Location loc)
      : lhs(std::move(lhs)), op(std::move(op)), rhs(std::move(rhs)), loc(loc),
        isValue(false) {}

  // Function
  void print();
  std::unique_ptr<ExpressionNode> clone();
  bool
  propagateVarToExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
};

class MetadataNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  MetadataNode(const std::string &key, std::unique_ptr<ExpressionNode> &expNode,
               Location loc)
      : key(key), expNode(std::move(expNode)), loc(loc) {}

  // Function
  void print(int indent = 0);
};

class GlobalConstDefNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  GlobalConstDefNode(const std::string &key,
                     std::unique_ptr<ExpressionNode> &expNode, Location loc)
      : key(key), expNode(std::move(expNode)), loc(loc) {}

  // Function
  void print(int indent = 0);
};

class PositionalArgNode {
public:
  // Variable
  Location loc;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  PositionalArgNode(std::unique_ptr<ExpressionNode> &expNode, Location loc)
      : expNode(std::move(expNode)), loc(loc) {}

  // Function
  std::unique_ptr<PositionalArgNode> clone();
};

class NamedArgNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  NamedArgNode(const std::string &key, std::unique_ptr<ExpressionNode> &expNode,
               Location loc)
      : key(key), expNode(std::move(expNode)), loc(loc) {}

  // Function
  std::unique_ptr<NamedArgNode> clone();
};

class InstructionNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::vector<std::unique_ptr<PositionalArgNode>> positional_args;
  std::vector<std::unique_ptr<NamedArgNode>> named_args;

  // Constructor
  InstructionNode(std::string identifier, Location loc)
      : identifier(identifier), loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<InstructionNode> clone();
};

class ActionsNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<InstructionNode>> instructions;

  // Constructor
  ActionsNode(Location loc) : loc(loc) {}

  // Function
  void print(int indent = 0);
};

class ChecksNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<InstructionNode>> instructions;

  // Function
  ChecksNode(Location loc) : loc(loc) {}
  void print(int indent = 0);
};

class TriggersNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<InstructionNode>> instructions;

  // Constructor
  TriggersNode(Location loc) : loc(loc) {}

  // Function
  void print(int indent = 0);
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

  // Constructor
  BlockNode(const std::string &id, Location loc) : identifier(id), loc(loc) {}

  // Function
  void print(int indent = 0);
};

class AliasNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::vector<std::string> params;
  std::vector<std::unique_ptr<InstructionNode>> instructions;

  // Constructor
  AliasNode(const std::string &id, Location loc) : identifier(id), loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<AliasNode> clone();
};

class ModuleNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<BlockNode>> blocks;
  std::map<std::string, std::unique_ptr<AliasNode>> aliases;
  std::vector<std::unique_ptr<GlobalConstDefNode>> constDefs;

  // Constructor
  ModuleNode(Location loc) : loc(loc) {}

  // Function
  void print(std::string title, int indent = 0);
};

#endif