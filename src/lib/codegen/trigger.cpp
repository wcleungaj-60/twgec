#include "codegen/trigger.h"
#include "formatter.h"
#include "keyword.h"
#include "utils/utils.h"
#include <fstream>
#include <memory>
#include <unordered_map>

using namespace codegen;
using namespace formatter;
using namespace keyword;

DefaultMap trigger::TriggerActorFire::defaultMap =
    DefaultMap({{"matchKind", {AST_STRING, CODEGEN_STRING, "contain"}},
                {"actorId", {AST_STRING, CODEGEN_STRING, ""}},
                {"varName", {AST_STRING, CODEGEN_STRING, "instance"}}});

void trigger::TriggerActorFire::actorFire(
    std::ofstream &of, std::unique_ptr<InstructionNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode actorCodeNode =
      JsonObjectNode()
          .addNode("method", defaultMap.get("matchKind"))
          .addNode("actorCode", defaultMap.get("actorId"));
  JsonArrayNode actorCodesNode =
      JsonArrayNode().addNode(std::make_shared<JsonObjectNode>(actorCodeNode));
  JsonObjectNode campNode = JsonObjectNode().addNode("campAll", "true");
  JsonObjectNode actorMatchNode =
      JsonObjectNode()
          .addNode("actorCodes",
                   std::make_shared<JsonArrayNode>(actorCodesNode))
          .addNode("brain", "\"all\"")
          .addNode("camp", std::make_shared<JsonObjectNode>(campNode))
          .addNode("excludeActorCodes", "[]");
  JsonArrayNode actorMatchesNode =
      JsonArrayNode().addNode(std::make_shared<JsonObjectNode>(actorMatchNode));
  JsonObjectNode dataNode =
      JsonObjectNode()
          .addNode("actorMatches",
                   std::make_shared<JsonArrayNode>(actorMatchesNode))
          .addNode("varname", defaultMap.get("varName"))
          .addNode("fireTriggerType", "\"all\"")
          .addNode("damageTypes", "[]");
  JsonObjectNode rootNode =
      JsonObjectNode()
          .addNode("type", "\"ActorFire\"")
          .addNode("data", std::make_shared<JsonObjectNode>(dataNode));
  of << inden(16) << rootNode.to_string(16);
}
