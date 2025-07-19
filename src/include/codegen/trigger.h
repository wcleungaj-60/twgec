#ifndef CODEGEN_TRIGGER_H
#define CODEGEN_TRIGGER_H

#include "transformer.h"

namespace codegen {
namespace trigger {

class TriggerActorFire {
public:
  static DefaultMap defaultMap;
  static void actorFire(std::ofstream &of,
                        std::unique_ptr<InstructionNode> &trigger);
};

class TriggerClickButton {
public:
  static DefaultMap defaultMap;
  static void clickButton(std::ofstream &of,
                          std::unique_ptr<InstructionNode> &trigger);
};

class TriggerReleasePower {
public:
  static DefaultMap defaultMap;
  static void releasePower(std::ofstream &of,
                           std::unique_ptr<InstructionNode> &trigger);
};

} // namespace trigger
} // namespace codegen

#endif