#include "frontend/ast.h"

void ModuleNode::print(int indent) const {
  std::string indentation(indent, ' ');
  std::cout << indentation << "ModuleNode: "
            << "\n";
  for (auto &node : nodes)
    node->print(indent + 4);
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