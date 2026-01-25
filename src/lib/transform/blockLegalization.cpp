#include "ast.h"
#include "transform.h"
#include <set>
namespace transform {

bool blockLegalization(const std::unique_ptr<ModuleNode> &moduleNode,
                       PassConfig config) {
  for (auto &blockNode : moduleNode->blocks) {
    auto &blockBody = blockNode->blockBody;
    std::set<FunDefType> instrSetType;
    for (auto &typedInstrSet : blockBody->typedInstrSets) {
      auto type = typedInstrSet->type;
      if (instrSetType.count(type)) {
        std::cerr << "Syntax Error: Redefinition of `" << type
                  << "` inside a block. Please unify them into one `" << type
                  << "`. Found at " << typedInstrSet->loc << ".\n";
        return false;
      } else {
        instrSetType.insert(type);
      }
    }
    if (!instrSetType.count(FUN_DEF_TYPE_ACTIONS)) {
      blockBody->typedInstrSets.push_back(std::make_unique<TypedInstrSetNode>(
          blockNode->loc, FUN_DEF_TYPE_ACTIONS));
    }
    if (!instrSetType.count(FUN_DEF_TYPE_CHECKS)) {
      blockBody->typedInstrSets.push_back(std::make_unique<TypedInstrSetNode>(
          blockNode->loc, FUN_DEF_TYPE_CHECKS));
    }
    if (!instrSetType.count(FUN_DEF_TYPE_TRIGGERS)) {
      blockBody->typedInstrSets.push_back(std::make_unique<TypedInstrSetNode>(
          blockNode->loc, FUN_DEF_TYPE_TRIGGERS));
    }
    for (int i = 0; i < blockBody->typedInstrSets.size(); i++) {
      auto type = blockBody->typedInstrSets[i]->type;
      switch (type) {
      case FUN_DEF_TYPE_ACTIONS:
        blockBody->setActionsIdx(i);
        break;
      case FUN_DEF_TYPE_CHECKS:
        blockBody->setChecksIdx(i);
        break;
      case FUN_DEF_TYPE_TRIGGERS:
        blockBody->setTriggersIdx(i);
        break;
      default:
        break;
      }
    }
  }
  return true;
}
} // namespace transform