#include "codegen/transformer.h"
#include <iostream>

using namespace codegen;

std::string DefaultMap::get(std::string key,
                            const std::shared_ptr<ValueNode> &value,
                            std::shared_ptr<keyword::KeywordEnum> keywordEnum) {
  if (defaultMap.find(key) == defaultMap.end()) {
    std::cerr << "Compiler Implementation Error: Found unknown key \'" << key
              << "\'\n";
    return "";
  }
  // Variable Initialization
  IntValueNode *intNode = nullptr;
  BoolValueNode *boolNode = nullptr;
  StringValueNode *stringNode = nullptr;
  ListValueNode *listNode = nullptr;
  bool astBool = defaultMap.at(key).astType == AST_BOOL;
  bool astInt = defaultMap.at(key).astType == AST_INT;
  bool astString = defaultMap.at(key).astType == AST_STRING;
  bool astPointList = defaultMap.at(key).astType == AST_LIST_POINT;
  if (astInt)
    intNode = dynamic_cast<IntValueNode *>(value.get());
  if (astBool)
    boolNode = dynamic_cast<BoolValueNode *>(value.get());
  if (astString)
    stringNode = dynamic_cast<StringValueNode *>(value.get());
  if (astPointList)
    listNode = dynamic_cast<ListValueNode *>(value.get());
  bool codegenBool = defaultMap.at(key).codegenType == CODEGEN_BOOL;
  bool codegenInt = defaultMap.at(key).codegenType == CODEGEN_INT;
  bool codegenString = defaultMap.at(key).codegenType == CODEGEN_STRING;
  bool codegenListPatrol =
      defaultMap.at(key).codegenType == CODEGEN_LIST_PATROL;
  bool codegenListPoint = defaultMap.at(key).codegenType == CODEGEN_LIST_POINT;
  // Default Value
  if (!value) {
    if (codegenString)
      return "\"" + defaultMap.at(key).defaultValue + "\"";
    else
      return defaultMap.at(key).defaultValue;
  }
  // Int
  if (intNode) {
    if (codegenString)
      return "\"" + std::to_string(intNode->value) + "\"";
    else if (codegenInt)
      return std::to_string(intNode->value);
  }
  // Bool
  if (boolNode) {
    if (codegenBool)
      return boolNode->value ? "true" : "false";
  }
  if (stringNode) {
    if (codegenString) {
      if (!keywordEnum) {
        return "\"" + stringNode->value + "\"";
      } else {
        std::pair<bool, std::string> enumResult =
            keywordEnum.get()->get(stringNode->value);
        if (!enumResult.first)
          std::cerr << " ->  Found at " << stringNode->loc << "\n";
        return "\"" + enumResult.second + "\"";
      }
    }
    if (codegenInt) {
      if (!keywordEnum) {
        return stringNode->value;
      } else {
        std::pair<bool, std::string> enumResult =
            keywordEnum.get()->get(stringNode->value);
        if (!enumResult.first)
          std::cerr << " ->  Found at " << stringNode->loc << "\n";
        return enumResult.second;
      }
    }
  }
  if (listNode) {
    if (codegenListPatrol) {
      std::string ret = "[\n";
      for (auto idx = 0; idx < listNode->items.size(); idx++) {
        if (auto *pointNode =
                dynamic_cast<PointValueNode *>(listNode->items[idx].get())) {
          ret += std::string(28, ' ') + "{\n";
          ret += std::string(32, ' ') + "\"loc\": {\n";
          ret += std::string(36, ' ') + "\"x\": \"" +
                 std::to_string(pointNode->x) + "\",\n";
          ret += std::string(36, ' ') + "\"y\": \"" +
                 std::to_string(pointNode->y) + "\",\n";
          ret += std::string(36, ' ') + "\"range\": \"0\"\n";
          ret += std::string(32, ' ') + "},\n";
          ret += std::string(32, ' ') + "\"rotation\": \"0\",\n";
          ret += std::string(32, ' ') + "\"duration\": \"3000\"\n";
          ret += std::string(28, ' ') + "}";
          if (idx != listNode->items.size() - 1)
            ret += ",";
          ret += "\n";
        }
      }
      ret += std::string(24, ' ') + "]";
      return ret;
    }
    if (codegenListPoint) {
      std::string ret = "[\n";
      for (auto idx = 0; idx < listNode->items.size(); idx++) {
        if (auto *pointNode =
                dynamic_cast<PointValueNode *>(listNode->items[idx].get())) {
          ret += std::string(20, ' ') + "{\n";
          ret += std::string(24, ' ') + "\"x\": \"" +
                 std::to_string(pointNode->x) + "\",\n";
          ret += std::string(24, ' ') + "\"y\": \"" +
                 std::to_string(pointNode->y) + "\"\n";
          ret += std::string(20, ' ') + "}";
          if (idx != listNode->items.size() - 1)
            ret += ",";
          ret += "\n";
        }
      }
      ret += std::string(16, ' ') + "]";
      return ret;
    }
  }
  std::cerr << "Compiler Implementation Error: incorrect type conversion at "
            << value->loc << "\n";
  return "";
}