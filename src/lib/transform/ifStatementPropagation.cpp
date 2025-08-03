#include "ast.h"
#include "transform.h"
#include <algorithm>
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

bool ifStatementPropagation(std::unique_ptr<InstrSetNode> &instrSet) {
  std::vector<int> ifStatementIdxes;
  // Step 1: Get the if-statement index
  for (int idx = 0; idx < instrSet->instructions.size(); idx++) {
    if (!instrSet->instructions[idx]->ifStatement)
      continue;
    auto &condition = instrSet->instructions[idx]->ifStatement->condition;
    if (!condition->isValue ||
        !dynamic_cast<BoolValueNode *>(condition->value.get())) {
      std::cerr << "Syntax Error: Boolean value is expected in the if statment "
                   "condition. Found at "
                << instrSet->instructions[idx]->loc << ".\n";
      return false;
    }
    ifStatementIdxes.push_back(idx);
  }
  if (ifStatementIdxes.empty())
    return true;
  std::reverse(ifStatementIdxes.begin(), ifStatementIdxes.end());
  // Step 2: Flatten the if-statement
  for (int idx : ifStatementIdxes) {
    auto &ifStatement = instrSet->instructions[idx]->ifStatement;
    auto pos = instrSet->instructions.begin() + idx;
    auto ifTrue =
        dynamic_cast<BoolValueNode *>(ifStatement->condition->value.get())
            ->value;
    auto clonedTrueBlock = ifStatement->trueBlock->clone();
    instrSet->instructions.erase(pos);
    if (ifTrue) {
      for (auto it = clonedTrueBlock->instructions.rbegin();
           it != clonedTrueBlock->instructions.rend(); it++) {
        auto clonedInstr = std::make_unique<CompositeInstrNode>(
            it->get()->loc, it->get()->instruction->clone());
        instrSet->instructions.insert(pos, std::move(clonedInstr));
      }
    }
  }
  return true;
}

bool ifStatementPropagation(const unique_ptr<ModuleNode> &moduleNode) {
  for (auto &blockNode : moduleNode->blocks)
    for (auto &typedInstrSet : blockNode->blockBody->typedInstrSets)
      if (!ifStatementPropagation(typedInstrSet->instrSet))
        return false;
  return true;
}
} // namespace transform