#include "ast.h"
#include "option/option.h"
#include <algorithm>

namespace transform {
namespace {
#define RUN_PASS(passFunc, passName)                                           \
  if (!runPass(passFunc, passName, moduleNode, opt))                           \
    return false;

bool runPass(bool (*passFunc)(const std::unique_ptr<ModuleNode> &),
             std::string passName,
             const std::unique_ptr<ModuleNode> &moduleNode, Option opt) {
  if (!opt.runOnly.empty() &&
      std::find(opt.runOnly.begin(), opt.runOnly.end(),
                std::string(passName)) == opt.runOnly.end())
    return true;
  // Print Before
  if (opt.printASTBefore ||
      std::find(opt.printASTBeforeOnly.begin(), opt.printASTBeforeOnly.end(),
                std::string(passName)) != opt.printASTBeforeOnly.end())
    moduleNode->print(std::string("AST Before ") + passName);
  // Pass Execution
  if (!passFunc(std::move(moduleNode)))
    return false;
  // Print After
  if (opt.printASTAfter ||
      std::find(opt.printASTAfterOnly.begin(), opt.printASTAfterOnly.end(),
                std::string(passName)) != opt.printASTAfterOnly.end())
    moduleNode->print(std::string("AST After ") + passName);
  return true;
}
} // namespace

// Symbol redefinition or use before define will be checked
bool symbolChecking(const std::unique_ptr<ModuleNode> &moduleNode);
// Positional Arg will be converted into Named Arg
bool argBinding(const std::unique_ptr<ModuleNode> &moduleNode);
// The block function will be inlined
bool blockInling(const std::unique_ptr<ModuleNode> &moduleNode);
// Number of actions, checks, and triggers in a block will be checked
bool blockLegalization(const std::unique_ptr<ModuleNode> &moduleNode);
// The actions, checks, and triggers function will be inlined
bool functionInling(const std::unique_ptr<ModuleNode> &moduleNode);
// Propagate the constant and fold the constant
bool constantFolding(const std::unique_ptr<ModuleNode> &moduleNode);
// Flatten the if-statement and remove the dead condition branch
bool ifStatementPropagation(const std::unique_ptr<ModuleNode> &moduleNode);
// Promote type `A` to `list[A]` implicitly
bool implicitListPromotion(const std::unique_ptr<ModuleNode> &moduleNode);

inline bool loweringPipeline(const std::unique_ptr<ModuleNode> &moduleNode,
                             Option opt) {
  RUN_PASS(symbolChecking, "symbolChecking");
  RUN_PASS(argBinding, "argBinding");
  RUN_PASS(blockInling, "blockInling");
  RUN_PASS(blockLegalization, "blockLegalization");
  RUN_PASS(functionInling, "functionInling");
  RUN_PASS(constantFolding, "constantFolding");
  RUN_PASS(ifStatementPropagation, "ifStatementPropagation");
  RUN_PASS(implicitListPromotion, "implicitListPromotion");
  if (opt.printASTBefore)
    moduleNode->print("AST Before Code Generation");
  return true;
}

} // namespace transform