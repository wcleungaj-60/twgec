#include "ast.h"
#include "transform.h"
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

bool constantFolding(const unique_ptr<ModuleNode> &moduleNode) {
  std::map<std::string, std::unique_ptr<ExpressionNode>> constDefMap;
  for (auto &constDef : moduleNode->constDefs)
    constDefMap.insert({constDef->key, constDef->expNode->clone()});
  for (auto &blockNode : moduleNode->blocks)
    for (auto &typedInstrSet : blockNode->typedInstrSets) {
      typedInstrSet->instrSet->propagateExp(constDefMap);
      typedInstrSet->instrSet->foldValue();
    }
  return true;
}
} // namespace transform