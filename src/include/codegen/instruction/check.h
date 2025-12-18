#ifndef CODEGEN_CHECK_H
#define CODEGEN_CHECK_H

#include "codegen.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"

namespace codegen {
namespace check {

#define REGISTER_CODE_GEN(name)                                                \
  namespace Check##name {                                                      \
    formatter::JsonObjectNode method(DefaultMap defaultMap,                    \
                                     UserDefinedMetadata userDefinedMeta);     \
  }

REGISTER_CODE_GEN(ActorCount);
REGISTER_CODE_GEN(ForEachActor);
REGISTER_CODE_GEN(String);
REGISTER_CODE_GEN(Number);

#undef REGISTER_CODE_GEN

} // namespace check
} // namespace codegen

#endif