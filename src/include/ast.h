#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

class BlockNode : public ASTNode {
public:
    std::string identifier;
    std::unique_ptr<ASTNode> checksNode;
    std::unique_ptr<ASTNode> triggersNode;
    std::unique_ptr<ASTNode> actionsNode;

    BlockNode(const std::string& id) : identifier(id) {}
    void print(int indent = 0) const override;
};

class ChecksNode : public ASTNode {
public:
    void print(int indent = 0) const override;
};

class TriggersNode : public ASTNode {
public:
    void print(int indent = 0) const override;
};

class ActionsNode : public ASTNode {
public:
    void print(int indent = 0) const override;
};

#endif