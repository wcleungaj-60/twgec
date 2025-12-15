#include "codegen/instruction/metadata.h"
#include "keyword.h"
#include "utils/defaultMap.h"
#include "utils/utils.h"
#include <fstream>
#include <iostream>

using namespace codegen;

DefaultMap metadata::ConfigSetup::defaultMap = DefaultMap(
    {
        {keyword::metadataKind::stageWidth,
         {config::AST_INT, config::CODEGEN_INT, "600"}},
        {keyword::metadataKind::stageHeight,
         {config::AST_INT, config::CODEGEN_INT, "500"}},
        {keyword::metadataKind::roomSize,
         {config::AST_INT, config::CODEGEN_INT, "4"}},
        {keyword::metadataKind::lives,
         {config::AST_INT, config::CODEGEN_INT, "3"}},
        {keyword::metadataKind::maxAbilityLevel,
         {config::AST_INT, config::CODEGEN_INT, "2"}},
        {keyword::metadataKind::bornDuration,
         {config::AST_INT, config::CODEGEN_INT, "300"}},
        {keyword::metadataKind::bornLockDuration,
         {config::AST_INT, config::CODEGEN_INT, "100"}},
        {keyword::metadataKind::minPlayers,
         {config::AST_INT, config::CODEGEN_INT, "1"}},
        {keyword::metadataKind::supportSignin,
         {config::AST_BOOL, config::CODEGEN_BOOL, "true"}},
        {keyword::metadataKind::mustLogin,
         {config::AST_BOOL, config::CODEGEN_BOOL, "true"}},
        {keyword::metadataKind::allowGuest,
         {config::AST_BOOL, config::CODEGEN_BOOL, "true"}},
        {keyword::metadataKind::supportMsgServer,
         {config::AST_BOOL, config::CODEGEN_BOOL, "true"}},
        {keyword::metadataKind::runGame,
         {config::AST_BOOL, config::CODEGEN_BOOL, "true"}},
        {keyword::metadataKind::campOpSkydow,
         {config::AST_BOOL, config::CODEGEN_BOOL, "true"}},
        {keyword::metadataKind::campOpRoyal,
         {config::AST_BOOL, config::CODEGEN_BOOL, "true"}},
        {keyword::metadataKind::campOpThird,
         {config::AST_BOOL, config::CODEGEN_BOOL, "true"}},
        {keyword::metadataKind::setInitFocus,
         {config::AST_BOOL, config::CODEGEN_BOOL, "false"}},
        {keyword::metadataKind::setBornDuration,
         {config::AST_BOOL, config::CODEGEN_BOOL, "false"}},
        {keyword::metadataKind::nextGameEnabled,
         {config::AST_BOOL, config::CODEGEN_BOOL, "true"}},
        {keyword::metadataKind::playDefaultMusic,
         {config::AST_BOOL, config::CODEGEN_BOOL, "true"}},
        {keyword::metadataKind::disableNextGameOnMissionComplete,
         {config::AST_BOOL, config::CODEGEN_BOOL, "false"}},
        {keyword::metadataKind::useDefaultItems,
         {config::AST_BOOL, config::CODEGEN_BOOL, "false"}},
        {keyword::metadataKind::defCarryItems,
         {config::AST_BOOL, config::CODEGEN_BOOL, "false"}},
        {keyword::metadataKind::useDefaultCampLocs,
         {config::AST_BOOL, config::CODEGEN_BOOL, "false"}},
        {keyword::metadataKind::useCustomWeapons,
         {config::AST_BOOL, config::CODEGEN_BOOL, "false"}},
        {keyword::metadataKind::customWeapons,
         {config::AST_LIST_CUSTOM_WEAPON, config::CODEGEN_LIST_CUSTOM_WEAPON,
          "[]"}},
        {keyword::metadataKind::useCustomFarWeapons,
         {config::AST_BOOL, config::CODEGEN_BOOL, "false"}},
        {keyword::metadataKind::useCustomItems,
         {config::AST_BOOL, config::CODEGEN_BOOL, "false"}},
        {keyword::metadataKind::title,
         {config::AST_STRING, config::CODEGEN_STRING, "我的任務"}},
        {keyword::metadataKind::map,
         {config::AST_STRING, config::CODEGEN_STRING,
          "CG.TWEventsGameTemplate/gamemap.twmap"}},
        {keyword::metadataKind::schema,
         {config::AST_STRING, config::CODEGEN_STRING,
          "https://code.gamelet.com/gassets/schema/events/v1"}},
        {keyword::metadataKind::stageBackgroundColor,
         {config::AST_STRING, config::CODEGEN_STRING, "#999999"}},
        {keyword::metadataKind::gamezoneCode,
         {config::AST_STRING, config::CODEGEN_STRING, "mission"}},
        {keyword::metadataKind::skydowLocs,
         {config::AST_LIST_POINT, config::CODEGEN_LIST_SPAWN_POINT, "[]"}},
        {keyword::metadataKind::royalLocs,
         {config::AST_LIST_POINT, config::CODEGEN_LIST_SPAWN_POINT, "[]"}},
        {keyword::metadataKind::thirdLocs,
         {config::AST_LIST_POINT, config::CODEGEN_LIST_SPAWN_POINT, "[]"}},
        {keyword::metadataKind::preloadSources,
         {config::AST_LIST_STRING, config::CODEGEN_LIST_STRING, "[]"}},
        {keyword::metadataKind::preloadResourcesExclude,
         {config::AST_LIST_STRING, config::CODEGEN_LIST_STRING, "[]"}},
        {keyword::metadataKind::carryItemCodes,
         {config::AST_LIST_STRING, config::CODEGEN_LIST_STRING, "[]"}},
    },
    "global metadata", true);

