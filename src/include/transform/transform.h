#include "ast.h"
#include "option/option.h"
#include <algorithm>
#include <functional>

namespace transform {

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
// Unroll for loop
bool forLoopUnrolling(const std::unique_ptr<ModuleNode> &moduleNode);
// Flatten the if-statement and remove the dead condition branch
bool ifStatementPropagation(const std::unique_ptr<ModuleNode> &moduleNode);
// Promote type `A` to `list[A]` implicitly
bool implicitListPromotion(const std::unique_ptr<ModuleNode> &moduleNode);

namespace pass {
const std::string symbolChecking = "symbolChecking";
const std::string argBinding = "argBinding";
const std::string blockInling = "blockInling";
const std::string blockLegalization = "blockLegalization";
const std::string functionInling = "functionInling";
const std::string forLoopUnrolling = "forLoopUnrolling";
const std::string constantFolding = "constantFolding";
const std::string ifStatementPropagation = "ifStatementPropagation";
const std::string implicitListPromotion = "implicitListPromotion";
} // namespace pass

class PassManager {
private:
  const std::unique_ptr<ModuleNode> &moduleNode;
  Option opt;

  map<std::string, std::function<bool(const std::unique_ptr<ModuleNode> &)>>
      passMap = {
          {pass::symbolChecking, symbolChecking},
          {pass::argBinding, argBinding},
          {pass::blockInling, blockInling},
          {pass::blockLegalization, blockLegalization},
          {pass::functionInling, functionInling},
          {pass::forLoopUnrolling, forLoopUnrolling},
          {pass::constantFolding, constantFolding},
          {pass::ifStatementPropagation, ifStatementPropagation},
          {pass::implicitListPromotion, implicitListPromotion},
  };

  bool singlePass(std::string passName) {
    std::function<bool(const std::unique_ptr<ModuleNode> &)> passFunc =
        passMap[passName];
    if (opt.printASTBefore)
      moduleNode->print(std::string("AST Before ") + passName);
    if (!passFunc(std::move(moduleNode)))
      return false;
    if (opt.printASTAfter)
      moduleNode->print(std::string("AST After ") + passName);
    return true;
  }

  bool pipeline() {
    bool ret = singlePass(pass::symbolChecking) &&
               singlePass(pass::argBinding) && singlePass(pass::blockInling) &&
               singlePass(pass::blockLegalization) &&
               singlePass(pass::functionInling) &&
               singlePass(pass::constantFolding) &&
               singlePass(pass::forLoopUnrolling) &&
               singlePass(pass::constantFolding) &&
               singlePass(pass::ifStatementPropagation) &&
               singlePass(pass::implicitListPromotion);
    if (!ret)
      return false;
    if (opt.printASTBefore)
      moduleNode->print("AST Before Code Generation");
    return true;
  }

public:
  bool execute() {
    if (opt.runOnly.empty())
      return pipeline();
    for(std::string pass: opt.runOnly)
      if(!singlePass(pass))
        return false;
    return true;
  }

  PassManager(const std::unique_ptr<ModuleNode> &moduleNode, Option opt)
      : moduleNode(moduleNode), opt(opt){};
};
} // namespace transform