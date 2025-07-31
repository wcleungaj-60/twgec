#ifndef AST_H
#define AST_H

#include "utils/location.h"
#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Global level
class ModuleNode;
class MetadataNode;
class FunDefNode;
class ConstDefNode;
class BlockNode;
// Instruction Set level
class TypedInstrSetNode;
class InstrSetNode;
class CompositeInstrNode;
class BranchNode;
// Instruction level
class InstructionNode;
class NamedArgNode;
class PositionalArgNode;
// Expression level
class ExpressionNode;
class ValueNode;
class BoolValueNode;
class IntValueNode;
class ListValueNode;
class PointValueNode;
class StringValueNode;
class VariableValueNode;

//------------ Enumeration definition ------------//
enum FunDefType {
  FUN_DEF_TYPE_INVALID,
  FUN_DEF_TYPE_ACTIONS,
  FUN_DEF_TYPE_CHECKS,
  FUN_DEF_TYPE_TRIGGERS,
};

enum ExpOpType {
  EXP_OP_TYPE_VOID,
  EXP_OP_TYPE_AND,
  EXP_OP_TYPE_OR,
  EXP_OP_TYPE_EQUAL,
  EXP_OP_TYPE_NOT_EQUAL,
  EXP_OP_TYPE_LESS_THAN,
  EXP_OP_TYPE_LESS_THAN_EQUAL,
  EXP_OP_TYPE_GREATER_THAN,
  EXP_OP_TYPE_GREATER_THAN_EQUAL,
  EXP_OP_TYPE_ADD,
  EXP_OP_TYPE_SUB,
  EXP_OP_TYPE_MUL,
  EXP_OP_TYPE_DIV,
  EXP_OP_TYPE_MOD,
};

//------------ Global level definition ------------//
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

class FunDefNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::vector<std::string> params;
  std::unique_ptr<TypedInstrSetNode> typedInstrSet;

  // Constructor
  FunDefNode(const std::string &id, Location loc) : identifier(id), loc(loc) {}
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
  void setActionsIdx(int idx) { actionsIdx = idx; };
  void setChecksIdx(int idx) { checksIdx = idx; };
  void setTriggersIdx(int idx) { triggersIdx = idx; };
  std::unique_ptr<TypedInstrSetNode> &getActions() {
    return typedInstrSets[actionsIdx];
  };
  std::unique_ptr<TypedInstrSetNode> &getChecks() {
    return typedInstrSets[checksIdx];
  };
  std::unique_ptr<TypedInstrSetNode> &getTriggers() {
    return typedInstrSets[triggersIdx];
  };

private:
  int actionsIdx = -1;
  int checksIdx = -1;
  int triggersIdx = -1;
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

//------------ Instruction Set level definition ------------//
class TypedInstrSetNode {
public:
  // Variable
  Location loc;
  FunDefType type;
  std::unique_ptr<InstrSetNode> instrSet;

  // Constructor
  TypedInstrSetNode(Location loc, FunDefType type = FUN_DEF_TYPE_INVALID)
      : loc(loc), type(type), instrSet(std::make_unique<InstrSetNode>(loc)) {}
  TypedInstrSetNode(Location loc, FunDefType type,
                    std::unique_ptr<InstrSetNode> instrSet)
      : loc(loc), type(type), instrSet(std::move(instrSet)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<TypedInstrSetNode> clone();
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
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue();
};

class CompositeInstrNode {
public:
  // Variable
  Location loc;
  std::unique_ptr<InstructionNode> instruction;
  std::unique_ptr<BranchNode> ifStatement;

  // Constructor
  CompositeInstrNode(Location loc) : loc(loc) {}
  CompositeInstrNode(Location loc, std::unique_ptr<InstructionNode> instruction)
      : loc(loc), instruction(std::move(instruction)) {}
  CompositeInstrNode(Location loc, std::unique_ptr<BranchNode> ifStatement)
      : loc(loc), ifStatement(std::move(ifStatement)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<CompositeInstrNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue();
};

class BranchNode {
public:
  // Variable
  Location loc;
  std::unique_ptr<ExpressionNode> condition;
  std::unique_ptr<InstrSetNode> trueBlock;

  // Constructor
  BranchNode(std::unique_ptr<ExpressionNode> condition,
             std::unique_ptr<InstrSetNode> trueBlock, Location loc)
      : condition(std::move(condition)), trueBlock(std::move(trueBlock)),
        loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<BranchNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue();
};

//------------ Instruction level definition ------------//
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
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue();
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
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue();
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
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue();
};

//------------ Expression level definition ------------//
class ValueNode {
public:
  // Variable
  Location loc;

