#include "ast.h"
#include "transform.h"
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

bool constantFolding(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &constDefs,
    const unique_ptr<InstrSetNode> &instrSet) {
  for (auto &compositeInstr : instrSet->instructions) {
    if (compositeInstr->instruction) {
      for (auto &arg : compositeInstr->instruction->named_args) {
        arg->expNode->propagateVarToExp(constDefs);
        arg->expNode->foldValue();
      }
    } else if (compositeInstr->ifStatement) {
      compositeInstr->ifStatement->condition->propagateVarToExp(constDefs);
      compositeInstr->ifStatement->condition->foldValue();
      constantFolding(constDefs, compositeInstr->ifStatement->trueBlock);
    }
  }
  return true;
}

bool constantFolding(const unique_ptr<ModuleNode> &moduleNode) {
  std::map<std::string, std::unique_ptr<ExpressionNode>> constDefMap;
  for (auto &constDef : moduleNode->constDefs)
    constDefMap.insert({constDef->key, constDef->expNode->clone()});
  for (auto &blockNode : moduleNode->blocks)
    for (auto &typedInstrSet : blockNode->typedInstrSets)
      if (!constantFolding(constDefMap, typedInstrSet->instrSet))
        return false;
  return true;
}
} // namespace transform