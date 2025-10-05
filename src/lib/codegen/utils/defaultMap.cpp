#include "utils/defaultMap.h"
#include "utils.h"
#include "utils/builtin.h"
#include <iostream>
#include <set>
#include <sstream>

using namespace codegen;
using std::cerr;

namespace {
string getCodeGen(CodegenType codegenType, string text) {
  switch (codegenType) {
  case CODEGEN_INT:
  case CODEGEN_BOOL:
    return text;
  case CODEGEN_STRING:
    return "\"" + text + "\"";
  case CODEGEN_ACTOR_MATCH:
  case CODEGEN_LIST_CUSTOM_WEAPON:
  case CODEGEN_LIST_SPAWN_POINT:
  case CODEGEN_LIST_PATROL_POINT:
    assert(text == "[]" && "`[]` is expected as a default value.\n");
    return text;
  default:
    assert(false && "Unreachable codegen type.\n");
    return text;
  }
};
} // namespace

void DefaultMap::verifyInputMap() {
  for (auto &input : inputMap)
    if (defaultMap.find(input.first) == defaultMap.end()) {
      cerr << "Codegen Warnings: Undefined key \"" << input.first << "\" at "
           << input.second->loc << "\n";
      string name =
          isMetadata ? "available metadata list" : "function signature";
      cerr << "Please following this " << name << ":\n" << print() << "\n";
    }
};

string DefaultMap::print() {
  string ret = functionName + "(\n";
  for (auto it = defaultMap.begin(); it != defaultMap.end(); ++it) {
    ret += inden(4);
    std::ostringstream oss;
    oss << it->second.astType;
    ret += oss.str() + inden(16 - oss.str().size());
    string key = it->first;
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

string DefaultMap::get(string key, keyword::KeywordEnum keywordEnum,
                       std::map<string, string> extraEnum) {
  // Handle Unknown key or unknown value
  auto codegenType = defaultMap.at(key).codegenType;
  auto astType = defaultMap.at(key).astType;
  const std::shared_ptr<ValueNode> &input = inputMap[key];
  if (defaultMap.find(key) == defaultMap.end()) {
    assert(false && "Found unknown key\n");
    return "";
  }
  if (auto varNode = dynamic_cast<VariableValueNode *>(input.get())) {
    cerr << "Codegen Error: Cannot find the definition of variable `"
         << varNode->value << "` at " << input->loc
         << ". Please define it as a constant value or function\'s param.\n";
    return "";
  }
  if (!input)
    return getCodeGen(codegenType, defaultMap.at(key).defaultValue);

  // Handle Built-in Type
  switch (codegenType) {
  case CODEGEN_LIST_CUSTOM_WEAPON:
    return getCustomWeaponsListNode(input).to_string(16);
  case CODEGEN_LIST_SPAWN_POINT:
    return getSpawnPointListNode(input).to_string(16);
  case CODEGEN_LIST_PATROL_POINT:
    return getPatrolPathListNode(input).to_string(24);
  case CODEGEN_ACTOR_MATCH:
    return getActorMatchesNode(input).to_string(24);
  default:
    break;
  }

  // Handle Int Type
  if (astType == AST_INT) {
    if (auto intNode = dynamic_cast<IntValueNode *>(input.get()))
      return getCodeGen(codegenType, std::to_string(intNode->value));
    else if (auto stringNode = dynamic_cast<StringValueNode *>(input.get()))
      return getCodeGen(codegenType, stringNode->value);
    else
      cerr << "int-typed or string-typed value is expected at " << input->loc
           << "\n";
    return "";
  }

  // Handle Bool Type
  if (astType == AST_BOOL) {
    if (auto boolNode = dynamic_cast<BoolValueNode *>(input.get()))
      return getCodeGen(codegenType, boolNode->value ? "true" : "false");
    else
      cerr << "bool-typed value is expected at " << input->loc << "\n";
    return "";
  }

  // Handle String Type (and Enum)
  if (astType == AST_STRING) {
    auto stringNode = dynamic_cast<StringValueNode *>(input.get());
    if (!stringNode) {
      cerr << "string-typed value is expected at " << input->loc << "\n";
      return "";
    }
    if (keywordEnum.isEmpty())
      return getCodeGen(codegenType, stringNode->value);
    std::pair<bool, string> enumResult =
        keywordEnum.get(stringNode->value, extraEnum);
    if (!enumResult.first)
      cerr << " ->  Found at " << stringNode->loc << "\n";
    return getCodeGen(codegenType, enumResult.second);
  }

  // Unreachable
  assert(false && "Incorrect type conversion\n");
  return "";
}

void DefaultMap::addInputMap(vector<unique_ptr<MetadataNode>> &metadatas) {
  clearInputMap();
  std::set<string> keySet;
  for (auto &metadata : metadatas) {
    string key = metadata->key;
    if (keySet.count(key))
      cerr << "Codegen Warnings: Redefined key \"" << key << "\" at "
           << metadata->loc << "\n";
    inputMap.insert({key, std::move(metadata->expNode->value)});
    keySet.insert(key);
  }
  verifyInputMap();
}

void DefaultMap::addInputMap(vector<unique_ptr<NamedParamAppsNode>> &namedArgs) {
  clearInputMap();
  std::set<string> keySet;
  for (auto &namedArg : namedArgs) {
    string key = namedArg->key;
    if (keySet.count(key))
      cerr << "Codegen Warnings: Redefined key \"" << key << "\" at "
           << namedArg->loc << "\n";
    inputMap.insert({key, std::move(namedArg->expNode->value)});
    keySet.insert(key);
  }
  verifyInputMap();
}