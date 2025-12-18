#include "instruction/action.h"
#include "codegen.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"

using namespace codegen;
using namespace formatter;
using namespace keyword;

JsonObjectNode action::ActionEnhFFPlayerMousePosition::method(
    DefaultMap defaultMap, UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"varX", defaultMap.get("varX")},
      {"varY", defaultMap.get("varY")},
      {"wait", "true"},
  });
}