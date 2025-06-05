#ifndef TWGE_ACTION_H
#define TWGE_ACTION_H

#include "metadata.h"
#include "utils/utils.h"

namespace twge {
namespace action {
void addActor(std::ofstream &of, std::unique_ptr<ActionNode> &);
void console(std::ofstream &of, std::unique_ptr<ActionNode> &);
} // namespace action
} // namespace twge

#endif