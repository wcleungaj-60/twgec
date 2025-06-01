#include "codegen/metadata.h"

void MetadataLegalizer::initDefaultMap() {
  // Integer
  metaMap["stageWidth"] = META_INT("stageWidth", 600);
  metaMap["stageHeight"] = META_INT("stageHeight", 500);
  metaMap["roomSize"] = META_INT("roomSize", 4, 1, 8);
  metaMap["lives"] = META_INT("lives", 3);
  metaMap["maxAbilityLevel"] = META_INT("maxAbilityLevel", 2, 0, 2);
  metaMap["bornDuration"] = META_INT("bornDuration", 300);
  metaMap["bornLockDuration"] = META_INT("bornLockDuration", 100);
  metaMap["minPlayers"] = META_INT("minPlayers", 1, 1, 8);
  // Boolean
  metaMap["supportSignin"] = META_BOOL("supportSignin", true);
  metaMap["mustLogin"] = META_BOOL("mustLogin", true);
  metaMap["allowGuest"] = META_BOOL("allowGuest", true);
  metaMap["supportMsgServer"] = META_BOOL("supportMsgServer", true);
  metaMap["runGame"] = META_BOOL("runGame", true);
  metaMap["campOpSkydow"] = META_BOOL("campOpSkydow", true);
  metaMap["campOpRoyal"] = META_BOOL("campOpRoyal", true);
  metaMap["campOpThird"] = META_BOOL("campOpThird", true);
  metaMap["setInitFocus"] = META_BOOL("setInitFocus", false);
  metaMap["setBornDuration"] = META_BOOL("setBornDuration", false);
  metaMap["nextGameEnabled"] = META_BOOL("nextGameEnabled", true);
  metaMap["playDefaultMusic"] = META_BOOL("playDefaultMusic", true);
  metaMap["disableNextGameOnMissionComplete"] =
      META_BOOL("disableNextGameOnMissionComplete", false);
  metaMap["useDefaultItems"] = META_BOOL("useDefaultItems", false);
  metaMap["defCarryItems"] = META_BOOL("defCarryItems", false);
  metaMap["useDefaultCampLocs"] = META_BOOL("useDefaultCampLocs", false);
  metaMap["useCustomWeapons"] = META_BOOL("useCustomWeapons", false);
  metaMap["useCustomFarWeapons"] = META_BOOL("useCustomFarWeapons", false);
  metaMap["useCustomItems"] = META_BOOL("useCustomItems", false);
  metaMap["allowGuest"] = META_BOOL("allowGuest", true);
  // String
  metaMap["title"] = META_STRING("title", "", true);
  metaMap["map"] = META_STRING("map", "", true);
  metaMap["schema"] = META_STRING("schema", "\"https://code.gamelet.com/gassets/schema/events/v1\"");
  metaMap["stageBackgroundColor"] =
      META_STRING("stageBackgroundColor", "\"#999999\"");
  metaMap["gamezoneCode"] = META_STRING("gamezoneCode", "\"mission\"");
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
  metaMap["skydowLocs"] = META_LIST(Point, "skydowLocs");
  metaMap["royalLocs"] = META_LIST(Point, "royalLocs");
  metaMap["thirdLocs"] = META_LIST(Point, "thirdLocs");
  metaMap["initFocus"] = META_LIST(Point, "initFocus");
  metaMap["preloadSources"] = META_LIST(std::string, "preloadSources");
  metaMap["preloadResourcesExclude"] =
      META_LIST(std::string, "preloadResourcesExclude");
  metaMap["carryItemCodes"] = META_LIST(std::string, "carryItemCodes");
  // std::make_unique<MetadataList<std::string>>("releaseCamp", {"third",
  // "royal", "skydow"},
  //                           {"third", "royal", "skydow", "random"}),
  // std::make_unique<MetadataList<std::string>>("debugCamp", {"third", "royal",
  // "skydow"},
  //                           {"third", "royal", "skydow", "random"}),
  // Unsupport
  metaMap["customWeapons"] = META_UNSUPPORT("customWeapons");
  metaMap["customFarWeapons"] = META_UNSUPPORT("customFarWeapons");
  metaMap["customItems"] = META_UNSUPPORT("customItems");

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

std::string MetadataLegalizer::getStr(std::string key) {
  return (static_cast<MetadataString *>(metaMap[key].get()))->value;
}