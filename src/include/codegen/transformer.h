#ifndef CODEGEN_TRANSFORMER_H
#define CODEGEN_TRANSFORMER_H

#include "ast.h"
#include "keyword.h"
#include "utils.h"
#include <iostream>
#include <map>
#include <set>
#include <string>

namespace codegen {
namespace {
static keyword::KeywordEnum emptyKeywordEnum = keyword::KeywordEnum("", {});
}
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
    for (auto &input : inputMap)
      if (defaultMap.find(input.first) == defaultMap.end()) {
        std::cerr << "Codegen Warnings: Undefined key \"" << input.first
                  << "\" at " << input.second->loc << "\n";
        if (!functionName.empty())
          std::cerr << "Please following this function signature:\n"
                    << print() << "\n";
        // TODO: handle metadata
      }
  };

  std::string print() {
    std::string ret = functionName + "(\n";
    for (auto it = defaultMap.begin(); it != defaultMap.end(); ++it) {
      ret += inden(4);
      switch (it->second.astType) {
      case AST_BOOL:
        ret += "bool"+ inden(12);
        break;
      case AST_INT:
        ret += "int|string"+ inden(6);
        break;
      case AST_LIST_POINT:
        ret += "list[point]" + inden(5);
        break;
      case AST_STRING:
        ret += "string"+ inden(10);
        break;
      case AST_LIST_STRING:
        ret += "list[string]"+ inden(4);
        break;
      case AST_INVALID:
        ret += "?"+ inden(15);
        break;
      }
      ret += " " + it->first + " = ";
      if (it->second.codegenType == CODEGEN_STRING)
        ret += "\"" + it->second.defaultValue + "\"";
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
  std::map<std::string, const std::shared_ptr<ValueNode>> inputMap;
  DefaultMap(std::map<std::string, DefaultMapValue> defaultMap,
             std::string functionName = "")
      : functionName(functionName), defaultMap(defaultMap) {}
  void addInputMap(std::vector<std::unique_ptr<MetadataNode>> &metadatas) {
    inputMap = {};
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
  void addInputMap(std::vector<std::unique_ptr<NamedArgNode>> &namedArgs) {
    inputMap = {};
    std::set<std::string> keySet;
    for (auto &namedArg : namedArgs) {
      std::string key = namedArg->key;
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