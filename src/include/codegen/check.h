#ifndef CODEGEN_CHECK_H
#define CODEGEN_CHECK_H

#include "transformer.h"

namespace codegen {
namespace check {

#define REGISTER_CODE_GEN_CHECK(name)                                         \
  class Check##name {                                                         \
  public:                                                                      \
    static DefaultMap defaultMap;                                              \
    static void method(std::ofstream &of,                                      \
                       std::unique_ptr<InstructionNode> &action);              \
  };

REGISTER_CODE_GEN_CHECK(String);
REGISTER_CODE_GEN_CHECK(Number);

} // namespace check
} // namespace codegen

#endif