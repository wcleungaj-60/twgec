#ifndef METADATA_H
#define METADATA_H

#include "ast.h"
#include <memory>
#include <unordered_map>

#define INIT_META_INT(map, key, ...)                                           \
  map[key] = std::make_unique<MetadataInteger>(key, __VA_ARGS__)
#define INIT_META_BOOL(map, key, ...)                                          \
  map[key] = std::make_unique<MetadataBool>(key, __VA_ARGS__)
#define INIT_META_STRING(map, key, ...)                                        \
  map[key] = std::make_unique<MetadataString>(key, __VA_ARGS__)
#define INIT_META_LIST(map, type, key)                                         \
  map[key] = std::make_unique<MetadataList<type>>(key)
#define INIT_META_UNSUPPORT(map, key)                                          \
  map[key] = std::make_unique<MetadataUnsupport>(key)

namespace metadata {
// Int
const std::string stageWidth = "stageWidth";
const std::string stageHeight = "stageHeight";
const std::string roomSize = "roomSize";
const std::string lives = "lives";
const std::string maxAbilityLevel = "maxAbilityLevel";
const std::string bornDuration = "bornDuration";
const std::string bornLockDuration = "bornLockDuration";
const std::string minPlayers = "minPlayers";
// Boolean
const std::string supportSignin = "supportSignin";
const std::string mustLogin = "mustLogin";
const std::string allowGuest = "allowGuest";
const std::string supportMsgServer = "supportMsgServer";
const std::string runGame = "runGame";
const std::string campOpSkydow = "campOpSkydow";
const std::string campOpRoyal = "campOpRoyal";
const std::string campOpThird = "campOpThird";
const std::string setInitFocus = "setInitFocus";
const std::string setBornDuration = "setBornDuration";
const std::string nextGameEnabled = "nextGameEnabled";
const std::string playDefaultMusic = "playDefaultMusic";
const std::string disableNextGameOnMissionComplete =
    "disableNextGameOnMissionComplete";
const std::string useDefaultItems = "useDefaultItems";
const std::string defCarryItems = "defCarryItems";
const std::string useDefaultCampLocs = "useDefaultCampLocs";
const std::string useCustomWeapons = "useCustomWeapons";
const std::string useCustomFarWeapons = "useCustomFarWeapons";
const std::string useCustomItems = "useCustomItems";
// String
const std::string title = "title";
const std::string map = "map";
const std::string schema = "schema";
const std::string stageBackgroundColor = "stageBackgroundColor";
const std::string gamezoneCode = "gamezoneCode";
// List
const std::string skydowLocs = "skydowLocs";
const std::string royalLocs = "royalLocs";
const std::string thirdLocs = "thirdLocs";
const std::string initFocus = "initFocus";
const std::string preloadSources = "preloadSources";
const std::string preloadResourcesExclude = "preloadResourcesExclude";
const std::string carryItemCodes = "carryItemCodes";
// Unsupport
const std::string customWeapons = "customWeapons";
const std::string customFarWeapons = "customFarWeapons";
const std::string customItems = "customItems";
} // namespace metadata

struct Point {
  int x;
  int y;
};

class MetadataAbstract {
public:
  std::string key;
  MetadataAbstract(std::string key) : key(key){};
  virtual ~MetadataAbstract(){};
};

class MetadataBool : public MetadataAbstract {
public:
  bool value;
  MetadataBool(std::string key, bool value)
      : MetadataAbstract(key), value(value){};
};

class MetadataEnum : public MetadataAbstract {
public:
  std::string value;
  std::vector<std::string> possibleValue;
  MetadataEnum(std::string key, std::string value,
               std::vector<std::string> possibleValue)
      : MetadataAbstract(key), value(value), possibleValue(possibleValue){};
};

class MetadataString : public MetadataAbstract {
public:
  std::string value;
  std::vector<std::string> possibleValue;
  MetadataString(std::string key, std::string value)
      : MetadataAbstract(key), value(value){};
};

template <typename T> class MetadataList : public MetadataAbstract {
public:
  std::vector<T> value;
  std::vector<T> possibleValue;
  MetadataList(std::string key, std::vector<T> value = {},
               std::vector<T> possibleValue = {})
      : MetadataAbstract(key), value(value), possibleValue(possibleValue){};
};

class MetadataUnsupport : public MetadataAbstract {
public:
  std::string value;
  MetadataUnsupport(std::string key, std::string value = "")
      : MetadataAbstract(key), value(value){};
};

class MetadataInteger : public MetadataAbstract {
public:
  int value;
  int lowerBound;
  int upperBound;
  MetadataInteger(std::string key, int value, int lowerBound = -1,
                  int upperBound = -1)
      : MetadataAbstract(key), value(value), lowerBound(lowerBound),
        upperBound(upperBound){};
};

class MetadataLegalizer {
private:
  const std::unique_ptr<ModuleNode> &moduleNode;
  void initDefaultMap();
  std::unordered_map<std::string, std::unique_ptr<MetadataAbstract>> metaMap;

public:
  MetadataLegalizer(const std::unique_ptr<ModuleNode> &moduleNode)
      : moduleNode(moduleNode) {
    initDefaultMap();
  };
  std::string getStr(std::string key);
  std::string getBool(std::string key);
  int getInt(std::string key);
  std::string getList(std::string key, int indent);
};

#endif