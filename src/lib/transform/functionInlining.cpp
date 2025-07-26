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

bool isValidFuncCaller(std::unique_ptr<InstructionNode> &caller,
                       map<string, unique_ptr<FunDefNode>> &funDefMap,
                       FunDefType expectedType) {
  FunDefNode *funDef = funDefMap[caller->identifier].get();
  if (funDef->type != expectedType) {
    std::cerr << "Syntax Error: Unmatched function type. " << funDef->type
              << "-typed function cannot be called inside the " << expectedType
              << " scope. Found at " << caller->loc << ".\n";
    return false;
  }
  auto funParamNum = funDef->params.size();
  auto callerArgNum = caller->named_args.size();
  set<string> paramSet;
  set<string> argSet;
  if (funParamNum != callerArgNum) {
    std::cerr << "Syntax Error: Unmatched function parameters number. "
              << funParamNum << " parameters are expected at " << funDef->loc
              << ". " << callerArgNum << " paramters are found at "
              << caller->loc << ".\n";
    return false;
  }
  std::map<string, unique_ptr<ExpressionNode>> namedArgsMap;
  for (string param : funDef->params)
    paramSet.insert(param);
  for (auto &namedArg : caller->named_args) {
    namedArgsMap.insert({namedArg->key, namedArg->expNode->clone()});
    argSet.insert(namedArg->key);
  }
  if (paramSet != argSet) {
    std::cerr << "Syntax Error: Unmatched paramter naming at " << funDef->loc
              << "(function definition) and " << caller->loc
              << "(function application).\n";
    return false;
  }

  return true;
}

bool functionInling(
    std::map<std::string, std::unique_ptr<FunDefNode>> &funDefMap,
    std::vector<std::unique_ptr<InstructionNode>> &instructions,
    FunDefType expectedType) {
  std::vector<int> funCallerIdxes;
  // Step 1: Get the function caller index
  for (int idx = 0; idx < instructions.size(); idx++) {
    auto &caller = instructions[idx];
    string callerName = caller->identifier;
    if (funDefMap.count(callerName) == 0)
      continue;
    FunDefNode *funDef = funDefMap[callerName].get();
    if (!isValidFuncCaller(caller, funDefMap, expectedType))
      return false;
    funCallerIdxes.push_back(idx);
  }
  std::reverse(funCallerIdxes.begin(), funCallerIdxes.end());
  // Step 2: Inline the function
  for (auto idx : funCallerIdxes) {
    std::map<std::string, unique_ptr<ExpressionNode>> callerMap;
    auto &callerInstr = instructions[idx];
    auto pos = instructions.begin() + idx;
    auto funDefNode = funDefMap[callerInstr->identifier]->clone();
    for (auto &arg : callerInstr->named_args)
      callerMap.insert({arg->key, std::move(arg->expNode)});
    instructions.erase(pos);
    for (auto it = funDefNode.get()->instructions.rbegin();
         it != funDefNode.get()->instructions.rend(); ++it) {
      auto clonedIns = it->get()->clone();
      for (auto &arg : clonedIns->named_args)
        arg->expNode->propagateVarToExp(callerMap);
      instructions.insert(pos, std::move(clonedIns));
    }
  }
  return true;
}

bool functionInling(const unique_ptr<ModuleNode> &moduleNode) {
  bool ret = true;
  auto &funDefs = moduleNode->funDefs;
  for (auto &blockNode : moduleNode->blocks) {
    if (blockNode.get()->actionsNode.get())
      ret &= functionInling(funDefs, blockNode.get()->actionsNode->instructions,
                            FUN_DEF_TYPE_ACTIONS);
    if (blockNode.get()->checksNode.get())
      ret &= functionInling(funDefs, blockNode.get()->checksNode->instructions,
                            FUN_DEF_TYPE_CHECKS);
    if (blockNode.get()->triggersNode.get())
      ret &=
          functionInling(funDefs, blockNode.get()->triggersNode->instructions,
                         FUN_DEF_TYPE_TRIGGERS);
  }
  funDefs.clear();
  return ret;
}
} // namespace transform