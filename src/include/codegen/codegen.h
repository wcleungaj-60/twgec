#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <string>

#define CODEGEN_ACTION(inputName, outputClass)                                 \
  if (action->identifier == inputName)                                         \
  return action::Action##outputClass::method(of, action->paramApps)

#define CODEGEN_CHECK(inputName, outputClass)                                  \
  if (check->identifier == inputName)                                          \
  return check::Check##outputClass::method(of, check->paramApps)

#define CODEGEN_TRIGGER(inputName, outputClass)                                \
  if (trigger->identifier == inputName)                                        \
  return trigger::Trigger##outputClass::method(of, trigger->paramApps)

#define CODEGEN_TRIGGER_EXTRA_ARG(inputName, outputClass, ...)                 \
  if (trigger->identifier == inputName)                                        \
  return trigger::Trigger##outputClass::method(of, trigger->paramApps,         \
                                               __VA_ARGS__)

namespace codegen {
class CodeGenerator {
private:
  // Variable
  const std::unique_ptr<ModuleNode> &moduleNode;
  std::map<std::string, std::string> customWeaponsKeywordEnum;
  // Functions
  void preCodegen();
  void codegenModuleNode(std::ofstream &of);
  void codegenBlocks(std::ofstream &of);
  void codegenAction(std::ofstream &of, std::unique_ptr<InstructionNode> &);
  void codegenTypedInstrSet(std::ofstream &of,
                            std::unique_ptr<TypedInstrSetNode> &);
  void codegenCheck(std::ofstream &of, std::unique_ptr<InstructionNode> &);
  void codegenTrigger(std::ofstream &of, std::unique_ptr<InstructionNode> &);

public:
  // Constructor
  CodeGenerator(const std::unique_ptr<ModuleNode> &moduleNode)
      : moduleNode(moduleNode){};
  // Functions
  void codegen(std::string filepath);
};
} // namespace codegen
#endif