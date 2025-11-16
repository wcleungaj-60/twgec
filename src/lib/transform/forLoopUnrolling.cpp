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

PropagationResult forLoopUnrolling(std::unique_ptr<InstrSetNode> &instrSet) {
  std::vector<int> forIdxes;
  auto &compositeInstrs = instrSet->instructions;
  // Step 1: Get the for loop node index
  for (int idx = 0; idx < compositeInstrs.size(); idx++)
    if (compositeInstrs[idx]->forNode)
      forIdxes.push_back(idx);
  if (forIdxes.empty())
    return PropagationResult::UNCHANGED;
  std::reverse(forIdxes.begin(), forIdxes.end());
  for (int idx : forIdxes) {
    // Step 2: Verify the for loop
    auto &forNode = compositeInstrs[idx]->forNode;
    auto pos = compositeInstrs.begin() + idx;
    if (!forNode->fromExp->isValue ||
        !dynamic_cast<IntValueNode *>(forNode->fromExp->value.get())) {
      std::cerr << "Syntax Error: Integer value is expected in the for "
                   "statment range. Found at "
                << forNode->fromExp->loc << ".\n";
      return PropagationResult::ERROR;
    }
    if (!forNode->toExp->isValue ||
        !dynamic_cast<IntValueNode *>(forNode->toExp->value.get())) {
      std::cerr << "Syntax Error: Integer value is expected in the for "
                   "statment range. Found at "
                << forNode->toExp->loc << ".\n";
      return PropagationResult::ERROR;
    }
    int fromValue =
        dynamic_cast<IntValueNode *>(forNode->fromExp->value.get())->value;
    int toValue =
        dynamic_cast<IntValueNode *>(forNode->toExp->value.get())->value;
    // Step 3: Get the region to be inserted
    Location loc = forNode->loc;
    std::unique_ptr<InstrSetNode> regionToInsert =
        std::make_unique<InstrSetNode>(loc);
    int step = fromValue < toValue ? 1 : -1;
    int times = std::abs(fromValue - toValue) + 1;
    while (times > 0) {
      auto iterRegion = forNode->region->clone();
      auto iterValue = std::make_unique<IntValueNode>(fromValue, loc);
      std::map<std::string, std::unique_ptr<ExpressionNode>> constDefMap;
      constDefMap.insert({forNode->iterArg,
                          ExpressionNode(std::move(iterValue), loc).clone()});
      iterRegion->propagateExp(constDefMap);
      iterRegion->foldValue();
      fromValue += step;
      times--;
      std::move(iterRegion->instructions.begin(),
                iterRegion->instructions.end(),
                std::back_inserter(regionToInsert->instructions));
    }
    // Step 4: Insert the regions
    compositeInstrs.erase(pos);
    compositeInstrs.insert(
        pos, std::make_move_iterator(regionToInsert->instructions.begin()),
        std::make_move_iterator(regionToInsert->instructions.end()));
  }
  return PropagationResult::PROPAGATED;
}

bool forLoopUnrolling(const unique_ptr<ModuleNode> &moduleNode) {
  // TODO: Remove duplicated logic
  std::map<std::string, std::unique_ptr<ExpressionNode>> constDefMap;
  for (auto &constDef : moduleNode->constDefs)
    constDefMap.insert({constDef->key, constDef->expNode->clone()});
  moduleNode->propagateExp(constDefMap);
  moduleNode->foldValue();
  moduleNode->constDefs.clear();
  PropagationResult result = PropagationResult::PROPAGATED;
  while (result == PropagationResult::PROPAGATED) {
    result = PropagationResult::UNCHANGED;
    for (auto &blockNode : moduleNode->blocks)
      for (auto &typedInstrSet : blockNode->blockBody->typedInstrSets) {
        auto blockResult = forLoopUnrolling(typedInstrSet->instrSet);
        if (blockResult == PropagationResult::ERROR)
          return false;
        if (blockResult == PropagationResult::PROPAGATED)
          result = PropagationResult::PROPAGATED;
      }
  }
  return true;
}
} // namespace transform