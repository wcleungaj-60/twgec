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

class ConstDefNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  ConstDefNode(const std::string &key, std::unique_ptr<ExpressionNode> &expNode,
               Location loc)
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

enum FunDefType {
  FUN_DEF_TYPE_INVALID,
  FUN_DEF_TYPE_ACTIONS,
  FUN_DEF_TYPE_CHECKS,
  FUN_DEF_TYPE_TRIGGERS,
};

inline std::ostream &operator<<(std::ostream &os, FunDefType type) {
  switch (type) {
  case FUN_DEF_TYPE_ACTIONS:
    os << "actions";
    break;
  case FUN_DEF_TYPE_CHECKS:
    os << "checks";
    break;
  case FUN_DEF_TYPE_TRIGGERS:
    os << "triggers";
    break;
  default:
    os << "invalid";
    break;
  }
  return os;
}

class CompositeInstrNode {
public:
  // Variable
  Location loc;
  std::unique_ptr<InstructionNode> instruction;

  // Constructor
  CompositeInstrNode(Location loc, std::unique_ptr<InstructionNode> instruction)
      : loc(loc), instruction(std::move(instruction)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<CompositeInstrNode> clone();
};

class InstrSetNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<CompositeInstrNode>> instructions;

  // Constructor
  InstrSetNode(Location loc) : loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<InstrSetNode> clone();
};

class TypedInstrSetNode {
public:
  // Variable
  Location loc;
  FunDefType type;
  std::unique_ptr<InstrSetNode> instrSet;

  // Constructor
  TypedInstrSetNode(Location loc, FunDefType type = FUN_DEF_TYPE_INVALID)
      : loc(loc), type(type), instrSet(std::make_unique<InstrSetNode>(loc)) {}
  TypedInstrSetNode(Location loc, FunDefType type, std::unique_ptr<InstrSetNode> instrSet)
      : loc(loc), type(type), instrSet(std::move(instrSet)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<TypedInstrSetNode> clone();
};

class FunDefNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::vector<std::string> params;
  std::unique_ptr<TypedInstrSetNode> typedInstrSet;

  // Constructor
  FunDefNode(const std::string &id, Location loc)
      : identifier(id), loc(loc) {}
  FunDefNode(const std::string &id, Location loc,
             std::unique_ptr<TypedInstrSetNode> typedInstrSet)
      : identifier(id), loc(loc), typedInstrSet(std::move(typedInstrSet)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<FunDefNode> clone();
};

class BlockNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<TypedInstrSetNode>> typedInstrSets;

  // Constructor
  BlockNode(const std::string &id, Location loc) : identifier(id), loc(loc) {}

  // Function
  void print(int indent = 0);
};

class ModuleNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<BlockNode>> blocks;
  std::map<std::string, std::unique_ptr<FunDefNode>> funDefs;
  std::vector<std::unique_ptr<ConstDefNode>> constDefs;

  // Constructor
  ModuleNode(Location loc) : loc(loc) {}

  // Function
  void print(std::string title, int indent = 0);
};

#endif