#include "ast.h"
#include "transform.h"
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

bool constantFolding(
    std::map<std::string, std::unique_ptr<AliasNode>> &aliases,
    std::vector<std::unique_ptr<InstructionNode>> &instructions) {
  for (auto &instr : instructions)
    for (auto &arg : instr->named_args)
      arg->expNode->foldValue();
  return true;
}

bool constantFolding(const unique_ptr<ModuleNode> &moduleNode) {
  bool ret = true;
  auto &aliases = moduleNode->aliases;
  for (auto &blockNode : moduleNode->blocks) {
    if (blockNode.get()->actionsNode.get())
      ret &= constantFolding(aliases,
                             blockNode.get()->actionsNode.get()->instructions);
    if (blockNode.get()->checksNode.get())
      ret &= constantFolding(aliases,
                             blockNode.get()->checksNode.get()->instructions);
    if (blockNode.get()->triggersNode.get())
      ret &= constantFolding(aliases,
                             blockNode.get()->triggersNode.get()->instructions);
  }
  return ret;
}
} // namespace transform