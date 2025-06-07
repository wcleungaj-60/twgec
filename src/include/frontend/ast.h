#ifndef AST_H
#define AST_H

#include "utils/location.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class ChecksNode {
public:
  Location loc;
  void print(int indent = 0) const;
  ChecksNode(Location loc) : loc(loc) {}
};

class TriggersNode {
public:
  Location loc;
  void print(int indent = 0) const;
  TriggersNode(Location loc) : loc(loc) {}
};

class PositionalArgNode {
public:
  Location loc;
  std::string value;
  PositionalArgNode(const std::string &value, Location loc)
      : value(value), loc(loc) {}
};

class NamedArgNode {
public:
  Location loc;
  std::string key;
  std::string value;
  NamedArgNode(const std::string &key, const std::string &value, Location loc)
      : key(key), value(value), loc(loc) {}
};

class ActionNode {
public:
  Location loc;
  std::vector<std::string> identifier;
  std::vector<std::unique_ptr<PositionalArgNode>> positional_args;
  std::vector<std::unique_ptr<NamedArgNode>> named_args;
  void print(int indent = 0) const;
  ActionNode(Location loc) : loc(loc) {}
};

class ActionsNode {
public:
  Location loc;
  std::vector<std::unique_ptr<ActionNode>> actions;
  void print(int indent = 0) const;
  ActionsNode(Location loc) : loc(loc) {}
};

class BlockNode {
public:
  Location loc;
  std::string identifier;
  std::unique_ptr<ChecksNode> checksNode;
  std::unique_ptr<TriggersNode> triggersNode;
  std::unique_ptr<ActionsNode> actionsNode;

  BlockNode(const std::string &id, Location loc) : identifier(id), loc(loc) {}
  void print(int indent = 0) const;
};

class MetadataNode {
public:
  Location loc;
  std::string key;
  std::string value;
  MetadataNode(const std::string &key, const std::string &value, Location loc)
      : key(key), value(value), loc(loc) {}
  void print(int indent = 0) const;
};

class ModuleNode {
public:
  Location loc;
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<BlockNode>> blocks;
  void print(int indent = 0) const;
  ModuleNode(Location loc) : loc(loc) {}
};

#endif