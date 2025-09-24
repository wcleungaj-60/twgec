#ifndef CODEGEN_TRIGGER_H
#define CODEGEN_TRIGGER_H

#include "transformer.h"

namespace codegen {
namespace trigger {

#define REGISTER_CODE_GEN_TRIGGER(name)                                        \
  class Trigger##name {                                                        \
  public:                                                                      \
    static DefaultMap defaultMap;                                              \
    static void method(std::ofstream &of,                                      \
                       std::unique_ptr<InstructionNode> &action);              \
  };

REGISTER_CODE_GEN_TRIGGER(ActorDead);
REGISTER_CODE_GEN_TRIGGER(ActorFire);
REGISTER_CODE_GEN_TRIGGER(ClickButton);
REGISTER_CODE_GEN_TRIGGER(KeyboardPressed);
REGISTER_CODE_GEN_TRIGGER(ReleasePower);

} // namespace trigger
} // namespace codegen

#endif