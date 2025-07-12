#ifndef CODEGEN_ACTION_H
#define CODEGEN_ACTION_H

#include "transformer.h"

namespace codegen {
namespace action {

using std::ofstream;
using std::unique_ptr;

class ActionAddActor {
public:
  static DefaultMap defaultMap;
  static void addActor(ofstream &of, unique_ptr<InstructionNode> &action);
};

class ActionAddStuff {
public:
  static DefaultMap defaultMap;
  static void addStuff(ofstream &of, unique_ptr<InstructionNode> &action);
};

class ActionConsole {
public:
  static DefaultMap defaultMap;
  static void console(ofstream &of, unique_ptr<InstructionNode> &action);
};

class ActionEnblastEffect {
public:
  static DefaultMap defaultMap;
  static void enblastEffect(ofstream &of, unique_ptr<InstructionNode> &action);
};

class ActionSetGlobal {
public:
  static DefaultMap defaultMap;
  static void setGlobal(ofstream &of, unique_ptr<InstructionNode> &action);
};
} // namespace action
} // namespace codegen

#endif