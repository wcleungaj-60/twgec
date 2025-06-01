#include "codegen/metadata.h"

void MetadataLegalizer::initDefaultMap() {
  // Integer
  INIT_META_INT(metaMap, metadata::stageWidth, 600);
  INIT_META_INT(metaMap, metadata::stageHeight, 500);
  INIT_META_INT(metaMap, metadata::roomSize, 4, 1, 8);
  INIT_META_INT(metaMap, metadata::lives, 3);
  INIT_META_INT(metaMap, metadata::maxAbilityLevel, 2, 0, 2);
  INIT_META_INT(metaMap, metadata::bornDuration, 300);
  INIT_META_INT(metaMap, metadata::bornLockDuration, 100);
  INIT_META_INT(metaMap, metadata::minPlayers, 1, 1, 8);
  // Boolean
  INIT_META_BOOL(metaMap, metadata::supportSignin, true);
  INIT_META_BOOL(metaMap, metadata::mustLogin, true);
  INIT_META_BOOL(metaMap, metadata::allowGuest, true);
  INIT_META_BOOL(metaMap, metadata::supportMsgServer, true);
  INIT_META_BOOL(metaMap, metadata::runGame, true);
  INIT_META_BOOL(metaMap, metadata::campOpSkydow, true);
  INIT_META_BOOL(metaMap, metadata::campOpRoyal, true);
  INIT_META_BOOL(metaMap, metadata::campOpThird, true);
  INIT_META_BOOL(metaMap, metadata::setInitFocus, false);
  INIT_META_BOOL(metaMap, metadata::setBornDuration, false);
  INIT_META_BOOL(metaMap, metadata::nextGameEnabled, true);
  INIT_META_BOOL(metaMap, metadata::playDefaultMusic, true);
  INIT_META_BOOL(metaMap, metadata::disableNextGameOnMissionComplete, false);
  INIT_META_BOOL(metaMap, metadata::useDefaultItems, false);
  INIT_META_BOOL(metaMap, metadata::defCarryItems, false);
  INIT_META_BOOL(metaMap, metadata::useDefaultCampLocs, false);
  INIT_META_BOOL(metaMap, metadata::useCustomWeapons, false);
  INIT_META_BOOL(metaMap, metadata::useCustomFarWeapons, false);
  INIT_META_BOOL(metaMap, metadata::useCustomItems, false);
  // String
  INIT_META_STRING(metaMap, metadata::title, "", true);
  INIT_META_STRING(metaMap, metadata::map, "", true);
  INIT_META_STRING(metaMap, metadata::schema,
                   "\"https://code.gamelet.com/gassets/schema/events/v1\"");
  INIT_META_STRING(metaMap, metadata::stageBackgroundColor, "\"#999999\"");
  INIT_META_STRING(metaMap, metadata::gamezoneCode, "\"mission\"");
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
  INIT_META_LIST(metaMap, Point, metadata::skydowLocs);
  INIT_META_LIST(metaMap, Point, metadata::royalLocs);
  INIT_META_LIST(metaMap, Point, metadata::thirdLocs);
  INIT_META_LIST(metaMap, Point, metadata::initFocus);
  INIT_META_LIST(metaMap, std::string, metadata::preloadSources);
  INIT_META_LIST(metaMap, std::string, metadata::preloadResourcesExclude);
  INIT_META_LIST(metaMap, std::string, metadata::carryItemCodes);
  // std::make_unique<MetadataList<std::string>>("releaseCamp", {"third",
  // "royal", "skydow"},
  //                           {"third", "royal", "skydow", "random"}),
  // std::make_unique<MetadataList<std::string>>("debugCamp", {"third", "royal",
  // "skydow"},
  //                           {"third", "royal", "skydow", "random"}),
  // Unsupport
  INIT_META_UNSUPPORT(metaMap, metadata::customWeapons);
  INIT_META_UNSUPPORT(metaMap, metadata::customFarWeapons);
  INIT_META_UNSUPPORT(metaMap, metadata::customItems);

  for (auto &metadata : moduleNode->metadatas) {
    if (metaMap.find(metadata->key) == metaMap.end()) {
      std::cerr << "Unknown metadata: " << metadata->key << std::endl;
      return;
    }
    if (auto metaStr =
            dynamic_cast<MetadataString *>(metaMap[metadata->key].get())) {
      metaStr->value = metadata->value;
      continue;
    }
    std::cerr << "Unsupported metadata: " << metadata->key << std::endl;
    return;
  }
}

std::string MetadataLegalizer::getBool(std::string key) {
  if ((static_cast<MetadataBool *>(metaMap[key].get()))->value)
    return "true";
  return "false";
}

std::string MetadataLegalizer::getStr(std::string key) {
  return (static_cast<MetadataString *>(metaMap[key].get()))->value;
}

int MetadataLegalizer::getInt(std::string key) {
  return (static_cast<MetadataInteger *>(metaMap[key].get()))->value;
}