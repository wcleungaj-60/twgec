#ifndef CODEGEN_ACTION_H
#define CODEGEN_ACTION_H

#include "transformer.h"

namespace codegen {
namespace action {
class ActionAddActor {
public:
  static DefaultMap defaultMap;
  static void addActor(std::ofstream &of, std::unique_ptr<ActionNode> &action);
};

class ActionAddStuff {
public:
  static DefaultMap defaultMap;
  static void addStuff(std::ofstream &of, std::unique_ptr<ActionNode> &action);
};

class ActionConsole {
public:
  static DefaultMap defaultMap;
  static void console(std::ofstream &of, std::unique_ptr<ActionNode> &action);
};
} // namespace action
} // namespace codegen

#endif