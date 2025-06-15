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
};

enum CodegenType {
  CODEGEN_INVALID,
  CODEGEN_INT,
  CODEGEN_STRING, // two `\"` will be added
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
      }
    std::cerr << "Compiler Implementation Error: incorrect type conversion at "
              << value->loc << "\n";
    return "";
  }
};

#endif