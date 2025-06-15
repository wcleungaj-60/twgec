#include "codegen/metadata.h"
#include "utils/utils.h"

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
  INIT_META_STRING(metaMap, metadata::title, "我的任務");
  INIT_META_STRING(metaMap, metadata::map,
                   "CG.TWEventsGameTemplate/gamemap.twmap");
  INIT_META_STRING(metaMap, metadata::schema,
                   "https://code.gamelet.com/gassets/schema/events/v1");
  INIT_META_STRING(metaMap, metadata::stageBackgroundColor, "#999999");
  INIT_META_STRING(metaMap, metadata::gamezoneCode, "mission");
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
  INIT_META_UNSUPPORT(metaMap, metadata::initFocus);
  INIT_META_UNSUPPORT(metaMap, metadata::customWeapons);
  INIT_META_UNSUPPORT(metaMap, metadata::customFarWeapons);
  INIT_META_UNSUPPORT(metaMap, metadata::customItems);

  for (auto &metadata : moduleNode->metadatas) {
    if (metaMap.find(metadata->key) == metaMap.end()) {
      std::cerr << "Codegen error: Unknown metadata \'" << metadata->key
                << "\' at " << metadata->loc << std::endl;
      return;
    }
    auto codegenStr =
        dynamic_cast<MetadataString *>(metaMap[metadata->key].get());
    auto codegenInt =
        dynamic_cast<MetadataInteger *>(metaMap[metadata->key].get());
    auto codegenBool =
        dynamic_cast<MetadataBool *>(metaMap[metadata->key].get());
    auto codegenPointList =
        dynamic_cast<MetadataList<Point> *>(metaMap[metadata->key].get());
    auto astStr = dynamic_cast<StringValueNode *>(metadata->value.get());
    auto astInt = dynamic_cast<IntValueNode *>(metadata->value.get());
    auto astBool = dynamic_cast<BoolValueNode *>(metadata->value.get());
    auto astList = dynamic_cast<ListValueNode *>(metadata->value.get());
    if (codegenStr && astStr) {
      codegenStr->value = astStr->value;
      continue;
    }
    if (codegenInt && astInt) {
      codegenInt->value = astInt->value;
      continue;
    }
    if (codegenBool && astBool) {
      codegenBool->value = astBool->value;
      continue;
    }
    if (codegenPointList && astList) {
      for (auto &itemNode : astList->items) {
        if (auto pointNode = dynamic_cast<PointValueNode *>(itemNode.get())) {
          codegenPointList->value.push_back({pointNode->x, pointNode->y});
        } else {
          std::cerr << "Codegen error: Metadata \'" << metadata->key
                    << "\' expects a point type value at " << itemNode->loc
                    << std::endl;
          return;
        }
      }
      continue;
    }
    std::cerr << "Codegen error: twgec hasn\'t yet supported this metadata \'"
              << metadata->key << "\' at " << metadata->loc << std::endl;
    return;
  }
}

std::string MetadataLegalizer::getBool(std::string key) {
  if ((static_cast<MetadataBool *>(metaMap[key].get()))->value)
    return "true";
  return "false";
}

std::string MetadataLegalizer::getStr(std::string key) {
  return "\"" + (static_cast<MetadataString *>(metaMap[key].get()))->value +
         "\"";
}

int MetadataLegalizer::getInt(std::string key) {
  return (static_cast<MetadataInteger *>(metaMap[key].get()))->value;
}

std::string MetadataLegalizer::getList(std::string key, int indent) {
  auto codegenListPoint =
      dynamic_cast<MetadataList<Point> *>(metaMap[key].get());
  if (!codegenListPoint) {
    std::cerr << "Codegen error: twgec hasn\'t yet supported this metadata \'"
              << key << std::endl;
  }
  std::string ret = "[\n";
  for (auto i = 0; i < codegenListPoint->value.size(); i++) {
    Point pt = codegenListPoint->value[i];
    ret += inden(indent + 4) + "{\n";
    ret += inden(indent + 8) + "\"x\": \"" + std::to_string(pt.x) + "\",\n";
    ret += inden(indent + 8) + "\"y\": \"" + std::to_string(pt.y) + "\"\n";
    ret += inden(indent + 4) + "}";
    if (i != codegenListPoint->value.size() - 1)
      ret += ",\n";
  }
  ret += "\n" + inden(indent) + "]";
  return ret;
}