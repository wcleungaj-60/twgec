#ifndef CODEGEN_TRIGGER_H
#define CODEGEN_TRIGGER_H

#include "codegen.h"
#include "instruction.h"

namespace codegen {
namespace trigger {

class TriggerActorAdded {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class TriggerActorDead {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class TriggerActorFire {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class TriggerActorHit {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class TriggerClickButton {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class TriggerKeyboardPressed {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class TriggerItemPickup {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class TriggerReleasePower {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

} // namespace trigger
} // namespace codegen

#endif