DefaultMap metadata::BlockSetup::defaultMap = DefaultMap(
    {
        {"delay", {config::AST_INT, config::CODEGEN_INT, "0"}},
        {"repeat", {config::AST_INT, config::CODEGEN_INT, "0"}},
        {"repeatInterval", {config::AST_INT, config::CODEGEN_INT, "0"}},
    },
    "block metadata", true);

// Enum
// metadataVec.push_back(std::make_unique<MetadataEnum>("roomType", "close",
// {"close", "open"}));
// metadataVec.push_back(std::make_unique<MetadataEnum>("stageAlignHorizontal",
// "center", {"left", "center", "right"}));
// metadataVec.push_back(std::make_unique<MetadataEnum>("stageAlignVertical",
// "middle", {"top", "middle", "bottom"}));
// metadataVec.push_back(std::make_unique<MetadataEnum>("cameraAfterOver",
// "free", {"free", "restrict", "none"}));
// metadataVec.push_back(std::make_unique<MetadataEnum>("stageResolutionPolicy",
// "showAll",
//              {"showAll", "exactFit", "noBorder", "fixedWidth",
//               "fixedHeight", "origin"}),
// metadataVec.push_back(std::make_unique<MetadataEnum>("gameStartFadein",
// "fadein",
//              {"fadein", "fadeinL", "dark", "show"}));
// List
// std::make_unique<MetadataList<std::string>>("releaseCamp", {"third",
// "royal", "skydow"},
//                           {"third", "royal", "skydow", "random"}),
// std::make_unique<MetadataList<std::string>>("debugCamp", {"third", "royal",
// "skydow"},
//                           {"third", "royal", "skydow", "random"}),
// INIT_META_UNSUPPORT(metaMap, keyword::metadataKind::initFocus);
// INIT_META_UNSUPPORT(metaMap, keyword::metadataKind::customFarWeapons);
// INIT_META_UNSUPPORT(metaMap, keyword::metadataKind::customItems);

