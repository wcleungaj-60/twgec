#include "ast.h"
namespace transform {

bool functionInling(const std::unique_ptr<ModuleNode> &moduleNode);
bool argBinding(const std::unique_ptr<ModuleNode> &moduleNode);
bool constantFolding(const std::unique_ptr<ModuleNode> &moduleNode);

} // namespace transform