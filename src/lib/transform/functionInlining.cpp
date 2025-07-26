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
    std::vector<std::unique_ptr<InstrSetItemNode>> &InstrSetItems,
    FunDefType expectedType) {
  std::vector<int> funCallerIdxes;
  // Step 1: Get the function caller index
  for (int idx = 0; idx < InstrSetItems.size(); idx++) {
    std::unique_ptr<InstructionNode> &caller = InstrSetItems[idx]->instruction;
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
  for (int idx : funCallerIdxes) {
    std::map<std::string, unique_ptr<ExpressionNode>> callerMap;
    auto &callerInstr = InstrSetItems[idx]->instruction;
    auto pos = InstrSetItems.begin() + idx;
    auto funDefNode = funDefMap[callerInstr->identifier]->clone();
    for (auto &arg : callerInstr->named_args)
      callerMap.insert({arg->key, std::move(arg->expNode)});
    InstrSetItems.erase(pos);
    for (auto it = funDefNode.get()->instructions.rbegin();
         it != funDefNode.get()->instructions.rend(); ++it) {
      auto clonedIns = std::make_unique<InstrSetItemNode>(it->get()->loc,
                                                          it->get()->clone());
      for (auto &arg : clonedIns->instruction->named_args)
        arg->expNode->propagateVarToExp(callerMap);
      InstrSetItems.insert(pos, std::move(clonedIns));
    }
  }
  return true;
}

bool functionInling(const unique_ptr<ModuleNode> &moduleNode) {
  bool ret = true;
  auto &funDefs = moduleNode->funDefs;
  for (auto &blockNode : moduleNode->blocks) {
    std::set<FunDefType> instrSetType;
    for (auto &typedInstrSet : blockNode->typedInstrSets) {
      auto type = typedInstrSet->type;
      if (!functionInling(funDefs, typedInstrSet->instrSet->instructions, type))
        return false;
      // TODO: Should be done in the new transformation pass `block-legalizer`.
      if (instrSetType.count(type)) {
        std::cerr << "Syntax Error: Redefinition of `" << type
                  << "` inside a block. Please unify them into one `" << type
                  << "`. Found at " << typedInstrSet->loc << ".\n";
        return false;
      } else {
        instrSetType.insert(type);
      }
    }
    // TODO: Should be done in the new transformation pass `block-legalizer`.
    if (!instrSetType.count(FUN_DEF_TYPE_ACTIONS)) {
      blockNode->typedInstrSets.push_back(std::make_unique<TypedInstrSetNode>(
          blockNode->loc, FUN_DEF_TYPE_ACTIONS));
    }
    if (!instrSetType.count(FUN_DEF_TYPE_CHECKS)) {
      blockNode->typedInstrSets.push_back(std::make_unique<TypedInstrSetNode>(
          blockNode->loc, FUN_DEF_TYPE_CHECKS));
    }
    if (!instrSetType.count(FUN_DEF_TYPE_TRIGGERS)) {
      blockNode->typedInstrSets.push_back(std::make_unique<TypedInstrSetNode>(
          blockNode->loc, FUN_DEF_TYPE_TRIGGERS));
    }
  }
  funDefs.clear();
  return ret;
}
} // namespace transform