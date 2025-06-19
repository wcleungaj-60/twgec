#include "codegen.h"
#include "ast.h"
#include "metadata.h"
#include "twge_action.h"
#include "utils/utils.h"
#include <fstream>
#include <iostream>

void CodeGen::codegen(std::string filepath) {
  std::ofstream outputFile(filepath);
  if (!outputFile)
    return;
  codegenModuleNode(outputFile);
  outputFile.close();
}

void CodeGen::codegenBlocks(std::ofstream &of) {
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
    codegen::metadata::BlockSetup::setup(of, block->metadatas);
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

void CodeGen::codegenActions(std::ofstream &of,
                             std::unique_ptr<ActionsNode> &actions) {
  of << inden(12) << "\"actions\": [";
  if (actions->actions.empty()) {
    of << "]," << std::endl;
    return;
  }
  of << std::endl;
  for (auto i = 0; i < actions->actions.size(); i++) {
    of << inden(16) << "{" << std::endl;
    codegenAction(of, actions->actions[i]);
    of << inden(16) << "}";
    if (i != actions->actions.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(12) << "]," << std::endl;
}

void CodeGen::codegenAction(std::ofstream &of,
                            std::unique_ptr<ActionNode> &action) {
  if (action->identifier[0] == "console")
    return codegen::action::console(of, action);
  if (action->identifier[0] == "addActor")
    return codegen::action::ActionAddActor::addActor(of, action);
  std::cerr << "Codegen error at " << action.get()->loc << ": ";
  action->print();
}

void CodeGen::codegenChecks(std::ofstream &of,
                            std::unique_ptr<ChecksNode> &checks) {
  of << inden(12) << "\"checks\": []," << std::endl;
}

void CodeGen::codegenTriggers(std::ofstream &of,
                              std::unique_ptr<TriggersNode> &triggers) {
  of << inden(12) << "\"triggers\": []" << std::endl;
}

void CodeGen::codegenModuleNode(std::ofstream &of) {
  of << "{" << std::endl;
  codegen::metadata::ConfigSetup::setup(of, moduleNode->metadatas);
  codegenBlocks(of);
  of << "}" << std::endl;
}