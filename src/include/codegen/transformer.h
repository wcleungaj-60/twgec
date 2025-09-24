#ifndef CODEGEN_TRANSFORMER_H
#define CODEGEN_TRANSFORMER_H

#include "ast.h"
#include "keyword.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace codegen {
namespace {
static keyword::KeywordEnum emptyKeywordEnum = keyword::KeywordEnum("", {});
}
enum ASTType {
  AST_INVALID,
  AST_INT,
  AST_STRING,
  AST_BOOL,
  AST_POINT,
  AST_ACTOR_MATCH,
  AST_LIST_STRING,
  AST_LIST_POINT,
};

enum CodegenType {
  CODEGEN_INVALID,
  CODEGEN_INT,
  CODEGEN_STRING, // two `\"` will be added
  CODEGEN_BOOL,
  CODEGEN_ACTOR_MATCH,
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
    for (auto &input : inputMap)
      if (defaultMap.find(input.first) == defaultMap.end()) {
        std::cerr << "Codegen Warnings: Undefined key \"" << input.first
                  << "\" at " << input.second->loc << "\n";
        std::string name =
            isMetadata ? "available metadata list" : "function signature";
        std::cerr << "Please following this " << name << ":\n"
                  << print() << "\n";
      }
  };

  std::string print() {
    std::string ret = functionName + "(\n";
    for (auto it = defaultMap.begin(); it != defaultMap.end(); ++it) {
      ret += inden(4);
      switch (it->second.astType) {
      case AST_BOOL:
        ret += "bool" + inden(12);
        break;
      case AST_INT:
        ret += "int|string" + inden(6);
        break;
      case AST_LIST_POINT:
        ret += "list[Point]" + inden(5);
        break;
      case AST_POINT:
        ret += "Point" + inden(11);
        break;
      case AST_ACTOR_MATCH:
        ret += "ActorMatch" + inden(6);
        break;
      case AST_STRING:
        ret += "string" + inden(10);
        break;
      case AST_LIST_STRING:
        ret += "list[string]" + inden(4);
        break;
      case AST_INVALID:
        ret += "?" + inden(15);
        break;
      }
      std::string key = it->first;
      if (isMetadata)
        key = "__" + key + "__";
      ret += " " + key + " = ";
      if (it->second.codegenType == CODEGEN_STRING)
        ret += "\"" + it->second.defaultValue + "\"";
      else if (it->second.codegenType == CODEGEN_ACTOR_MATCH)
        ret += "NULL";
      else
        ret += it->second.defaultValue;
      if (std::next(it) != defaultMap.end())
        ret += ", ";
      ret += "\n";
    }
    ret += ")";
    return ret;
  }

public:
  const std::map<std::string, DefaultMapValue> defaultMap;
  const std::string functionName;
  bool isMetadata;
  std::map<std::string, const std::shared_ptr<ValueNode>> inputMap;
  DefaultMap(std::map<std::string, DefaultMapValue> defaultMap,
             std::string functionName = "", bool isMetadata = false)
      : functionName(functionName), defaultMap(defaultMap),
        isMetadata(isMetadata) {}
  void clearInputMap() { inputMap = {}; }
  void addInputMap(std::vector<std::unique_ptr<MetadataNode>> &metadatas) {
    clearInputMap();
    std::set<std::string> keySet;
    for (auto &metadata : metadatas) {
      std::string key = metadata->key;
      if (keySet.count(key))
        std::cerr << "Codegen Warnings: Redefined key \"" << key << "\" at "
                  << metadata->loc << "\n";
      inputMap.insert({key, std::move(metadata->expNode->value)});
      keySet.insert(key);
    }
    verifyInputMap();
  }
  void addInputMap(std::vector<std::unique_ptr<NamedArgNode>> &namedArgs,
                   std::vector<std::string> exclusion = {}) {
    clearInputMap();
    std::set<std::string> keySet;
    for (auto &namedArg : namedArgs) {
      std::string key = namedArg->key;
      if (std::find(exclusion.begin(), exclusion.end(), key) != exclusion.end())
        continue;
      if (keySet.count(key))
        std::cerr << "Codegen Warnings: Redefined key \"" << key << "\" at "
                  << namedArg->loc << "\n";
      inputMap.insert({key, std::move(namedArg->expNode->value)});
      keySet.insert(key);
    }
    verifyInputMap();
  }
  std::string get(std::string key,
                  keyword::KeywordEnum keywordEnum = emptyKeywordEnum);
};
} // namespace codegen
#endif