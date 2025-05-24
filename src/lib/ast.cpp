#include "ast.h"

void BlockNode::print(int indent = 0) const
{
    std::string indentation(indent, ' ');
    std::cout << indentation << "BlockNode: " << identifier << "\n";
    if (checksNode)
    {
        std::cout << indentation << "  ChecksNode:\n";
        checksNode->print(indent + 4);
    }
    if (triggersNode)
    {
        std::cout << indentation << "  TriggersNode:\n";
        triggersNode->print(indent + 4);
    }
    if (actionsNode)
    {
        std::cout << indentation << "  ActionsNode:\n";
        actionsNode->print(indent + 4);
    }
}

void ChecksNode::print(int indent = 0) const
{
    std::string indentation(indent, ' ');
    std::cout << indentation << "ChecksNode\n";
}

void TriggersNode::print(int indent = 0) const
{
    std::string indentation(indent, ' ');
    std::cout << indentation << "TriggersNode\n";
}

void ActionsNode::print(int indent = 0) const
{
    std::string indentation(indent, ' ');
    std::cout << indentation << "ActionsNode\n";
}