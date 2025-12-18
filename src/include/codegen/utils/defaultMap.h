#ifndef CODEGEN_DEFAULTMAP_H
#define CODEGEN_DEFAULTMAP_H

#include "ast.h"
#include "instruction.h"
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

struct DefaultMapValue {
  config::ASTType astType;
  config::CodegenType codegenType;
  string defaultValue;
};

class DefaultMap {
private:
  // Functions
  void verifyInputMap();
  string print();

public:
  // Variables
  std::map<string, DefaultMapValue> defaultMap;
  std::map<string, const std::shared_ptr<ValueNode>> inputMap;
  const string functionName;
  bool isMetadata;
  // Constructor
  DefaultMap(std::map<string, DefaultMapValue> defaultMap,
             string functionName = "", bool isMetadata = false)
      : functionName(functionName), defaultMap(defaultMap),
        isMetadata(isMetadata) {}
  DefaultMap(config::InstructionConfig config,
             vector<unique_ptr<NamedParamAppsNode>> &namedArgs)
      : functionName(config.name), isMetadata(false) {
    for (auto &param : config.params)
      defaultMap.insert(
          {param.key, {param.astType, param.codegenType, param.defaultValue}});
    addInputMap(namedArgs);
  }
  // Functions
  void clearInputMap() { inputMap = {}; }
  void addInputMap(vector<unique_ptr<MetadataNode>> &metadatas);
  void addInputMap(vector<unique_ptr<NamedParamAppsNode>> &namedArgs);
  string get(string key, keyword::KeywordEnum keywordEnum = emptyKeywordEnum,
             std::map<string, string> extraEnum = {});
};
} // namespace codegen
#endif // CODEGEN_DEFAULTMAP_H