  // Constructor
  ValueNode(Location loc) : loc(loc) {}

  // Function
  virtual std::unique_ptr<ValueNode> clone();
};

class BoolValueNode : public ValueNode {
public:
  // Variable
  bool value;

  // Constructor
  BoolValueNode(bool value, Location loc) : ValueNode(loc), value(value) {}

  // Function
  std::unique_ptr<ValueNode> clone();
};

class IntValueNode : public ValueNode {
public:
  // Variable
  int value;

  // Constructor
  IntValueNode(int value, Location loc) : ValueNode(loc), value(value) {}

  // Function
  std::unique_ptr<ValueNode> clone();
};

class ListValueNode : public ValueNode {
public:
  // Variable
  std::vector<std::unique_ptr<ValueNode>> items = {};

  // Constructor
  ListValueNode(Location loc) : ValueNode(loc) {}

  // Function
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
  std::unique_ptr<ValueNode> clone();
};

class ExpressionNode {
public:
  // Variable
  bool isValue;
  std::unique_ptr<ValueNode> value;
  std::unique_ptr<ExpressionNode> lhs;
  ExpOpType op;
  std::unique_ptr<ExpressionNode> rhs;
  Location loc;

  // Constructor
  ExpressionNode(std::unique_ptr<ValueNode> value, Location loc)
      : value(std::move(value)), op(EXP_OP_TYPE_VOID), loc(loc), isValue(true) {
  }
  ExpressionNode(std::unique_ptr<ExpressionNode> lhs, ExpOpType op,
                 std::unique_ptr<ExpressionNode> rhs, Location loc)
      : lhs(std::move(lhs)), op(op), rhs(std::move(rhs)), loc(loc),
        isValue(false) {}

  // Function
  std::unique_ptr<ExpressionNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue();
};

//------------ operator<< ------------//
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

inline std::ostream &operator<<(std::ostream &os, ExpOpType type) {
  switch (type) {
  case EXP_OP_TYPE_AND:
    os << "&&";
    break;
  case EXP_OP_TYPE_OR:
    os << "||";
    break;
  case EXP_OP_TYPE_EQUAL:
    os << "==";
    break;
  case EXP_OP_TYPE_NOT_EQUAL:
    os << "!=";
    break;
  case EXP_OP_TYPE_LESS_THAN:
    os << "<";
    break;
  case EXP_OP_TYPE_LESS_THAN_EQUAL:
    os << "<=";
    break;
  case EXP_OP_TYPE_GREATER_THAN:
    os << ">";
    break;
  case EXP_OP_TYPE_GREATER_THAN_EQUAL:
    os << ">=";
    break;
  case EXP_OP_TYPE_ADD:
    os << "+";
    break;
  case EXP_OP_TYPE_SUB:
    os << "-";
    break;
  case EXP_OP_TYPE_MUL:
    os << "*";
    break;
  case EXP_OP_TYPE_DIV:
    os << "/";
    break;
  case EXP_OP_TYPE_MOD:
    os << "%";
    break;
  default:
    os << "";
    break;
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os,
                                std::unique_ptr<ValueNode> valueNode) {
  if (auto stringNode = dynamic_cast<StringValueNode *>(valueNode.get())) {
    os << "\"" << stringNode->value << "\"";
  } else if (auto intNode = dynamic_cast<IntValueNode *>(valueNode.get())) {
    os << intNode->value;
  } else if (auto boolNode = dynamic_cast<BoolValueNode *>(valueNode.get())) {
    os << (boolNode->value ? "true" : "false");
  } else if (auto varNode = dynamic_cast<VariableValueNode *>(valueNode.get())) {
    os << varNode->value;
  } else if (auto ptNode = dynamic_cast<PointValueNode *>(valueNode.get())) {
    os << "(" << ptNode->x << "," << ptNode->y << ")";
  } else if (auto listNode = dynamic_cast<ListValueNode *>(valueNode.get())) {
    os << "[";
    for (auto i = 0; i < listNode->items.size(); i++) {
      os << listNode->items[i]->clone();
      if (i != listNode->items.size() - 1)
        os << ",";
    }
    os << "]";
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os,
                                const ExpressionNode &expNode) {
  if (expNode.isValue) {
    os << expNode.value->clone();
  } else {
    os << "(" << *expNode.lhs.get() << " " << expNode.op << " "
       << *expNode.rhs.get() << ")";
  }
  return os;
}

#endif