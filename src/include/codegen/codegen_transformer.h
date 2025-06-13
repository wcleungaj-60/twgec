#ifndef CODEGEN_TRANSFORMER_H
#define CODEGEN_TRANSFORMER_H

#include "ast.h"
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
  const std::unordered_map<std::string, DefaultMapValue> map;
  DefaultMap(std::unordered_map<std::string, DefaultMapValue> map) : map(map) {}
  std::string get(std::string key) {
    if (map.find(key) != map.end()) {
      if (map.at(key).codegenType == CODEGEN_STRING)
        return "\"" + map.at(key).defaultValue + "\"";
      if (map.at(key).codegenType == CODEGEN_INT)
        return map.at(key).defaultValue;
    }
    std::cerr << "Compiler Implementation Error: Found unknown key \'" << key
              << "\'\n";
    return "";
  }
  std::string get(std::string key, const std::shared_ptr<ValueNode> &value) {
    std::string ret = "";
    if (map.find(key) == map.end()) {
      std::cerr << "Compiler Implementation Error: Found unknown key \'" << key
                << "\'\n";
      return ret;
    }
    if (!value) {
      if (map.at(key).codegenType == CODEGEN_STRING)
        return "\"" + map.at(key).defaultValue + "\"";
      if (map.at(key).codegenType == CODEGEN_INT)
        return map.at(key).defaultValue;
    }
    if (map.at(key).astType == AST_INT)
      if (auto *intNode = dynamic_cast<IntValueNode *>(value.get())) {
        if (map.at(key).codegenType == CODEGEN_STRING)
          ret = "\"" + std::to_string(intNode->value) + "\"";
        else if (map.at(key).codegenType == CODEGEN_INT)
          ret = std::to_string(intNode->value);
      }
    if (map.at(key).astType == AST_STRING)
      if (auto *stringNode = dynamic_cast<StringValueNode *>(value.get())) {
        if (map.at(key).codegenType == CODEGEN_STRING)
          ret = stringNode->value;
      }
    if (ret == "")
      std::cerr
          << "Compiler Implementation Error: incorrect type conversion at "
          << value->loc << "\n";
    return ret;
  }
};

#endif