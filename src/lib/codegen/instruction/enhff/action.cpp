#include "instruction/action.h"
#include "codegen.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"
#include "utils/utils.h"
#include <fstream>
#include <map>

using namespace codegen;
using namespace formatter;
using namespace keyword;

DefaultMap action::ActionEnhFFPlayerMousePosition::defaultMap = DefaultMap(
    {
        {"actorId", {AST_STRING, CODEGEN_STRING, ""}},
        {"varX", {AST_STRING, CODEGEN_STRING, "x"}},
        {"varY", {AST_STRING, CODEGEN_STRING, "y"}},
    },
    "EnhFF::playerMousePosition");

void action::ActionEnhFFPlayerMousePosition::method(
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &action,
    UserDefinedMetadata userDefinedMeta) {
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"varX", defaultMap.get("varX")},
      {"varY", defaultMap.get("varY")},
      {"wait", "true"},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"PlayerMousePositionEnh\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}