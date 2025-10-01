#include "ast.h"
#include "option/option.h"
namespace transform {

#define TRANSFORM_WITH_PRINT(passFunc, passName)                               \
  if (opt.printASTBefore) {                                                    \
    moduleNode->print(std::string("AST Before ") + passName);                  \
  }                                                                            \
  if (!passFunc(std::move(moduleNode))) {                                      \
    return false;                                                              \
  }                                                                            \
  if (opt.printASTAfter) {                                                     \
    moduleNode->print(std::string("AST After ") + passName);                   \
  }

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
  TRANSFORM_WITH_PRINT(symbolChecking, "Symbol Checking");
  TRANSFORM_WITH_PRINT(argBinding, "Arg Binding");
  TRANSFORM_WITH_PRINT(blockInling, "Block Inling");
  TRANSFORM_WITH_PRINT(blockLegalization, "Block Legalization");
  TRANSFORM_WITH_PRINT(functionInling, "Function Inlining");
  TRANSFORM_WITH_PRINT(constantFolding, "Constant Folding");
  TRANSFORM_WITH_PRINT(ifStatementPropagation, "Statement Propagation");
  TRANSFORM_WITH_PRINT(implicitListPromotion, "Implicit List Promotion");
  if (opt.printASTBefore)
    moduleNode->print("AST Before Code Generation");
  return true;
}

} // namespace transform