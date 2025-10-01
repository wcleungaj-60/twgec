#ifndef CODEGEN_defaultMap_H
#define CODEGEN_defaultMap_H

#include "ast.h"
#include "keyword.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
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
  AST_LIST_CUSTOM_WEAPON,
};

inline std::ostream &operator<<(std::ostream &os, const ASTType &astType) {
  switch (astType) {
  case AST_BOOL:
    os << "bool";
    break;
  case AST_INT:
    os << "int|string";
    break;
  case AST_LIST_POINT:
    os << "list[Point]";
    break;
  case AST_POINT:
    os << "Point";
    break;
  case AST_ACTOR_MATCH:
    os << "ActorMatch";
    break;
  case AST_LIST_CUSTOM_WEAPON:
    os << "CustomWeapon";
    break;
  case AST_STRING:
    os << "string";
    break;
  case AST_LIST_STRING:
    os << "list[string]";
    break;
  case AST_INVALID:
    os << "?";
    break;
  }
  return os;
}

enum CodegenType {
  CODEGEN_INVALID,
  CODEGEN_INT,
  CODEGEN_STRING, // two `\"` will be added
  CODEGEN_BOOL,
  CODEGEN_ACTOR_MATCH,
  CODEGEN_LIST_CUSTOM_WEAPON,
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
      std::ostringstream oss;
      oss << it->second.astType;
      ret += oss.str() + inden(16 - oss.str().size());
      std::string key = it->first;
      if (isMetadata)
        key = "__" + key + "__";
      ret += " " + key + " = ";
      if (it->second.codegenType == CODEGEN_STRING)
        ret += "\"" + it->second.defaultValue + "\"";
      else if (it->second.codegenType == CODEGEN_ACTOR_MATCH)
        ret += "NULL";
      else if (it->second.codegenType == CODEGEN_LIST_CUSTOM_WEAPON)
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