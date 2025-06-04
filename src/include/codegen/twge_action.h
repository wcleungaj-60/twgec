#ifndef TWGE_ACTION_H
#define TWGE_ACTION_H

#include "metadata.h"
#include "utils/utils.h"

namespace twge {
namespace action {
void console(std::ofstream &of, std::unique_ptr<ActionNode> &);
}
}

#endif