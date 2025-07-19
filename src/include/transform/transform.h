#include "ast.h"
namespace transform {

bool aliasInling(const std::unique_ptr<ModuleNode> &moduleNode);
bool argBinding(const std::unique_ptr<ModuleNode> &moduleNode);

} // namespace transform