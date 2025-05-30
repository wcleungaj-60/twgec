#include "frontend/ast.h"

void ModuleNode::print(int indent) const {
  std::string indentation(indent, ' ');
  std::cout << indentation << "ModuleNode: "
            << "\n";
  for (auto &metadata : metadatas)
    metadata->print(indent + 4);
  for (auto &block : blocks)
    block->print(indent + 4);
}

void MetadataNode::print(int indent) const {
  std::string indentation(indent, ' ');
  std::cout << indentation << "MetadataNode: " << key << "=" << value << "\n";
}

void BlockNode::print(int indent) const {
  std::string indentation(indent, ' ');
  std::cout << indentation << "BlockNode: " << identifier << "\n";
  if (checksNode)
    checksNode->print(indent + 4);
  if (triggersNode)
    triggersNode->print(indent + 4);
  if (actionsNode)
    actionsNode->print(indent + 4);
}

void ChecksNode::print(int indent) const {
  std::string indentation(indent, ' ');
  std::cout << indentation << "ChecksNode\n";
}

void TriggersNode::print(int indent) const {
  std::string indentation(indent, ' ');
  std::cout << indentation << "TriggersNode\n";
}

void ActionsNode::print(int indent) const {
  std::string indentation(indent, ' ');
  std::cout << indentation << "ActionsNode\n";
}