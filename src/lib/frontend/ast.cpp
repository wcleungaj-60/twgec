#include "frontend/ast.h"
#include "utils/utils.h"

void ModuleNode::print(int indent) const {
  std::cout << inden(indent) << "ModuleNode: "
            << "\n";
  for (auto &metadata : metadatas)
    metadata->print(indent + 4);
  for (auto &alias : aliases)
    alias.second->print(indent + 4);
  for (auto &block : blocks)
    block->print(indent + 4);
}

void MetadataNode::print(int indent) const {
  std::cout << inden(indent) << "MetadataNode: " << key << "=";
  value->print();
  std::cout << "\n";
}

void BlockNode::print(int indent) const {
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

void ChecksNode::print(int indent) const {
  std::cout << inden(indent) << "ChecksNode\n";
}

void TriggersNode::print(int indent) const {
  std::cout << inden(indent) << "TriggersNode\n";
}

void ActionsNode::print(int indent) const {
  std::cout << inden(indent) << "ActionsNode {\n";
  for (auto &ins : instructions)
    ins.get()->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void AliasNode::print(int indent) const {
  std::cout << inden(indent) << "alias " << identifier << "(";
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

void InstructionNode::print(int indent) const {
  std::cout << inden(indent) << join(identifier, ".") << "(";
  for (int i = 0; i < positional_args.size(); i++) {
    positional_args[i]->valueNode->print();
    if (i != positional_args.size() - 1 || !named_args.empty())
      std::cout << ", ";
  }
  for (int i = 0; i < named_args.size(); i++) {
    std::cout << named_args[i].get()->key << " = ";
    named_args[i]->valueNode->print();
    if (i != named_args.size() - 1)
      std::cout << ", ";
  }
  std::cout << ")\n";
}

std::unique_ptr<NamedArgNode> NamedArgNode::clone() {
  auto newValue = std::move(valueNode.get()->clone());
  return std::make_unique<NamedArgNode>(key, newValue, loc);
}

std::unique_ptr<PositionalArgNode> PositionalArgNode::clone() {
  auto newValue = std::move(valueNode.get()->clone());
  return std::make_unique<PositionalArgNode>(newValue, loc);
}

std::unique_ptr<InstructionNode> InstructionNode::clone() {
  auto newNode = std::make_unique<InstructionNode>(loc);
  newNode.get()->identifier = identifier;
  for (auto &namedArg : named_args)
    newNode.get()->named_args.push_back(namedArg.get()->clone());
  for (auto &positionalArg : positional_args)
    newNode.get()->positional_args.push_back(positionalArg.get()->clone());
  return newNode;
}