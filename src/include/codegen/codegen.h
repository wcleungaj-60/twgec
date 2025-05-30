#ifndef CODEGEN_H
#define CODEGEN_H

#include <iostream>
#include <fstream>
#include "ast.h"

class CodeGen {
private:
    const std::unique_ptr<ModuleNode>& moduleNode;
    std::string codegenModuleNode();
public:
    CodeGen(const std::unique_ptr<ModuleNode>& moduleNode): moduleNode(moduleNode) {};
    void codegen(std::string filepath);
};

#endif