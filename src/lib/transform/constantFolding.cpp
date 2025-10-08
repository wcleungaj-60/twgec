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
  moduleNode->propagateExp(constDefMap);
  moduleNode->foldValue();
  moduleNode->constDefs.clear();
  return !moduleNode->hasUnresolvedValue();
}
} // namespace transform