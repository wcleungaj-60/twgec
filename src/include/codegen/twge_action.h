#ifndef TWGE_ACTION_H
#define TWGE_ACTION_H

#include "ast.h"
#include "codegen_transformer.h"

namespace twge {
namespace action {
void console(std::ofstream &of, std::unique_ptr<ActionNode> &);

class ActionAddActor {
public:
  static DefaultMap defaultMap;
  static void addActor(std::ofstream &of, std::unique_ptr<ActionNode> &action);
};
} // namespace action
} // namespace twge

#endif