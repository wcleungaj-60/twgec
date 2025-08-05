#include "ast.h"
#include "location.h"
#include "transform.h"
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

bool throwRedefinitionError(std::string name, Location loc1, Location loc2) {
  std::cerr << "Compilation Error: Redefinition of `" << name << "` found at "
            << loc1 << " and " << loc2 << "\n";
  return false;
}

bool redefinitionChecking(
    const unique_ptr<ModuleNode> &moduleNode,
    std::map<std::string, std::unique_ptr<ConstDefNode>> &constDefMap,
    std::map<std::string, std::unique_ptr<FunDefNode>> &funDefMap,
    std::map<std::string, std::unique_ptr<BlockNode>> &blockMap) {
  for (auto &constDef : moduleNode->constDefs) {
    if (constDefMap.count(constDef->key) != 0)
      return throwRedefinitionError(
          constDef->key, constDefMap.at(constDef->key)->loc, constDef->loc);
    constDefMap.insert({constDef->key, constDef->clone()});
  }
  for (auto &funDef : moduleNode->funDefs) {
    if (constDefMap.count(funDef->identifier) != 0)
      return throwRedefinitionError(funDef->identifier,
                                    constDefMap.at(funDef->identifier)->loc,
                                    funDef->loc);
    if (funDefMap.count(funDef->identifier) != 0)
      return throwRedefinitionError(funDef->identifier,
                                    funDefMap.at(funDef->identifier)->loc,
                                    funDef->loc);
    funDefMap.insert({funDef->identifier, funDef->clone()});
  }
  for (auto &block : moduleNode->blocks) {
    if (constDefMap.count(block->identifier) != 0)
      return throwRedefinitionError(block->identifier,
                                    constDefMap.at(block->identifier)->loc,
                                    block->loc);
    if (funDefMap.count(block->identifier) != 0)
      return throwRedefinitionError(
          block->identifier, funDefMap.at(block->identifier)->loc, block->loc);
    if (blockMap.count(block->identifier) != 0)
      return throwRedefinitionError(
          block->identifier, blockMap.at(block->identifier)->loc, block->loc);
    blockMap.insert({block->identifier, block->clone()});
  }
  return true;
}

bool symbolChecking(const unique_ptr<ModuleNode> &moduleNode) {
  std::map<std::string, std::unique_ptr<ConstDefNode>> constDefMap;
  std::map<std::string, std::unique_ptr<FunDefNode>> funDefMap;
  std::map<std::string, std::unique_ptr<BlockNode>> blockMap;
  if (!redefinitionChecking(moduleNode, constDefMap, funDefMap, blockMap))
    return false;
  return true;
}
} // namespace transform