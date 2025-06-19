#ifndef CODEGEN_H
#define CODEGEN_H

#include "metadata.h"

class CodeGen {
private:
  const std::unique_ptr<ModuleNode> &moduleNode;
  void codegenModuleNode(std::ofstream &of);
  void codegenBlocks(std::ofstream &of);
  void codegenAction(std::ofstream &of, std::unique_ptr<ActionNode> &);
  void codegenActions(std::ofstream &of, std::unique_ptr<ActionsNode> &);
  void codegenChecks(std::ofstream &of, std::unique_ptr<ChecksNode> &);
  void codegenTriggers(std::ofstream &of, std::unique_ptr<TriggersNode> &);

public:
  CodeGen(const std::unique_ptr<ModuleNode> &moduleNode)
      : moduleNode(moduleNode){};
  void codegen(std::string filepath);
};

#endif