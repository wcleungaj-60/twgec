#include "ast.h"
#include "transform.h"
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

bool argBinding(std::map<std::string, std::unique_ptr<FunDefNode>> &funDefs,
                std::vector<std::unique_ptr<CompositeInstrNode>> &compositeInstrs) {
  for (auto &compositeInstr : compositeInstrs) {
    auto& instr = compositeInstr->instruction;
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
  auto &funDefs = moduleNode->funDefs;
  for (auto &blockNode : moduleNode->blocks) {
    for(auto &typedInstrSet: blockNode->typedInstrSets) {
      if(!argBinding(funDefs, typedInstrSet->instrSet->instructions))
        return false;
    }
  }
  return true;
}
} // namespace transform