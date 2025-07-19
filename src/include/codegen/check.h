#ifndef CODEGEN_CHECK_H
#define CODEGEN_CHECK_H

#include "transformer.h"

namespace codegen {
namespace check {

class CheckString {
public:
  static DefaultMap defaultMap;
  static void checkString(std::ofstream &of,
                          std::unique_ptr<InstructionNode> &check);
};

} // namespace check
} // namespace codegen

#endif