void metadata::ConfigSetup::setup(
    std::ofstream &of, std::vector<std::unique_ptr<MetadataNode>> &metadatas) {
  defaultMap.addInputMap(metadatas);
  of << inden(4)
     << "\"$schema\": " << defaultMap.get(keyword::metadataKind::schema) << ","
     << std::endl;
  of << inden(4) << "\"config\": {" << std::endl;
  of << inden(8) << "\"stage\": {" << std::endl;
  of << inden(12)
     << "\"width\": " << defaultMap.get(keyword::metadataKind::stageWidth)
     << "," << std::endl;
  of << inden(12)
     << "\"height\": " << defaultMap.get(keyword::metadataKind::stageHeight)
     << "," << std::endl;
  of << inden(12) << "\"backgroundColor\": "
     << defaultMap.get(keyword::metadataKind::stageBackgroundColor) << ","
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
  of << inden(16)
     << "\"title\": " << defaultMap.get(keyword::metadataKind::title) << ","
     << std::endl;
  of << inden(16) << "\"serverConfig\": {" << std::endl;
  of << inden(20)
     << "\"minPlayers\": " << defaultMap.get(keyword::metadataKind::minPlayers)
     << "," << std::endl;
  of << inden(20) << "\"supportSignin\": "
     << defaultMap.get(keyword::metadataKind::supportSignin) << ","
     << std::endl;
  of << inden(20)
     << "\"mustLogin\": " << defaultMap.get(keyword::metadataKind::mustLogin)
     << "," << std::endl;
  of << inden(20)
     << "\"allowGuest\": " << defaultMap.get(keyword::metadataKind::allowGuest)
     << "," << std::endl;
  of << inden(20) << "\"supportMsgServer\": "
     << defaultMap.get(keyword::metadataKind::supportMsgServer) << ","
     << std::endl;
  of << inden(20) << "\"gamezoneCode\": "
     << defaultMap.get(keyword::metadataKind::gamezoneCode) << "," << std::endl;
  of << inden(20) << "\"roomType\": \"close\"," << std::endl;
  of << inden(20)
     << "\"roomSize\": " << defaultMap.get(keyword::metadataKind::roomSize)
     << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16)
     << "\"runGame\": " << defaultMap.get(keyword::metadataKind::runGame) << ","
     << std::endl;
  of << inden(16) << "\"gameStartFadein\": \"fadein\"," << std::endl;
  of << inden(16)
     << "\"lives\": " << defaultMap.get(keyword::metadataKind::lives) << ","
     << std::endl;
  of << inden(16) << "\"debugCamp\": \"ask\"," << std::endl;
  of << inden(16) << "\"releaseCamp\": \"ask\"," << std::endl;
  of << inden(16) << "\"setInitFocus\": "
     << defaultMap.get(keyword::metadataKind::setInitFocus) << "," << std::endl;
  of << inden(16) << "\"initFocus\": {" << std::endl;
  of << inden(20) << "\"x\": \"0\"," << std::endl;
  of << inden(20) << "\"y\": \"0\"" << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16) << "\"campOptions\": {" << std::endl;
  of << inden(20) << "\"campOpSkydow\": "
     << defaultMap.get(keyword::metadataKind::campOpSkydow) << "," << std::endl;
  of << inden(20) << "\"campOpRoyal\": "
     << defaultMap.get(keyword::metadataKind::campOpRoyal) << "," << std::endl;
  of << inden(20) << "\"campOpThird\": "
     << defaultMap.get(keyword::metadataKind::campOpThird) << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16) << "\"map\": " << defaultMap.get(keyword::metadataKind::map)
     << "," << std::endl;
  of << inden(16) << "\"maxAbilityLevel\": "
     << defaultMap.get(keyword::metadataKind::maxAbilityLevel) << ","
     << std::endl;
  of << inden(16) << "\"nextGameEnabled\": "
     << defaultMap.get(keyword::metadataKind::nextGameEnabled) << ","
     << std::endl;
  of << inden(16) << "\"disableNextGameOnMissionComplete\": "
     << defaultMap.get(keyword::metadataKind::disableNextGameOnMissionComplete)
     << "," << std::endl;
  of << inden(16) << "\"playDefaultMusic\": "
     << defaultMap.get(keyword::metadataKind::playDefaultMusic) << ","
     << std::endl;
  of << inden(16) << "\"cameraAfterOver\": \"restrict\"," << std::endl;
  of << inden(16) << "\"useDefaultItems\": "
     << defaultMap.get(keyword::metadataKind::useDefaultItems) << ","
     << std::endl;
  of << inden(16) << "\"useDefaultCampLocs\": "
     << defaultMap.get(keyword::metadataKind::useDefaultCampLocs) << ","
     << std::endl;
  of << inden(16)
     << "\"skydowLocs\": " << defaultMap.get(keyword::metadataKind::skydowLocs)
     << "," << std::endl;
  of << inden(16)
     << "\"royalLocs\": " << defaultMap.get(keyword::metadataKind::royalLocs)
     << "," << std::endl;
  of << inden(16)
     << "\"thirdLocs\": " << defaultMap.get(keyword::metadataKind::thirdLocs)
     << "," << std::endl;
  of << inden(16) << "\"useCustomWeapons\": "
     << defaultMap.get(keyword::metadataKind::useCustomWeapons) << ","
     << std::endl;
  of << inden(16) << "\"customWeapons\": "
     << defaultMap.get(keyword::metadataKind::customWeapons) << ","
     << std::endl;
  of << inden(16) << "\"useCustomItems\": "
     << defaultMap.get(keyword::metadataKind::useCustomItems) << ","
     << std::endl;
  of << inden(16) << "\"customItems\": []," << std::endl;
  of << inden(16) << "\"enabled\": true," << std::endl;
  of << inden(16) << "\"@use\": 0" << std::endl;
  of << inden(12) << "}" << std::endl;
  of << inden(8) << "}" << std::endl;
  of << inden(4) << "}," << std::endl;
}

void metadata::BlockSetup::setup(
    std::ofstream &of, std::vector<std::unique_ptr<MetadataNode>> &metadatas) {
  defaultMap.addInputMap(metadatas);
  of << inden(12) << "\"disabled\": false," << std::endl;
  of << inden(12) << "\"folder\": \"\"," << std::endl;
  of << inden(12) << "\"startTime\": " << defaultMap.get("delay") << ","
     << std::endl;
  of << inden(12) << "\"checkInterval\": 10," << std::endl;
  of << inden(12) << "\"repeats\": " << defaultMap.get("repeat") << ","
     << std::endl;
  of << inden(12) << "\"repeatInterval\": " << defaultMap.get("repeatInterval")
     << "," << std::endl;
  of << inden(12) << "\"devOnly\": false," << std::endl;
}