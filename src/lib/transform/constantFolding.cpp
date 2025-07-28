#include "ast.h"
#include "transform.h"
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

bool constantFolding(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &constDefs,
    std::vector<std::unique_ptr<CompositeInstrNode>> &compositeInstrs) {
  for (auto &compositeInstr : compositeInstrs)
    for (auto &arg : compositeInstr->instruction->named_args){
      arg->expNode->propagateVarToExp(constDefs);
      arg->expNode->foldValue();
  }
  return true;
}

bool constantFolding(const unique_ptr<ModuleNode> &moduleNode) {
  std::map<std::string, std::unique_ptr<ExpressionNode>> constDefMap;
  for (auto &constDef : moduleNode->constDefs)
    constDefMap.insert({constDef->key, constDef->expNode->clone()});
  for (auto &blockNode : moduleNode->blocks)
    for (auto &typedInstrSet : blockNode->typedInstrSets)
      if (!constantFolding(constDefMap, typedInstrSet->instrSet->instructions))
        return false;
  return true;
}
} // namespace transform