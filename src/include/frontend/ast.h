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

class ActionNode {
public:
  std::vector<std::string> identifier;
  std::vector<std::string> args;
public:
  void print(int indent = 0) const;
};

class ActionsNode {
public:
  std::vector<std::unique_ptr<ActionNode>> actions;
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

class MetadataNode {
public:
  std::string key;
  std::string value;
  MetadataNode(const std::string &key, const std::string &value)
      : key(key), value(value) {}
  void print(int indent = 0) const;
};

class ModuleNode {
public:
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<BlockNode>> blocks;
  void print(int indent = 0) const;
};

#endif