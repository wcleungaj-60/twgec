#ifndef CODEGEN_TRANSFORMER_H
#define CODEGEN_TRANSFORMER_H

#include "ast.h"
#include "keyword.h"
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

namespace codegen {
enum ASTType {
  AST_INVALID,
  AST_INT,
  AST_STRING,
  AST_BOOL,
  AST_LIST_STRING,
  AST_LIST_POINT,
};

enum CodegenType {
  CODEGEN_INVALID,
  CODEGEN_INT,
  CODEGEN_STRING, // two `\"` will be added
  CODEGEN_BOOL,
  CODEGEN_LIST_STRING,
  CODEGEN_LIST_POINT,  // {x,y}
  CODEGEN_LIST_PATROL, // {loc{x,y,range},rotation,duration}
};

struct DefaultMapValue {
  ASTType astType;
  CodegenType codegenType;
  std::string defaultValue;
};

class DefaultMap {
private:
  void verifyInputMap() {
    for (auto &input : inputMap) {
      if (defaultMap.find(input.first) == defaultMap.end())
        std::cerr << "Codegen Warnings: Undefined key \"" << input.first
                  << "\" at " << input.second->loc << "\n";
    }
  };

public:
  const std::unordered_map<std::string, DefaultMapValue> defaultMap;
  std::unordered_map<std::string, const std::shared_ptr<ValueNode>> inputMap;
  DefaultMap(std::unordered_map<std::string, DefaultMapValue> defaultMap)
      : defaultMap(defaultMap) {}
  void addInputMap(std::vector<std::unique_ptr<MetadataNode>> &metadatas) {
    inputMap = {};
    std::set<std::string> keySet;
    for (auto &metadata : metadatas) {
      std::string key = metadata->key;
      if (keySet.count(key))
        std::cerr << "Codegen Warnings: Redefined key \"" << key << "\" at "
                  << metadata->loc << "\n";
      inputMap.insert({key, std::move(metadata->value)});
      keySet.insert(key);
    }
    verifyInputMap();
  }
  void addInputMap(std::vector<std::unique_ptr<NamedArgNode>> &namedArgs) {
    inputMap = {};
    std::set<std::string> keySet;
    for (auto &namedArg : namedArgs) {
      std::string key = namedArg->key;
      if (keySet.count(key))
        std::cerr << "Codegen Warnings: Redefined key \"" << key << "\" at "
                  << namedArg->loc << "\n";
      inputMap.insert({key, std::move(namedArg->valueNode)});
      keySet.insert(key);
    }
    verifyInputMap();
  }
  std::string get(std::string key,
                  std::shared_ptr<keyword::KeywordEnum> keywordEnum = nullptr);
};
} // namespace codegen
#endif