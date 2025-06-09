#include "codegen.h"
#include "twge_action.h"
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
  of << inden(4)
     << "\"$schema\": " << metadataLegalizer.getStr(metadata::schema) << ","
     << std::endl;
  of << inden(4) << "\"config\": {" << std::endl;
  of << inden(8) << "\"stage\": {" << std::endl;
  of << inden(12)
     << "\"width\": " << metadataLegalizer.getInt(metadata::stageWidth) << ","
     << std::endl;
  of << inden(12)
     << "\"height\": " << metadataLegalizer.getInt(metadata::stageHeight) << ","
     << std::endl;
  of << inden(12) << "\"backgroundColor\": "
     << metadataLegalizer.getStr(metadata::stageBackgroundColor) << ","
     << std::endl;
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
  of << inden(16) << "\"title\": " << metadataLegalizer.getStr(metadata::title)
     << "," << std::endl;
  of << inden(16) << "\"serverConfig\": {" << std::endl;
  of << inden(20)
     << "\"minPlayers\": " << metadataLegalizer.getInt(metadata::minPlayers)
     << "," << std::endl;
  of << inden(20) << "\"supportSignin\": "
     << metadataLegalizer.getBool(metadata::supportSignin) << "," << std::endl;
  of << inden(20)
     << "\"mustLogin\": " << metadataLegalizer.getBool(metadata::mustLogin)
     << "," << std::endl;
  of << inden(20)
     << "\"allowGuest\": " << metadataLegalizer.getBool(metadata::allowGuest)
     << "," << std::endl;
  of << inden(20) << "\"supportMsgServer\": "
     << metadataLegalizer.getBool(metadata::supportMsgServer) << ","
     << std::endl;
  of << inden(20)
     << "\"gamezoneCode\": " << metadataLegalizer.getStr(metadata::gamezoneCode)
     << "," << std::endl;
  of << inden(20) << "\"roomType\": \"close\"," << std::endl;
  of << inden(20)
     << "\"roomSize\": " << metadataLegalizer.getInt(metadata::roomSize)
     << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16)
     << "\"runGame\": " << metadataLegalizer.getBool(metadata::runGame) << ","
     << std::endl;
  of << inden(16) << "\"gameStartFadein\": \"fadein\"," << std::endl;
  of << inden(16) << "\"lives\": " << metadataLegalizer.getInt(metadata::lives)
     << "," << std::endl;
  of << inden(16) << "\"debugCamp\": \"ask\"," << std::endl;
  of << inden(16) << "\"releaseCamp\": \"ask\"," << std::endl;
  of << inden(16) << "\"setInitFocus\": "
     << metadataLegalizer.getBool(metadata::setInitFocus) << "," << std::endl;
  of << inden(16) << "\"initFocus\": {" << std::endl;
  of << inden(20) << "\"x\": \"0\"," << std::endl;
  of << inden(20) << "\"y\": \"0\"" << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16) << "\"campOptions\": {" << std::endl;
  of << inden(20) << "\"campOpSkydow\": true," << std::endl;
  of << inden(20) << "\"campOpRoyal\": true," << std::endl;
  of << inden(20) << "\"campOpThird\": true" << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16) << "\"map\": " << metadataLegalizer.getStr(metadata::map)
     << "," << std::endl;
  of << inden(16) << "\"maxAbilityLevel\": "
     << metadataLegalizer.getInt(metadata::maxAbilityLevel) << "," << std::endl;
  of << inden(16) << "\"nextGameEnabled\": "
     << metadataLegalizer.getBool(metadata::nextGameEnabled) << ","
     << std::endl;
  of << inden(16) << "\"disableNextGameOnMissionComplete\": "
     << metadataLegalizer.getBool(metadata::disableNextGameOnMissionComplete)
     << "," << std::endl;
  of << inden(16) << "\"playDefaultMusic\": "
     << metadataLegalizer.getBool(metadata::playDefaultMusic) << ","
     << std::endl;
  of << inden(16) << "\"cameraAfterOver\": \"restrict\"," << std::endl;
  of << inden(16) << "\"useDefaultItems\": "
     << metadataLegalizer.getBool(metadata::useDefaultItems) << ","
     << std::endl;
  of << inden(16) << "\"useDefaultCampLocs\": "
     << metadataLegalizer.getBool(metadata::useDefaultCampLocs) << ","
     << std::endl;
  of << inden(16) << "\"skydowLocs\": "
     << metadataLegalizer.getList(metadata::skydowLocs, 20) << "," << std::endl;
  of << inden(16)
     << "\"royalLocs\": " << metadataLegalizer.getList(metadata::royalLocs, 20)
     << "," << std::endl;
  of << inden(16)
     << "\"thirdLocs\": " << metadataLegalizer.getList(metadata::thirdLocs, 20)
     << "," << std::endl;
  of << inden(16) << "\"useCustomWeapons\": "
     << metadataLegalizer.getBool(metadata::useCustomWeapons) << ","
     << std::endl;
  of << inden(16) << "\"customWeapons\": []," << std::endl;
  of << inden(16) << "\"useCustomItems\": "
     << metadataLegalizer.getBool(metadata::useCustomItems) << "," << std::endl;
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
    of << inden(8) << "{" << std::endl;
    of << inden(12) << "\"id\": \"" << block->identifier << "\"," << std::endl;
    of << inden(12) << "\"disabled\": false," << std::endl;
    of << inden(12) << "\"folder\": \"\"," << std::endl;
    of << inden(12) << "\"startTime\": 0," << std::endl;
    of << inden(12) << "\"checkInterval\": 10," << std::endl;
    of << inden(12) << "\"repeats\": 0," << std::endl;
    of << inden(12) << "\"repeatInterval\": 0," << std::endl;
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
    return twge::action::addActor(of, action);
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