#include "codegen.h"
#include "ast.h"
#include "metadata.h"
#include "twge_action.h"
#include "utils/utils.h"
#include <fstream>
#include <iostream>

std::string getMetaString(
    std::unordered_map<std::string, std::unique_ptr<MetadataAbstract>> metaMap,
    std::string key) {
  return (static_cast<MetadataString *>(metaMap[key].get()))->value;
}

void CodeGen::codegen(std::string filepath) {
  std::ofstream outputFile(filepath);
  if (!outputFile)
    return;
  codegenModuleNode(outputFile);
  outputFile.close();
}

void CodeGen::codegenMetaData(std::ofstream &of) {
  std::unordered_map<std::string, const std::shared_ptr<ValueNode>> inputMap;
  for (auto &metadata : moduleNode->metadatas)
    inputMap.insert({metadata->key, std::move(metadata->value)});
  of << inden(4) << "\"$schema\": "
     << MetadataLegalizer::defaultMap.get(metadata::schema,
                                          inputMap[metadata::schema])
     << "," << std::endl;
  of << inden(4) << "\"config\": {" << std::endl;
  of << inden(8) << "\"stage\": {" << std::endl;
  of << inden(12) << "\"width\": "
     << MetadataLegalizer::defaultMap.get(metadata::stageWidth,
                                          inputMap[metadata::stageWidth])
     << "," << std::endl;
  of << inden(12) << "\"height\": "
     << MetadataLegalizer::defaultMap.get(metadata::stageHeight,
                                          inputMap[metadata::stageHeight])
     << "," << std::endl;
  of << inden(12) << "\"backgroundColor\": "
     << MetadataLegalizer::defaultMap.get(
            metadata::stageBackgroundColor,
            inputMap[metadata::stageBackgroundColor])
     << "," << std::endl;
  of << inden(12) << "\"resolutionPolicy\": \"showAll\"," << std::endl;
  of << inden(12) << "\"alignHorizontal\": \"center\"," << std::endl;
  of << inden(12) << "\"alignVertical\": \"middle\"" << std::endl;
  of << inden(8) << "}," << std::endl;
  of << inden(8) << "\"preload\": {" << std::endl;
  of << inden(12) << "\"sources\": []," << std::endl;
  of << inden(12) << "\"resourcesExclude\": []" << std::endl;
  of << inden(8) << "}," << std::endl;
  of << inden(8) << "\"configs\": {" << std::endl;
  of << inden(12) << "\"TwilightWarsConfig\": {" << std::endl;
  of << inden(16) << "\"title\": "
     << MetadataLegalizer::defaultMap.get(metadata::title,
                                          inputMap[metadata::title])
     << "," << std::endl;
  of << inden(16) << "\"serverConfig\": {" << std::endl;
  of << inden(20) << "\"minPlayers\": "
     << MetadataLegalizer::defaultMap.get(metadata::minPlayers,
                                          inputMap[metadata::minPlayers])
     << "," << std::endl;
  of << inden(20) << "\"supportSignin\": "
     << MetadataLegalizer::defaultMap.get(metadata::supportSignin,
                                          inputMap[metadata::supportSignin])
     << "," << std::endl;
  of << inden(20) << "\"mustLogin\": "
     << MetadataLegalizer::defaultMap.get(metadata::mustLogin,
                                          inputMap[metadata::mustLogin])
     << "," << std::endl;
  of << inden(20) << "\"allowGuest\": "
     << MetadataLegalizer::defaultMap.get(metadata::allowGuest,
                                          inputMap[metadata::allowGuest])
     << "," << std::endl;
  of << inden(20) << "\"supportMsgServer\": "
     << MetadataLegalizer::defaultMap.get(metadata::supportMsgServer,
                                          inputMap[metadata::supportMsgServer])
     << "," << std::endl;
  of << inden(20) << "\"gamezoneCode\": "
     << MetadataLegalizer::defaultMap.get(metadata::gamezoneCode,
                                          inputMap[metadata::gamezoneCode])
     << "," << std::endl;
  of << inden(20) << "\"roomType\": \"close\"," << std::endl;
  of << inden(20) << "\"roomSize\": "
     << MetadataLegalizer::defaultMap.get(metadata::roomSize,
                                          inputMap[metadata::roomSize])
     << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16) << "\"runGame\": "
     << MetadataLegalizer::defaultMap.get(metadata::runGame,
                                          inputMap[metadata::runGame])
     << "," << std::endl;
  of << inden(16) << "\"gameStartFadein\": \"fadein\"," << std::endl;
  of << inden(16) << "\"lives\": "
     << MetadataLegalizer::defaultMap.get(metadata::lives,
                                          inputMap[metadata::lives])
     << "," << std::endl;
  of << inden(16) << "\"debugCamp\": \"ask\"," << std::endl;
  of << inden(16) << "\"releaseCamp\": \"ask\"," << std::endl;
  of << inden(16) << "\"setInitFocus\": "
     << MetadataLegalizer::defaultMap.get(metadata::setInitFocus,
                                          inputMap[metadata::setInitFocus])
     << "," << std::endl;
  of << inden(16) << "\"initFocus\": {" << std::endl;
  of << inden(20) << "\"x\": \"0\"," << std::endl;
  of << inden(20) << "\"y\": \"0\"" << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16) << "\"campOptions\": {" << std::endl;
  of << inden(20) << "\"campOpSkydow\": "
     << MetadataLegalizer::defaultMap.get(metadata::campOpSkydow,
                                          inputMap[metadata::campOpSkydow])
     << "," << std::endl;
  of << inden(20) << "\"campOpRoyal\": "
     << MetadataLegalizer::defaultMap.get(metadata::campOpRoyal,
                                          inputMap[metadata::campOpRoyal])
     << "," << std::endl;
  of << inden(20) << "\"campOpThird\": "
     << MetadataLegalizer::defaultMap.get(metadata::campOpThird,
                                          inputMap[metadata::campOpThird])
     << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16) << "\"map\": "
     << MetadataLegalizer::defaultMap.get(metadata::map,
                                          inputMap[metadata::map])
     << "," << std::endl;
  of << inden(16) << "\"maxAbilityLevel\": "
     << MetadataLegalizer::defaultMap.get(metadata::maxAbilityLevel,
                                          inputMap[metadata::maxAbilityLevel])
     << "," << std::endl;
  of << inden(16) << "\"nextGameEnabled\": "
     << MetadataLegalizer::defaultMap.get(metadata::nextGameEnabled,
                                          inputMap[metadata::nextGameEnabled])
     << "," << std::endl;
  of << inden(16) << "\"disableNextGameOnMissionComplete\": "
     << MetadataLegalizer::defaultMap.get(
            metadata::disableNextGameOnMissionComplete,
            inputMap[metadata::disableNextGameOnMissionComplete])
     << "," << std::endl;
  of << inden(16) << "\"playDefaultMusic\": "
     << MetadataLegalizer::defaultMap.get(metadata::playDefaultMusic,
                                          inputMap[metadata::playDefaultMusic])
     << "," << std::endl;
  of << inden(16) << "\"cameraAfterOver\": \"restrict\"," << std::endl;
  of << inden(16) << "\"useDefaultItems\": "
     << MetadataLegalizer::defaultMap.get(metadata::useDefaultItems,
                                          inputMap[metadata::useDefaultItems])
     << "," << std::endl;
  of << inden(16) << "\"useDefaultCampLocs\": "
     << MetadataLegalizer::defaultMap.get(
            metadata::useDefaultCampLocs,
            inputMap[metadata::useDefaultCampLocs])
     << "," << std::endl;
  of << inden(16) << "\"skydowLocs\": "
     << MetadataLegalizer::defaultMap.get(metadata::skydowLocs,
                                          inputMap[metadata::skydowLocs])
     << "," << std::endl;
  of << inden(16) << "\"royalLocs\": "
     << MetadataLegalizer::defaultMap.get(metadata::royalLocs,
                                          inputMap[metadata::royalLocs])
     << "," << std::endl;
  of << inden(16) << "\"thirdLocs\": "
     << MetadataLegalizer::defaultMap.get(metadata::thirdLocs,
                                          inputMap[metadata::thirdLocs])
     << "," << std::endl;
  of << inden(16) << "\"useCustomWeapons\": "
     << MetadataLegalizer::defaultMap.get(metadata::useCustomWeapons,
                                          inputMap[metadata::useCustomWeapons])
     << "," << std::endl;
  of << inden(16) << "\"customWeapons\": []," << std::endl;
  of << inden(16) << "\"useCustomItems\": "
     << MetadataLegalizer::defaultMap.get(metadata::useCustomItems,
                                          inputMap[metadata::useCustomItems])
     << "," << std::endl;
  of << inden(16) << "\"customItems\": []," << std::endl;
  of << inden(16) << "\"enabled\": true," << std::endl;
  of << inden(16) << "\"@use\": 0" << std::endl;
  of << inden(12) << "}" << std::endl;
  of << inden(8) << "}" << std::endl;
  of << inden(4) << "}," << std::endl;
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
    int delay = 0;
    int repeat = 0;
    int repeatInterval = 0;
    for (auto &metadata : block->metadatas) {
      if (auto intNode = dynamic_cast<IntValueNode *>(metadata->value.get())) {
        if (metadata->key == "delay")
          delay = intNode->value;
        if (metadata->key == "repeat")
          repeat = intNode->value;
        if (metadata->key == "repeatInterval")
          repeatInterval = intNode->value;
      }
    }
    of << inden(8) << "{" << std::endl;
    of << inden(12) << "\"id\": \"" << block->identifier << "\"," << std::endl;
    of << inden(12) << "\"disabled\": false," << std::endl;
    of << inden(12) << "\"folder\": \"\"," << std::endl;
    of << inden(12) << "\"startTime\": " << delay << "," << std::endl;
    of << inden(12) << "\"checkInterval\": 10," << std::endl;
    of << inden(12) << "\"repeats\": " << repeat << "," << std::endl;
    of << inden(12) << "\"repeatInterval\": " << repeatInterval << ","
       << std::endl;
    of << inden(12) << "\"devOnly\": false," << std::endl;
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
    return twge::action::console(of, action);
  if (action->identifier[0] == "addActor")
    return twge::action::ActionAddActor::addActor(of, action);
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
  codegenMetaData(of);
  codegenBlocks(of);
  of << "}" << std::endl;
}