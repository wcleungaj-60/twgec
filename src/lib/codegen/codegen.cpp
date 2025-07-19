#include "codegen.h"
#include "action.h"
#include "ast.h"
#include "check.h"
#include "metadata.h"
#include "trigger.h"
#include "utils/utils.h"
#include <fstream>
#include <iostream>

#define CODEGEN_ACTION(inputName, outputClass)                                 \
  if (action->identifier == inputName)                                         \
  return action::Action##outputClass::method(of, action)

#define CODEGEN_CHECK(inputName, outputClass)                                  \
  if (check->identifier == inputName)                                          \
  return check::Check##outputClass::method(of, check)

#define CODEGEN_TRIGGER(inputName, outputClass)                                \
  if (trigger->identifier == inputName)                                        \
  return trigger::Trigger##outputClass::method(of, trigger)

using namespace codegen;

void CodeGenerator::codegen(std::string filepath) {
  std::ofstream outputFile(filepath);
  if (!outputFile)
    return;
  codegenModuleNode(outputFile);
  outputFile.close();
}

void CodeGenerator::codegenModuleNode(std::ofstream &of) {
  of << "{" << std::endl;
  metadata::ConfigSetup::setup(of, moduleNode->metadatas);
  codegenBlocks(of);
  of << "}" << std::endl;
}

void CodeGenerator::codegenBlocks(std::ofstream &of) {
  of << inden(4) << "\"events\": [";
  if (moduleNode->blocks.size() == 0) {
    of << "]" << std::endl;
    return;
  }
  of << std::endl;
  for (auto i = 0; i < moduleNode->blocks.size(); i++) {
    auto &block = moduleNode->blocks[i];
    of << inden(8) << "{" << std::endl;
    of << inden(12) << "\"id\": \"" << block->identifier << "\"," << std::endl;
    metadata::BlockSetup::setup(of, block->metadatas);
    codegenActions(of, block->actionsNode);
    codegenChecks(of, block->checksNode);
    codegenTriggers(of, block->triggersNode);
    of << inden(8) << "}";
    if (i != moduleNode->blocks.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(4) << "]" << std::endl;
}

void CodeGenerator::codegenActions(std::ofstream &of,
                                   std::unique_ptr<ActionsNode> &actions) {
  of << inden(12) << "\"actions\": [";
  if (!actions.get() || actions->instructions.empty()) {
    of << "]," << std::endl;
    return;
  }
  of << std::endl;
  for (auto i = 0; i < actions->instructions.size(); i++) {
    codegenAction(of, actions->instructions[i]);
    if (i != actions->instructions.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(12) << "]," << std::endl;
}

void CodeGenerator::codegenChecks(std::ofstream &of,
                                  std::unique_ptr<ChecksNode> &checks) {
  of << inden(12) << "\"checks\": [";
  if (!checks.get() || checks->instructions.empty()) {
    of << "]," << std::endl;
    return;
  }
  of << std::endl;
  for (auto i = 0; i < checks->instructions.size(); i++) {
    codegenCheck(of, checks->instructions[i]);
    if (i != checks->instructions.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(12) << "]," << std::endl;
}

void CodeGenerator::codegenTriggers(std::ofstream &of,
                                    std::unique_ptr<TriggersNode> &triggers) {
  of << inden(12) << "\"triggers\": [";
  if (!triggers.get() || triggers->instructions.empty()) {
    of << "]" << std::endl;
    return;
  }
  of << std::endl;
  for (auto i = 0; i < triggers->instructions.size(); i++) {
    codegenTrigger(of, triggers->instructions[i]);
    if (i != triggers->instructions.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(12) << "]" << std::endl;
}

void CodeGenerator::codegenAction(std::ofstream &of,
                                  std::unique_ptr<InstructionNode> &action) {
  CODEGEN_ACTION("actorDisappear", ActorDisappear);
  CODEGEN_ACTION("actorTalk", ActorTalk);
  CODEGEN_ACTION("addActor", AddActor);
  CODEGEN_ACTION("addMapSign", AddMapSign);
  CODEGEN_ACTION("addStuff", AddStuff);
  CODEGEN_ACTION("deltaHp", DeltaHp);
  CODEGEN_ACTION("enblastEffect", EnblastEffect);
  CODEGEN_ACTION("longBo", LongBo);
  CODEGEN_ACTION("print", Console);
  CODEGEN_ACTION("setGlobal", SetGlobal);
  CODEGEN_ACTION("setObjectVar", SetObjectVar);
  CODEGEN_ACTION("wait", Wait);
  std::cerr << "Codegen error: Cannot found the corresponding action name \""
            << action->identifier << "\" at " << action.get()->loc << "\n";
}

void CodeGenerator::codegenCheck(std::ofstream &of,
                                 std::unique_ptr<InstructionNode> &check) {
  CODEGEN_CHECK("checkString", String);
  std::cerr << "Codegen error: Cannot found the corresponding check name \""
            << check->identifier << "\" at " << check.get()->loc << "\n";
}

void CodeGenerator::codegenTrigger(std::ofstream &of,
                                   std::unique_ptr<InstructionNode> &trigger) {
  CODEGEN_TRIGGER("actorDead", ActorDead);
  CODEGEN_TRIGGER("actorFire", ActorFire);
  CODEGEN_TRIGGER("clickButton", ClickButton);
  CODEGEN_TRIGGER("releasePower", ReleasePower);
  std::cerr << "Codegen error: Cannot found the corresponding trigger name \""
            << trigger->identifier << "\" at " << trigger.get()->loc << "\n";
}
