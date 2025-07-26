#include "codegen/transformer.h"
#include <iostream>

using namespace codegen;

std::string DefaultMap::get(std::string key, keyword::KeywordEnum keywordEnum) {
  if (defaultMap.find(key) == defaultMap.end()) {
    std::cerr << "Compiler Implementation Error: Found unknown key \'" << key
              << "\'\n";
    return "";
  }

  // Variable Initialization
  const std::shared_ptr<ValueNode> &input = inputMap[key];
  IntValueNode *intNode = nullptr;
  BoolValueNode *boolNode = nullptr;
  StringValueNode *stringNode = nullptr;
  ListValueNode *listNode = nullptr;
  bool astBool = defaultMap.at(key).astType == AST_BOOL;
  bool astInt = defaultMap.at(key).astType == AST_INT;
  bool astString = defaultMap.at(key).astType == AST_STRING;
  bool astPointList = defaultMap.at(key).astType == AST_LIST_POINT;
  if (auto varNode = dynamic_cast<VariableValueNode *>(input.get())) {
    std::cerr << "Codegen Error: Cannot find the definition of variable `"
              << varNode->value << "` at " << input->loc
              << ". Please define it as a constant value or function\'s param.\n";
    return "";
  }
  if (astInt) {
    intNode = dynamic_cast<IntValueNode *>(input.get());
    stringNode = dynamic_cast<StringValueNode *>(input.get());
  }
  if (astBool)
    boolNode = dynamic_cast<BoolValueNode *>(input.get());
  if (astString)
    stringNode = dynamic_cast<StringValueNode *>(input.get());
  if (astPointList)
    listNode = dynamic_cast<ListValueNode *>(input.get());
  bool codegenBool = defaultMap.at(key).codegenType == CODEGEN_BOOL;
  bool codegenInt = defaultMap.at(key).codegenType == CODEGEN_INT;
  bool codegenString = defaultMap.at(key).codegenType == CODEGEN_STRING;
  bool codegenListPatrol =
      defaultMap.at(key).codegenType == CODEGEN_LIST_PATROL;
  bool codegenListPoint = defaultMap.at(key).codegenType == CODEGEN_LIST_POINT;

  auto format = [codegenString](std::string text) -> std::string {
    return codegenString ? "\"" + text + "\"" : text;
  };

  // Return search result
  if (!input)
    return format(defaultMap.at(key).defaultValue);
  if (intNode)
    return format(std::to_string(intNode->value));
  if (boolNode)
    return format(boolNode->value ? "true" : "false");
  if (stringNode && keywordEnum.isEmpty())
    return format(stringNode->value);
  // Enum Input Geneartion
  if (stringNode && !keywordEnum.isEmpty()) {
    std::pair<bool, std::string> enumResult =
        keywordEnum.get(stringNode->value);
    if (!enumResult.first)
      std::cerr << " ->  Found at " << stringNode->loc << "\n";
    return format(enumResult.second);
  }
  // List Input Generation
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
            << input->loc << "\n";
  return "";
}