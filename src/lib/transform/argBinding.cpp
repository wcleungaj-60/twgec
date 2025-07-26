#include "ast.h"
#include "transform.h"
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

bool argBinding(std::map<std::string, std::unique_ptr<FunDefNode>> &funDefs,
                std::vector<std::unique_ptr<InstructionNode>> &instructions) {
  for (auto &instr : instructions) {
    if (instr->positional_args.empty())
      continue;
    if (funDefs.count(instr->identifier) == 0)
      continue;
    // Currently only the function can map positional arg into named arg
    auto funParam = funDefs.find(instr->identifier)->second->params;
    if (instr->positional_args.size() > funParam.size()) {
      std::cerr << "Syntax Error: Too much positional arguments. Found at "
                << instr->loc << "\n";
      return false;
    }
    std::vector<std::unique_ptr<NamedArgNode>> bindedArgs;
    for (int i = 0; i < instr->positional_args.size(); i++) {
      auto &posArg = instr->positional_args[i];
      bindedArgs.push_back(std::make_unique<NamedArgNode>(
          funParam[i], posArg.get()->expNode, posArg->loc));
    }
    instr->named_args.insert(instr->named_args.begin(),
                             std::make_move_iterator(bindedArgs.begin()),
                             std::make_move_iterator(bindedArgs.end()));
    instr->positional_args.clear();
  }
  return true;
}

bool argBinding(const unique_ptr<ModuleNode> &moduleNode) {
  bool ret = true;
  auto &funDefs = moduleNode->funDefs;
  for (auto &blockNode : moduleNode->blocks) {
    if (blockNode.get()->actionsNode.get())
      ret &=
          argBinding(funDefs, blockNode.get()->actionsNode.get()->instructions);
    if (blockNode.get()->checksNode.get())
      ret &=
          argBinding(funDefs, blockNode.get()->checksNode.get()->instructions);
    if (blockNode.get()->triggersNode.get())
      ret &= argBinding(funDefs,
                        blockNode.get()->triggersNode.get()->instructions);
  }
  return ret;
}
} // namespace transform