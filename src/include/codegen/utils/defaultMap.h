#ifndef CODEGEN_DEFAULTMAP_H
#define CODEGEN_DEFAULTMAP_H

#include "ast.h"
#include "keyword.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace codegen {
namespace {
static keyword::KeywordEnum emptyKeywordEnum = keyword::KeywordEnum("", {});
}
using std::string;
using std::unique_ptr;
using std::vector;

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
  CODEGEN_LIST_SPAWN_POINT,  // {x,y}
  CODEGEN_LIST_PATROL_POINT, // {loc{x,y,range},rotation,duration}
};

struct DefaultMapValue {
  ASTType astType;
  CodegenType codegenType;
  string defaultValue;
};

class DefaultMap {
private:
  // Functions
  void verifyInputMap();
  string print();

public:
  // Variables
  const std::map<string, DefaultMapValue> defaultMap;
  std::map<string, const std::shared_ptr<ValueNode>> inputMap;
  const string functionName;
  bool isMetadata;
  // Constructor
  DefaultMap(std::map<string, DefaultMapValue> defaultMap,
             string functionName = "", bool isMetadata = false)
      : functionName(functionName), defaultMap(defaultMap),
        isMetadata(isMetadata) {}
  // Functions
  void clearInputMap() { inputMap = {}; }
  void addInputMap(vector<unique_ptr<MetadataNode>> &metadatas);
  void addInputMap(vector<unique_ptr<NamedParamAppsNode>> &namedArgs);
  string get(string key, keyword::KeywordEnum keywordEnum = emptyKeywordEnum,
             std::map<string, string> extraEnum = {});
};
} // namespace codegen
#endif // CODEGEN_DEFAULTMAP_H