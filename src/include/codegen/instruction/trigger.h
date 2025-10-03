#ifndef CODEGEN_TRIGGER_H
#define CODEGEN_TRIGGER_H

#include "utils/defaultMap.h"

namespace codegen {
namespace trigger {

#define REGISTER_CODE_GEN_TRIGGER(name)                                        \
  class Trigger##name {                                                        \
  public:                                                                      \
    static DefaultMap defaultMap;                                              \
    static void method(std::ofstream &of,                                      \
                       std::unique_ptr<InstructionNode> &action);              \
  };

#define REGISTER_CODE_GEN_TRIGGER_WITH_EXTRA_ENUM(name)                        \
  class Trigger##name {                                                        \
  public:                                                                      \
    static DefaultMap defaultMap;                                              \
    static void method(std::ofstream &of,                                      \
                       std::unique_ptr<InstructionNode> &action,               \
                       std::map<std::string, std::string> extraEnum);          \
  };

REGISTER_CODE_GEN_TRIGGER(ActorDead);
REGISTER_CODE_GEN_TRIGGER(ActorFire);
REGISTER_CODE_GEN_TRIGGER(ClickButton);
REGISTER_CODE_GEN_TRIGGER(KeyboardPressed);
REGISTER_CODE_GEN_TRIGGER_WITH_EXTRA_ENUM(ReleasePower);

} // namespace trigger
} // namespace codegen

#endif