#include "ast.h"
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

bool argBinding(const unique_ptr<ModuleNode> &moduleNode) {
  auto &aliases = moduleNode->aliases;
  for (auto &blockNode : moduleNode->blocks) {
    for (auto &instr : blockNode.get()->actionsNode->instructions) {
      if (instr->positional_args.empty())
        continue;
      string actionName = instr->identifier.front();
      if (aliases.count(actionName) == 0)
        continue;
      // Currently only the alias can map positional arg into named arg
      auto aliasParam = aliases.find(actionName)->second->params;
      if (instr->positional_args.size() > aliasParam.size()) {
        std::cerr << "Syntax Error: Too much positional arguments. Found at "
                  << instr->loc << "\n";
        return false;
      }
      std::vector<std::unique_ptr<NamedArgNode>> bindedArgs;
      for (int i = 0; i < instr->positional_args.size(); i++) {
        auto &posArg = instr->positional_args[i];
        bindedArgs.push_back(std::make_unique<NamedArgNode>(
            aliasParam[i], posArg.get()->valueNode, posArg->loc));
      }
      instr->named_args.insert(instr->named_args.begin(),
                               std::make_move_iterator(bindedArgs.begin()),
                               std::make_move_iterator(bindedArgs.end()));
      instr->positional_args.clear();
    }
  }
  return true;
}
} // namespace transform