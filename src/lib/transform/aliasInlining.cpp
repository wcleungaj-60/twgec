#include "ast.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>
namespace transform {

using std::set;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

bool isValidAliasCaller(std::unique_ptr<InstructionNode> &action,
                        unordered_map<string, unique_ptr<AliasNode>> &aliases) {
  AliasNode *alias = aliases[action->identifier].get();
  auto aliasParamNum = alias->params.size();
  auto actionArgNum = action->named_args.size();
  set<string> paramSet;
  set<string> argSet;
  if (aliasParamNum != actionArgNum) {
    std::cerr << "Syntax Error: Unmatched alias parameters number. "
              << aliasParamNum << " parameters are expected at " << alias->loc
              << ". " << actionArgNum << " paramters are found at "
              << action->loc << ".\n";
    return false;
  }
  std::unordered_map<string, unique_ptr<ValueNode>> namedArgsMap;
  for (string param : alias->params)
    paramSet.insert(param);
  for (auto &namedArg : action->named_args) {
    namedArgsMap.insert({namedArg->key, namedArg->valueNode->clone()});
    argSet.insert(namedArg->key);
  }
  if (paramSet != argSet) {
    std::cerr << "Syntax Error: Unmatched paramter naming at " << alias->loc
              << "(alias definition) and " << action->loc
              << "(alias application).\n";
    return false;
  }
  for (auto &instr : alias->instructions)
    for (auto &arg : instr->named_args)
      if (auto varNode =
              dynamic_cast<VariableValueNode *>(arg->valueNode.get()))
        if (!paramSet.count(varNode->value)) {
          std::cerr << "Syntax Error: Unknown variable at " << varNode->loc
                    << ".\n";
          return false;
        }

  return true;
}

bool aliasInling(const unique_ptr<ModuleNode> &moduleNode) {
  auto &aliases = moduleNode->aliases;
  for (auto &blockNode : moduleNode->blocks) {
    if(!blockNode.get()->actionsNode.get())
      continue;
    auto &blockInstrs = blockNode.get()->actionsNode->instructions;
    std::vector<int> aliasIdxes;
    // Step 1: Get the Alias Index
    for (int idx = 0; idx < blockInstrs.size(); idx++) {
      auto &action = blockInstrs[idx];
      string actionName = action->identifier;
      if (aliases.count(actionName) == 0)
        continue;
      AliasNode *alias = aliases[actionName].get();
      if (!isValidAliasCaller(action, aliases))
        return false;
      aliasIdxes.push_back(idx);
    }
    std::reverse(aliasIdxes.begin(), aliasIdxes.end());
    // Step 2: Inline the Alias
    for (auto idx : aliasIdxes) {
      std::unordered_map<std::string, unique_ptr<ValueNode>> callerMap;
      auto &callerInstr = blockInstrs[idx];
      auto pos = blockInstrs.begin() + idx;
      auto &aliasNode = aliases[callerInstr->identifier];
      for (auto &arg : callerInstr->named_args)
        callerMap.insert({arg->key, valueNodeClone(arg->valueNode)});
      blockInstrs.erase(pos);
      for (auto &aliasIns : aliasNode.get()->instructions) {
        auto clonedIns = aliasIns->clone();
        for (auto &arg : clonedIns->named_args)
          if (auto *varNode =
                  dynamic_cast<VariableValueNode *>(arg->valueNode.get()))
            arg->valueNode = valueNodeClone(callerMap[varNode->value]);
        blockInstrs.insert(pos, std::move(clonedIns));
      }
    }
  }
  aliases.clear();
  return true;
}
} // namespace transform