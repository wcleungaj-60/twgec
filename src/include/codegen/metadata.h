#ifndef METADATA_H
#define METADATA_H

#include "ast.h"
#include <memory>
#include <unordered_map>

#define META_INT(...) std::make_unique<MetadataInteger>(__VA_ARGS__)
#define META_BOOL(...) std::make_unique<MetadataBool>(__VA_ARGS__)
#define META_STRING(...) std::make_unique<MetadataString>(__VA_ARGS__)
#define META_LIST(type, ...) std::make_unique<MetadataList<type>>(__VA_ARGS__)
#define META_UNSUPPORT(...) std::make_unique<MetadataUnsupport>(__VA_ARGS__)

struct Point {
  int x;
  int y;
};

class MetadataAbstract {
public:
  std::string key;
  bool isCompulsory;
  MetadataAbstract(std::string key, bool isCompulsory = false)
      : key(key), isCompulsory(isCompulsory){};
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
  MetadataString(std::string key, std::string value, bool isCompulsory = false)
      : MetadataAbstract(key, isCompulsory), value(value){};
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
};

#endif