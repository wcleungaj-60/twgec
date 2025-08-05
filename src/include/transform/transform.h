#include "ast.h"
namespace transform {

bool symbolChecking(const std::unique_ptr<ModuleNode> &moduleNode);
bool argBinding(const std::unique_ptr<ModuleNode> &moduleNode);
bool blockInling(const std::unique_ptr<ModuleNode> &moduleNode);
bool blockLegalization(const std::unique_ptr<ModuleNode> &moduleNode);
bool functionInling(const std::unique_ptr<ModuleNode> &moduleNode);
bool constantFolding(const std::unique_ptr<ModuleNode> &moduleNode);
bool ifStatementPropagation(const std::unique_ptr<ModuleNode> &moduleNode);

} // namespace transform