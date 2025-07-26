#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <string>

namespace codegen {
class CodeGenerator {
private:
  const std::unique_ptr<ModuleNode> &moduleNode;
  void codegenModuleNode(std::ofstream &of);
  void codegenBlocks(std::ofstream &of);
  void codegenAction(std::ofstream &of, std::unique_ptr<InstructionNode> &);
  void codegenTypedInstrSet(std::ofstream &of, std::unique_ptr<TypedInstrSetNode> &);
  void codegenCheck(std::ofstream &of, std::unique_ptr<InstructionNode> &);
  void codegenTrigger(std::ofstream &of, std::unique_ptr<InstructionNode> &);

public:
  CodeGenerator(const std::unique_ptr<ModuleNode> &moduleNode)
      : moduleNode(moduleNode){};
  void codegen(std::string filepath);
};
} // namespace codegen
#endif