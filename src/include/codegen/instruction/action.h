#ifndef CODEGEN_ACTION_H
#define CODEGEN_ACTION_H

#include "codegen.h"
#include "instruction.h"

namespace codegen {
namespace action {
// Standard Module
class ActionActorAttributes {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionActorDisappear {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionActorFollow {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionActorTalk {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionAddActor {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionAddDropItem {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionAddMapSign {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionAddStuff {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionConsole {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionDeltaHp {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionEnblastEffect {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionLongBo {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionGetCookie {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionGetUserState {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionMapWarp {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionMissionComplete {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionSetCookie {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionSetGlobal {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionSetObjectVar {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionSetUserState {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionSetWeaponAbility {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

class ActionWait {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

// EnhFF module
class ActionEnhFFPlayerMousePosition {
public:
  static void method(std ::ofstream &of,
                     std ::unique_ptr<ParamAppsNode> &action,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

} // namespace action
} // namespace codegen

#endif