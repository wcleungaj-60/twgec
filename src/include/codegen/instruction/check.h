#ifndef CODEGEN_CHECK_H
#define CODEGEN_CHECK_H

#include "codegen.h"
#include "instruction.h"

namespace codegen {
namespace check {

class CheckActorCount {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};
class CheckForEachActor {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};
class CheckString {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};
class CheckNumber {
public:
  static void method(std ::ofstream &of, std ::unique_ptr<ParamAppsNode> &check,
                     const config::InstructionConfig config,
                     UserDefinedMetadata userDefinedMeta);
};

} // namespace check
} // namespace codegen

#endif