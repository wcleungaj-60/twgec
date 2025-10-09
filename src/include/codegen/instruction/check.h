#ifndef CODEGEN_CHECK_H
#define CODEGEN_CHECK_H

#include "codegen.h"
#include "utils/defaultMap.h"

namespace codegen {
namespace check {

#define REGISTER_CODE_GEN_CHECK(name)                                          \
  class Check##name {                                                          \
  public:                                                                      \
    static DefaultMap defaultMap;                                              \
    static void method(std::ofstream &of,                                      \
                       std::unique_ptr<ParamAppsNode> &check,                  \
                       UserDefinedMetadata userDefinedMeta);                   \
  };

REGISTER_CODE_GEN_CHECK(ActorCount);
REGISTER_CODE_GEN_CHECK(ForEachActor);
REGISTER_CODE_GEN_CHECK(String);
REGISTER_CODE_GEN_CHECK(Number);

} // namespace check
} // namespace codegen

#endif