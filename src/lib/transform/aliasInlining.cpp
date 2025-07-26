#include "ast.h"
#include "transform.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
namespace transform {

using std::map;
using std::set;
using std::string;
using std::unique_ptr;
using std::vector;

bool isValidAliasCaller(std::unique_ptr<InstructionNode> &caller,
                        map<string, unique_ptr<AliasNode>> &aliases) {
  AliasNode *alias = aliases[caller->identifier].get();
  auto aliasParamNum = alias->params.size();
  auto callerArgNum = caller->named_args.size();
  set<string> paramSet;
  set<string> argSet;
  if (aliasParamNum != callerArgNum) {
    std::cerr << "Syntax Error: Unmatched alias parameters number. "
              << aliasParamNum << " parameters are expected at " << alias->loc
              << ". " << callerArgNum << " paramters are found at "
              << caller->loc << ".\n";
    return false;
  }
  std::map<string, unique_ptr<ExpressionNode>> namedArgsMap;
  for (string param : alias->params)
    paramSet.insert(param);
  for (auto &namedArg : caller->named_args) {
    namedArgsMap.insert({namedArg->key, namedArg->expNode->clone()});
    argSet.insert(namedArg->key);
  }
  if (paramSet != argSet) {
    std::cerr << "Syntax Error: Unmatched paramter naming at " << alias->loc
              << "(alias definition) and " << caller->loc
              << "(alias application).\n";
    return false;
  }

  return true;
}

bool aliasInling(std::map<std::string, std::unique_ptr<AliasNode>> &aliases,
                 std::vector<std::unique_ptr<InstructionNode>> &instructions) {
  std::vector<int> aliasIdxes;
  // Step 1: Get the Alias Index
  for (int idx = 0; idx < instructions.size(); idx++) {
    auto &caller = instructions[idx];
    string callerName = caller->identifier;
    if (aliases.count(callerName) == 0)
      continue;
    AliasNode *alias = aliases[callerName].get();
    if (!isValidAliasCaller(caller, aliases))
      return false;
    aliasIdxes.push_back(idx);
  }
  std::reverse(aliasIdxes.begin(), aliasIdxes.end());
  // Step 2: Inline the Alias
  for (auto idx : aliasIdxes) {
    std::map<std::string, unique_ptr<ValueNode>> callerMap;
    auto &callerInstr = instructions[idx];
    auto pos = instructions.begin() + idx;
    auto aliasNode = aliases[callerInstr->identifier]->clone();
    for (auto &arg : callerInstr->named_args) {
      if (!arg->expNode->isValue) {
        std::cerr
            << "Alias caller doesn\'t support an expression as its argument. "
               "Found at "
            << aliasNode->loc << "\n";
        return false;
      }
      callerMap.insert({arg->key, arg->expNode->value->clone()});
    }
    instructions.erase(pos);
    for (auto it = aliasNode.get()->instructions.rbegin();
         it != aliasNode.get()->instructions.rend(); ++it) {
      auto clonedIns = it->get()->clone();
      for (auto &arg : clonedIns->named_args)
        arg->expNode->propagateAliasParam(callerMap);
      instructions.insert(pos, std::move(clonedIns));
    }
  }
  return true;
}

bool aliasInling(const unique_ptr<ModuleNode> &moduleNode) {
  bool ret = true;
  auto &aliases = moduleNode->aliases;
  for (auto &blockNode : moduleNode->blocks) {
    if (blockNode.get()->actionsNode.get())
      ret &= aliasInling(aliases, blockNode.get()->actionsNode->instructions);
    if (blockNode.get()->checksNode.get())
      ret &= aliasInling(aliases, blockNode.get()->checksNode->instructions);
    if (blockNode.get()->triggersNode.get())
      ret &= aliasInling(aliases, blockNode.get()->triggersNode->instructions);
  }
  aliases.clear();
  return ret;
}
} // namespace transform