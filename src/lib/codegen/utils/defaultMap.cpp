#include "utils/defaultMap.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>

using namespace codegen;
using std::cerr;

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
  if (defaultMap.find(key) == defaultMap.end()) {
    cerr << "Compiler Implementation Error: Found unknown key \'" << key
         << "\'\n";
    return "";
  }

  // Variable Initialization
  const std::shared_ptr<ValueNode> &input = inputMap[key];
  IntValueNode *intNode = nullptr;
  BoolValueNode *boolNode = nullptr;
  StringValueNode *stringNode = nullptr;
  ListValueNode *listNode = nullptr;
  ActorMatchValueNode *actorMatchNode = nullptr;
  CustomWeaponValueNode *customWeaponNode = nullptr;
  bool astBool = defaultMap.at(key).astType == AST_BOOL;
  bool astInt = defaultMap.at(key).astType == AST_INT;
  bool astString = defaultMap.at(key).astType == AST_STRING;
  bool astPointList = defaultMap.at(key).astType == AST_LIST_POINT;
  bool astActorMatch = defaultMap.at(key).astType == AST_ACTOR_MATCH;
  bool astCustomWeapon = defaultMap.at(key).astType == AST_LIST_CUSTOM_WEAPON;
  if (auto varNode = dynamic_cast<VariableValueNode *>(input.get())) {
    cerr << "Codegen Error: Cannot find the definition of variable `"
         << varNode->value << "` at " << input->loc
         << ". Please define it as a constant value or function\'s param.\n";
    return "";
  }
  if (astInt) {
    intNode = dynamic_cast<IntValueNode *>(input.get());
    stringNode = dynamic_cast<StringValueNode *>(input.get());
  }
  if (astBool)
    boolNode = dynamic_cast<BoolValueNode *>(input.get());
  if (astString)
    stringNode = dynamic_cast<StringValueNode *>(input.get());
  if (astPointList)
    listNode = dynamic_cast<ListValueNode *>(input.get());
  if (astActorMatch)
    actorMatchNode = dynamic_cast<ActorMatchValueNode *>(input.get());
  if (astCustomWeapon)
    customWeaponNode = dynamic_cast<CustomWeaponValueNode *>(input.get());
  bool codegenBool = defaultMap.at(key).codegenType == CODEGEN_BOOL;
  bool codegenInt = defaultMap.at(key).codegenType == CODEGEN_INT;
  bool codegenString = defaultMap.at(key).codegenType == CODEGEN_STRING;
  bool codegenListPatrol =
      defaultMap.at(key).codegenType == CODEGEN_LIST_PATROL;
  bool codegenListPoint = defaultMap.at(key).codegenType == CODEGEN_LIST_POINT;
  bool codegenCustomWeapon =
      defaultMap.at(key).codegenType == CODEGEN_LIST_CUSTOM_WEAPON;

  auto format = [&](string text) -> string {
    if (codegenInt || codegenBool) {
      return text;
    } else if (codegenString) {
      return "\"" + text + "\"";
    } else if (codegenListPatrol || codegenListPoint || codegenCustomWeapon) {
      if (text != "[]")
        cerr << "Compiler Implementation Error: codegenListPatrol and "
                "codegenListPoint cannot be handled.\n";
      return text;
    } else {
      cerr << "Compiler Implementation Error: Unreachable codegen type.\n";
      return text;
    }
  };

  // Return search result
  if (!input)
    return format(defaultMap.at(key).defaultValue);
  if (intNode)
    return format(std::to_string(intNode->value));
  if (boolNode)
    return format(boolNode->value ? "true" : "false");
  if (stringNode && keywordEnum.isEmpty())
    return format(stringNode->value);
  // Enum Input Geneartion
  if (stringNode && !keywordEnum.isEmpty()) {
    std::pair<bool, string> enumResult =
        keywordEnum.get(stringNode->value, extraEnum);
    if (!enumResult.first)
      cerr << " ->  Found at " << stringNode->loc << "\n";
    return format(enumResult.second);
  }
  cerr << "Compiler Implementation Error: incorrect type conversion at "
       << input->loc << "\n";
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

void DefaultMap::addInputMap(vector<unique_ptr<NamedArgNode>> &namedArgs,
                             vector<string> exclusion) {
  clearInputMap();
  std::set<string> keySet;
  for (auto &namedArg : namedArgs) {
    string key = namedArg->key;
    if (std::find(exclusion.begin(), exclusion.end(), key) != exclusion.end())
      continue;
    if (keySet.count(key))
      cerr << "Codegen Warnings: Redefined key \"" << key << "\" at "
           << namedArg->loc << "\n";
    inputMap.insert({key, std::move(namedArg->expNode->value)});
    keySet.insert(key);
  }
  verifyInputMap();
}