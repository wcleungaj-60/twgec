#ifndef CODEGEN_ACTION_H
#define CODEGEN_ACTION_H

#include "codegen.h"
#include "utils/defaultMap.h"

namespace codegen {
namespace action {

#define REGISTER_CODE_GEN_ACTION(name)                                         \
  class Action##name {                                                         \
  public:                                                                      \
    static DefaultMap defaultMap;                                              \
    static void method(std::ofstream &of,                                      \
                       std::unique_ptr<ParamAppsNode> &action,                 \
                       UserDefinedMetadata userDefinedMeta);                   \
  };

// Standard Module
REGISTER_CODE_GEN_ACTION(ActorAttributes);
REGISTER_CODE_GEN_ACTION(ActorDisappear);
REGISTER_CODE_GEN_ACTION(ActorFollow);
REGISTER_CODE_GEN_ACTION(ActorTalk);
REGISTER_CODE_GEN_ACTION(AddActor);
REGISTER_CODE_GEN_ACTION(AddMapSign);
REGISTER_CODE_GEN_ACTION(AddStuff);
REGISTER_CODE_GEN_ACTION(Console);
REGISTER_CODE_GEN_ACTION(DeltaHp);
REGISTER_CODE_GEN_ACTION(EnblastEffect);
REGISTER_CODE_GEN_ACTION(LongBo);
REGISTER_CODE_GEN_ACTION(GetCookie);
REGISTER_CODE_GEN_ACTION(GetUserState);
REGISTER_CODE_GEN_ACTION(MapWarp);
REGISTER_CODE_GEN_ACTION(MissionComplete);
REGISTER_CODE_GEN_ACTION(SetCookie);
REGISTER_CODE_GEN_ACTION(SetGlobal);
REGISTER_CODE_GEN_ACTION(SetObjectVar);
REGISTER_CODE_GEN_ACTION(SetUserState);
REGISTER_CODE_GEN_ACTION(SetWeaponAbility);
REGISTER_CODE_GEN_ACTION(Wait);

// EnhFF module
REGISTER_CODE_GEN_ACTION(EnhFFPlayerMousePosition);

} // namespace action
} // namespace codegen

#endif