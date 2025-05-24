#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class ChecksNode {
public:
  void print(int indent = 0) const;
};

class TriggersNode {
public:
  void print(int indent = 0) const;
};

class ActionsNode {
public:
  void print(int indent = 0) const;
};

class BlockNode {
public:
  std::string identifier;
  std::unique_ptr<ChecksNode> checksNode;
  std::unique_ptr<TriggersNode> triggersNode;
  std::unique_ptr<ActionsNode> actionsNode;

  BlockNode(const std::string &id) : identifier(id) {}
  void print(int indent = 0) const;
};

class ModuleNode {
public:
  std::vector<std::unique_ptr<BlockNode>> nodes;
  void print(int indent = 0) const;
};

#endif