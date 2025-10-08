#include "ast.h"
#include "transform.h"
#include <algorithm>
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

namespace {
enum PropagationResult { ERROR = 0, PROPAGATED = 1, UNCHANGED = 2 };
}

PropagationResult
ifStatementPropagation(std::unique_ptr<InstrSetNode> &instrSet) {
  std::vector<int> ifStatementIdxes;
  auto &compositeInstrs = instrSet->instructions;
  // Step 1: Get the if-statement index
  for (int idx = 0; idx < compositeInstrs.size(); idx++) {
    if (!compositeInstrs[idx]->ifStatement)
      continue;
    auto &condition = compositeInstrs[idx]->ifStatement->condition;
    if (!condition->isValue ||
        !dynamic_cast<BoolValueNode *>(condition->value.get())) {
      std::cerr << "Syntax Error: Boolean value is expected in the if statment "
                   "condition. Found at "
                << compositeInstrs[idx]->loc << ".\n";
      return PropagationResult::ERROR;
    }
    ifStatementIdxes.push_back(idx);
  }
  if (ifStatementIdxes.empty())
    return PropagationResult::UNCHANGED;
  std::reverse(ifStatementIdxes.begin(), ifStatementIdxes.end());
  // Step 2: Flatten the if-statement
  for (int idx : ifStatementIdxes) {
    auto &ifStatement = compositeInstrs[idx]->ifStatement;
    auto pos = compositeInstrs.begin() + idx;
    auto ifTrue =
        dynamic_cast<BoolValueNode *>(ifStatement->condition->value.get())
            ->value;
    auto clonedTrueBlock = ifStatement->trueBlock->clone();
    compositeInstrs.erase(pos);
    if (ifTrue)
      compositeInstrs.insert(
          pos, std::make_move_iterator(clonedTrueBlock->instructions.begin()),
          std::make_move_iterator(clonedTrueBlock->instructions.end()));
  }
  return PropagationResult::PROPAGATED;
}

bool ifStatementPropagation(const unique_ptr<ModuleNode> &moduleNode) {
  PropagationResult result = PropagationResult::PROPAGATED;
  while (result == PropagationResult::PROPAGATED) {
    result = PropagationResult::UNCHANGED;
    for (auto &blockNode : moduleNode->blocks)
      for (auto &typedInstrSet : blockNode->blockBody->typedInstrSets) {
        auto blockResult = ifStatementPropagation(typedInstrSet->instrSet);
        if (blockResult == PropagationResult::ERROR)
          return false;
        if (blockResult == PropagationResult::PROPAGATED)
          result = PropagationResult::PROPAGATED;
      }
  }
  return true;
}
} // namespace transform