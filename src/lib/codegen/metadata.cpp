#include "codegen/metadata.h"
#include "utils/utils.h"
#include <fstream>
#include <iostream>

using namespace codegen;

DefaultMap metadata::ConfigSetup::defaultMap = DefaultMap({
    {keyword::config::stageWidth, {AST_INT, CODEGEN_INT, "600"}},
    {keyword::config::stageHeight, {AST_INT, CODEGEN_INT, "500"}},
    {keyword::config::roomSize, {AST_INT, CODEGEN_INT, "4"}},
    {keyword::config::lives, {AST_INT, CODEGEN_INT, "3"}},
    {keyword::config::maxAbilityLevel, {AST_INT, CODEGEN_INT, "2"}},
    {keyword::config::bornDuration, {AST_INT, CODEGEN_INT, "300"}},
    {keyword::config::bornLockDuration, {AST_INT, CODEGEN_INT, "100"}},
    {keyword::config::minPlayers, {AST_INT, CODEGEN_INT, "1"}},
    {keyword::config::supportSignin, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {keyword::config::mustLogin, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {keyword::config::allowGuest, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {keyword::config::supportMsgServer, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {keyword::config::runGame, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {keyword::config::campOpSkydow, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {keyword::config::campOpRoyal, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {keyword::config::campOpThird, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {keyword::config::setInitFocus, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {keyword::config::setBornDuration, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {keyword::config::nextGameEnabled, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {keyword::config::playDefaultMusic, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {keyword::config::disableNextGameOnMissionComplete,
     {AST_BOOL, CODEGEN_BOOL, "false"}},
    {keyword::config::useDefaultItems, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {keyword::config::defCarryItems, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {keyword::config::useDefaultCampLocs, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {keyword::config::useCustomWeapons, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {keyword::config::useCustomFarWeapons, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {keyword::config::useCustomItems, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {keyword::config::title, {AST_STRING, CODEGEN_STRING, "我的任務"}},
    {keyword::config::map,
     {AST_STRING, CODEGEN_STRING, "CG.TWEventsGameTemplate/gamemap.twmap"}},
    {keyword::config::schema,
     {AST_STRING, CODEGEN_STRING,
      "https://code.gamelet.com/gassets/schema/events/v1"}},
    {keyword::config::stageBackgroundColor,
     {AST_STRING, CODEGEN_STRING, "#999999"}},
    {keyword::config::gamezoneCode, {AST_STRING, CODEGEN_STRING, "mission"}},
    {keyword::config::skydowLocs, {AST_LIST_POINT, CODEGEN_LIST_POINT, "[]"}},
    {keyword::config::royalLocs, {AST_LIST_POINT, CODEGEN_LIST_POINT, "[]"}},
    {keyword::config::thirdLocs, {AST_LIST_POINT, CODEGEN_LIST_POINT, "[]"}},
    {keyword::config::preloadSources,
     {AST_LIST_STRING, CODEGEN_LIST_STRING, "[]"}},
    {keyword::config::preloadResourcesExclude,
     {AST_LIST_STRING, CODEGEN_LIST_STRING, "[]"}},
    {keyword::config::carryItemCodes,
     {AST_LIST_STRING, CODEGEN_LIST_STRING, "[]"}},
});

DefaultMap metadata::BlockSetup::defaultMap = DefaultMap({
    {"delay", {AST_INT, CODEGEN_INT, "0"}},
    {"repeat", {AST_INT, CODEGEN_INT, "0"}},
    {"repeatInterval", {AST_INT, CODEGEN_INT, "0"}},
});

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
// INIT_META_UNSUPPORT(metaMap, keyword::config::initFocus);
// INIT_META_UNSUPPORT(metaMap, keyword::config::customWeapons);
// INIT_META_UNSUPPORT(metaMap, keyword::config::customFarWeapons);
// INIT_META_UNSUPPORT(metaMap, keyword::config::customItems);

void metadata::ConfigSetup::setup(
    std::ofstream &of, std::vector<std::unique_ptr<MetadataNode>> &metadatas) {
  defaultMap.addInputMap(metadatas);
  of << inden(4) << "\"$schema\": " << defaultMap.get(keyword::config::schema)
     << "," << std::endl;
  of << inden(4) << "\"config\": {" << std::endl;
  of << inden(8) << "\"stage\": {" << std::endl;
  of << inden(12)
     << "\"width\": " << defaultMap.get(keyword::config::stageWidth) << ","
     << std::endl;
  of << inden(12)
     << "\"height\": " << defaultMap.get(keyword::config::stageHeight) << ","
     << std::endl;
  of << inden(12) << "\"backgroundColor\": "
     << defaultMap.get(keyword::config::stageBackgroundColor) << ","
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
  of << inden(16) << "\"title\": " << defaultMap.get(keyword::config::title)
     << "," << std::endl;
  of << inden(16) << "\"serverConfig\": {" << std::endl;
  of << inden(20)
     << "\"minPlayers\": " << defaultMap.get(keyword::config::minPlayers) << ","
     << std::endl;
  of << inden(20)
     << "\"supportSignin\": " << defaultMap.get(keyword::config::supportSignin)
     << "," << std::endl;
  of << inden(20)
     << "\"mustLogin\": " << defaultMap.get(keyword::config::mustLogin) << ","
     << std::endl;
  of << inden(20)
     << "\"allowGuest\": " << defaultMap.get(keyword::config::allowGuest) << ","
     << std::endl;
  of << inden(20) << "\"supportMsgServer\": "
     << defaultMap.get(keyword::config::supportMsgServer) << "," << std::endl;
  of << inden(20)
     << "\"gamezoneCode\": " << defaultMap.get(keyword::config::gamezoneCode)
     << "," << std::endl;
  of << inden(20) << "\"roomType\": \"close\"," << std::endl;
  of << inden(20)
     << "\"roomSize\": " << defaultMap.get(keyword::config::roomSize)
     << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16) << "\"runGame\": " << defaultMap.get(keyword::config::runGame)
     << "," << std::endl;
  of << inden(16) << "\"gameStartFadein\": \"fadein\"," << std::endl;
  of << inden(16) << "\"lives\": " << defaultMap.get(keyword::config::lives)
     << "," << std::endl;
  of << inden(16) << "\"debugCamp\": \"ask\"," << std::endl;
  of << inden(16) << "\"releaseCamp\": \"ask\"," << std::endl;
  of << inden(16)
     << "\"setInitFocus\": " << defaultMap.get(keyword::config::setInitFocus)
     << "," << std::endl;
  of << inden(16) << "\"initFocus\": {" << std::endl;
  of << inden(20) << "\"x\": \"0\"," << std::endl;
  of << inden(20) << "\"y\": \"0\"" << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16) << "\"campOptions\": {" << std::endl;
  of << inden(20)
     << "\"campOpSkydow\": " << defaultMap.get(keyword::config::campOpSkydow)
     << "," << std::endl;
  of << inden(20)
     << "\"campOpRoyal\": " << defaultMap.get(keyword::config::campOpRoyal)
     << "," << std::endl;
  of << inden(20)
     << "\"campOpThird\": " << defaultMap.get(keyword::config::campOpThird)
     << std::endl;
  of << inden(16) << "}," << std::endl;
  of << inden(16) << "\"map\": " << defaultMap.get(keyword::config::map) << ","
     << std::endl;
  of << inden(16) << "\"maxAbilityLevel\": "
     << defaultMap.get(keyword::config::maxAbilityLevel) << "," << std::endl;
  of << inden(16) << "\"nextGameEnabled\": "
     << defaultMap.get(keyword::config::nextGameEnabled) << "," << std::endl;
  of << inden(16) << "\"disableNextGameOnMissionComplete\": "
     << defaultMap.get(keyword::config::disableNextGameOnMissionComplete) << ","
     << std::endl;
  of << inden(16) << "\"playDefaultMusic\": "
     << defaultMap.get(keyword::config::playDefaultMusic) << "," << std::endl;
  of << inden(16) << "\"cameraAfterOver\": \"restrict\"," << std::endl;
  of << inden(16) << "\"useDefaultItems\": "
     << defaultMap.get(keyword::config::useDefaultItems) << "," << std::endl;
  of << inden(16) << "\"useDefaultCampLocs\": "
     << defaultMap.get(keyword::config::useDefaultCampLocs) << "," << std::endl;
  of << inden(16)
     << "\"skydowLocs\": " << defaultMap.get(keyword::config::skydowLocs) << ","
     << std::endl;
  of << inden(16)
     << "\"royalLocs\": " << defaultMap.get(keyword::config::royalLocs) << ","
     << std::endl;
  of << inden(16)
     << "\"thirdLocs\": " << defaultMap.get(keyword::config::thirdLocs) << ","
     << std::endl;
  of << inden(16) << "\"useCustomWeapons\": "
     << defaultMap.get(keyword::config::useCustomWeapons) << "," << std::endl;
  of << inden(16) << "\"customWeapons\": []," << std::endl;
  of << inden(16) << "\"useCustomItems\": "
     << defaultMap.get(keyword::config::useCustomItems) << "," << std::endl;
  of << inden(16) << "\"customItems\": []," << std::endl;
  of << inden(16) << "\"enabled\": true," << std::endl;
  of << inden(16) << "\"@use\": 0" << std::endl;
  of << inden(12) << "}" << std::endl;
  of << inden(8) << "}" << std::endl;
  of << inden(4) << "}," << std::endl;
}

void metadata::BlockSetup::setup(
    std::ofstream &of, std::vector<std::unique_ptr<MetadataNode>> &metadatas) {
  std::unordered_map<std::string, const std::shared_ptr<ValueNode>> inputMap;
  for (auto &metadata : metadatas)
    inputMap.insert({metadata->key, std::move(metadata->value)});
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