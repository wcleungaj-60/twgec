#include "codegen.h"
#include "ast.h"
#include "instruction/action.h"
#include "instruction/check.h"
#include "instruction/metadata.h"
#include "instruction/trigger.h"
#include "utils/utils.h"
#include <fstream>
#include <iostream>

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
    metadata::BlockSetup::setup(of, block->blockBody->metadatas);
    codegenTypedInstrSet(of, block->blockBody->getActions());
    codegenTypedInstrSet(of, block->blockBody->getChecks());
    codegenTypedInstrSet(of, block->blockBody->getTriggers());
    of << inden(8) << "}";
    if (i != moduleNode->blocks.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(4) << "]" << std::endl;
}

void CodeGenerator::codegenTypedInstrSet(
    std::ofstream &of, std::unique_ptr<TypedInstrSetNode> &typedInstrSet) {
  std::string endingStr =
      typedInstrSet->type == FUN_DEF_TYPE_TRIGGERS ? "]\n" : "],\n";
  auto &instructions = typedInstrSet->instrSet->instructions;
  of << inden(12) << "\"" << typedInstrSet->type << "\": [";
  if (instructions.empty()) {
    of << endingStr;
    return;
  }
  of << std::endl;
  for (auto i = 0; i < instructions.size(); i++) {
    switch (typedInstrSet->type) {
    case FUN_DEF_TYPE_ACTIONS:
      codegenAction(of, instructions[i]->instruction);
      break;
    case FUN_DEF_TYPE_CHECKS:
      codegenCheck(of, instructions[i]->instruction);
      break;
    case FUN_DEF_TYPE_TRIGGERS:
      codegenTrigger(of, instructions[i]->instruction);
      break;
    default:
      break;
    }
    if (i != instructions.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(12) << endingStr;
}

void CodeGenerator::codegenAction(std::ofstream &of,
                                  std::unique_ptr<InstructionNode> &action) {
  CODEGEN_ACTION("actorDisappear", ActorDisappear);
  CODEGEN_ACTION("actorFollow", ActorFollow);
  CODEGEN_ACTION("actorTalk", ActorTalk);
  CODEGEN_ACTION("addActor", AddActor);
  CODEGEN_ACTION("addMapSign", AddMapSign);
  CODEGEN_ACTION("addStuff", AddStuff);
  CODEGEN_ACTION("deltaHp", DeltaHp);
  CODEGEN_ACTION("enblastEffect", EnblastEffect);
  CODEGEN_ACTION("missionComplete", MissionComplete);
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
  CODEGEN_CHECK("checkNumber", Number);
  std::cerr << "Codegen error: Cannot found the corresponding check name \""
            << check->identifier << "\" at " << check.get()->loc << "\n";
}

void CodeGenerator::codegenTrigger(std::ofstream &of,
                                   std::unique_ptr<InstructionNode> &trigger) {
  CODEGEN_TRIGGER("actorDead", ActorDead);
  CODEGEN_TRIGGER("actorFire", ActorFire);
  CODEGEN_TRIGGER("clickButton", ClickButton);
  CODEGEN_TRIGGER("keyboardPressed", KeyboardPressed);
  CODEGEN_TRIGGER("releasePower", ReleasePower);
  std::cerr << "Codegen error: Cannot found the corresponding trigger name \""
            << trigger->identifier << "\" at " << trigger.get()->loc << "\n";
}
