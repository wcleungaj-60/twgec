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

JsonObjectNode action::ActionEnhFFGeneralCircularRange::method(
    DefaultMap defaultMap, UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode posNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
  });
  JsonObjectNode regionNode = JsonObjectNode({
      {"defType", "\"circle\""},
      {"ciRad", defaultMap.get("radius")},
      {"offX", defaultMap.get("offsetX")},
      {"offY", defaultMap.get("offsetY")},
  });
  std::string actorId = defaultMap.get("actorId");
  JsonObjectNode dataNode = JsonObjectNode();
  if (actorId != "\"\"")
    dataNode.addNode("select", "\"actor\"")
        .addNode("actorCode", defaultMap.get("actorId"))
        .addNode("onActorLayer", "\"over\"");
  else
    dataNode.addNode("select", "\"map\"").addNode("pos", posNode.to_string(24));
  dataNode.addNode("region", regionNode.to_string(24))
      .addNode("duration", defaultMap.get("duration"))
      .addNode("color", defaultMap.get("color"))
      .addNode("alpha", "\"1\"")
      .addNode("lineWidth", defaultMap.get("lineWidth"))
      .addNode("lineAlign", "\"1\"")
      .addNode("angle", "\"0\"")
      .addNode("bornAnim", "\"none\"")
      .addNode("disAnim", "\"none\"");
  return dataNode;
}