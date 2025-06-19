#ifndef CODEGEN_TRANSFORMER_H
#define CODEGEN_TRANSFORMER_H

#include "ast.h"
#include "keyword.h"
#include <string>
#include <unordered_map>

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
public:
  const std::unordered_map<std::string, DefaultMapValue> defaultMap;
  DefaultMap(std::unordered_map<std::string, DefaultMapValue> defaultMap)
      : defaultMap(defaultMap) {}
  std::string get(std::string key, const std::shared_ptr<ValueNode> &value,
                  std::shared_ptr<keyword::KeywordEnum> keywordEnum = nullptr);
};

#endif