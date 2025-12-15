#include "instruction/action.h"
#include "codegen.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"
#include "utils/utils.h"
#include <fstream>

using namespace codegen;
using namespace formatter;
using namespace keyword;

void action::ActionEnhFFPlayerMousePosition::method(
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &action,
                                const config::InstructionConfig config,
                                UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"varX", defaultMap.get("varX")},
      {"varY", defaultMap.get("varY")},
      {"wait", "true"},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}