#include "frontend/ast.h"
#include "utils/utils.h"

void ModuleNode::print(int indent) const {
  std::cout << inden(indent) << "ModuleNode: "
            << "\n";
  for (auto &metadata : metadatas)
    metadata->print(indent + 4);
  for (auto &block : blocks)
    block->print(indent + 4);
}

void MetadataNode::print(int indent) const {
  std::cout << inden(indent) << "MetadataNode: " << key << "=" << value << "\n";
}

void BlockNode::print(int indent) const {
  std::cout << inden(indent) << "BlockNode: " << identifier << "\n";
  if (checksNode)
    checksNode->print(indent + 4);
  if (triggersNode)
    triggersNode->print(indent + 4);
  if (actionsNode)
    actionsNode->print(indent + 4);
}

void ChecksNode::print(int indent) const {
  std::cout << inden(indent) << "ChecksNode\n";
}

void TriggersNode::print(int indent) const {
  std::cout << inden(indent) << "TriggersNode\n";
}

void ActionsNode::print(int indent) const {
  std::cout << inden(indent) << "ActionsNode {\n";
  for (auto &a : actions)
    a.get()->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void ActionNode::print(int indent) const {
  std::cout << inden(indent) << join(identifier, ".") << "("
            << join(positional_args, ",");
  if (!named_args.empty())
    std::cout << "," << join(named_args, ",");
  std::cout << ")\n";
}