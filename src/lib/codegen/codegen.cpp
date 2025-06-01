#include "codegen.h"
#include <fstream>
#include <iostream>

/** Helper Function */
std::string inden(int _) { return std::string(_, ' '); }

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
  of << inden(12) << "\"title\": " << metadataLegalizer.getStr(metadata::title)
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
  of << inden(16) << "\"skydowLocs\": []," << std::endl;
  of << inden(16) << "\"royalLocs\": []," << std::endl;
  of << inden(16) << "\"thirdLocs\": []," << std::endl;
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

void CodeGen::codegenModuleNode(std::ofstream &of) {
  of << "{" << std::endl;
  codegenMetaData(of);
  of << "    \"events\": []" << std::endl;
  of << "}" << std::endl;
}