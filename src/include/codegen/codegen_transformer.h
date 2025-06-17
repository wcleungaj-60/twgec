#ifndef CODEGEN_TRANSFORMER_H
#define CODEGEN_TRANSFORMER_H

#include "ast.h"
#include "keyword.h"
#include <iostream>
#include <string>
#include <unordered_map>

enum ASTType {
  AST_INVALID,
  AST_INT,
  AST_STRING,
  AST_LIST_POINT,
};

enum CodegenType {
  CODEGEN_INVALID,
  CODEGEN_INT,
  CODEGEN_STRING,      // two `\"` will be added
  CODEGEN_LIST_PATROL, // {loc{x,y,range},rotation,duration}
};

struct DefaultMapValue {
  ASTType astType;
  CodegenType codegenType;
  std::string defaultValue;
};

class DefaultMap {
public:
  const std::unordered_map<std::string, DefaultMapValue> defaultMap;
  DefaultMap(std::unordered_map<std::string, DefaultMapValue> defaultMap)
      : defaultMap(defaultMap) {}
  std::string get(std::string key) {
    if (defaultMap.find(key) != defaultMap.end()) {
      if (defaultMap.at(key).codegenType == CODEGEN_STRING)
        return "\"" + defaultMap.at(key).defaultValue + "\"";
      if (defaultMap.at(key).codegenType == CODEGEN_INT)
        return defaultMap.at(key).defaultValue;
      if (defaultMap.at(key).codegenType == CODEGEN_LIST_PATROL)
        return defaultMap.at(key).defaultValue;
    }
    std::cerr << "Compiler Implementation Error: Found unknown key \'" << key
              << "\'\n";
    return "";
  }
  std::string get(std::string key, const std::shared_ptr<ValueNode> &value,
                  std::shared_ptr<keyword::KeywordEnum> keywordEnum = nullptr) {
    if (defaultMap.find(key) == defaultMap.end()) {
      std::cerr << "Compiler Implementation Error: Found unknown key \'" << key
                << "\'\n";
      return "";
    }
    if (!value) {
      if (defaultMap.at(key).codegenType == CODEGEN_STRING)
        return "\"" + defaultMap.at(key).defaultValue + "\"";
      if (defaultMap.at(key).codegenType == CODEGEN_INT)
        return defaultMap.at(key).defaultValue;
      if (defaultMap.at(key).codegenType == CODEGEN_LIST_PATROL)
        return defaultMap.at(key).defaultValue;
    }
    if (defaultMap.at(key).astType == AST_INT)
      if (auto *intNode = dynamic_cast<IntValueNode *>(value.get())) {
        if (defaultMap.at(key).codegenType == CODEGEN_STRING)
          return "\"" + std::to_string(intNode->value) + "\"";
        else if (defaultMap.at(key).codegenType == CODEGEN_INT)
          return std::to_string(intNode->value);
      }
    if (defaultMap.at(key).astType == AST_STRING)
      if (auto *stringNode = dynamic_cast<StringValueNode *>(value.get())) {
        if (defaultMap.at(key).codegenType == CODEGEN_STRING) {
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
        if (defaultMap.at(key).codegenType == CODEGEN_INT) {
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
    if (defaultMap.at(key).astType == AST_LIST_POINT) {
      if (auto *listNode = dynamic_cast<ListValueNode *>(value.get())) {
        if (defaultMap.at(key).codegenType == CODEGEN_LIST_PATROL) {
          std::string ret = "[\n";
          for (auto idx = 0; idx < listNode->items.size(); idx++) {
            if (auto *pointNode = dynamic_cast<PointValueNode *>(
                    listNode->items[idx].get())) {
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
      }
    }
    std::cerr << "Compiler Implementation Error: incorrect type conversion at "
              << value->loc << "\n";
    return "";
  }
};

#endif