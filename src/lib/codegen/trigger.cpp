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

DefaultMap trigger::TriggerActorFire::defaultMap = DefaultMap({
    {"matchKind", {AST_STRING, CODEGEN_STRING, "contain"}},
    {"actorId", {AST_STRING, CODEGEN_STRING, ""}},
    {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
});

DefaultMap trigger::TriggerClickButton::defaultMap = DefaultMap({
    {"matchKind", {AST_STRING, CODEGEN_STRING, "contain"}},
    {"actorId", {AST_STRING, CODEGEN_STRING, ""}},
    {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
    {"buttonId", {AST_STRING, CODEGEN_STRING, ""}},
});

DefaultMap trigger::TriggerReleasePower::defaultMap = DefaultMap({
    {"matchKind", {AST_STRING, CODEGEN_STRING, "contain"}},
    {"actorId", {AST_STRING, CODEGEN_STRING, ""}},
    {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
    {"ability", {AST_STRING, CODEGEN_STRING, ""}},
    {"weapon", {AST_STRING, CODEGEN_STRING, ""}},
});

void trigger::TriggerActorFire::method(
    std::ofstream &of, std::unique_ptr<InstructionNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode actorCodeNode = JsonObjectNode({
      {"method", defaultMap.get("matchKind", matchKind::keywordEnum)},
      {"actorCode", defaultMap.get("actorId")},
  });
  JsonArrayNode actorCodesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(actorCodeNode));
  JsonObjectNode campNode = JsonObjectNode("campAll", "true");
  JsonObjectNode actorMatchNode = JsonObjectNode({
      {"actorCodes", actorCodesNode.to_string(32)},
      {"brain", "\"all\""},
      {"camp", campNode.to_string(32)},
      {"excludeActorCodes", "[]"},
  });
  JsonArrayNode actorMatchesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(actorMatchNode));
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", actorMatchesNode.to_string(24)},
      {"varname", defaultMap.get("varName")},
      {"fireTriggerType", "\"all\""},
      {"damageTypes", "[]"},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"ActorFire\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerClickButton::method(
    std::ofstream &of, std::unique_ptr<InstructionNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode actorCodeNode = JsonObjectNode({
      {"method", defaultMap.get("matchKind", matchKind::keywordEnum)},
      {"actorCode", defaultMap.get("actorId")},
  });
  JsonArrayNode actorCodesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(actorCodeNode));
  JsonObjectNode campNode = JsonObjectNode("campAll", "true");
  JsonObjectNode actorMatchNode = JsonObjectNode({
      {"actorCodes", actorCodesNode.to_string(32)},
      {"brain", "\"all\""},
      {"camp", campNode.to_string(32)},
      {"excludeActorCodes", "[]"},
  });
  JsonArrayNode actorMatchesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(actorMatchNode));
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", actorMatchesNode.to_string(24)},
      {"varname", defaultMap.get("varName")},
      {"checkButtoncode", "true"},
      {"buttonCode", defaultMap.get("buttonId")},
      {"varnameDevice", "\"\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"TalkButton\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerReleasePower::method(
    std::ofstream &of, std::unique_ptr<InstructionNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args);
  std::string ability = defaultMap.get("ability", ability::keywordEnum);
  std::string weaponType = defaultMap.get("weapon", weapon::keywordEnum);

  JsonObjectNode actorCodeNode = JsonObjectNode({
      {"method", defaultMap.get("matchKind", matchKind::keywordEnum)},
      {"actorCode", defaultMap.get("actorId")},
  });
  JsonArrayNode actorCodesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(actorCodeNode));
  JsonObjectNode campNode = JsonObjectNode("campAll", "true");
  JsonObjectNode actorMatchNode = JsonObjectNode({
      {"actorCodes", actorCodesNode.to_string(32)},
      {"brain", "\"all\""},
      {"camp", campNode.to_string(32)},
      {"excludeActorCodes", "[]"},
  });
  JsonArrayNode actorMatchesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(actorMatchNode));

  JsonObjectNode abilityNode = JsonObjectNode({
      {"value", ability},
  });
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", actorMatchesNode.to_string(24)},
      {"varname", defaultMap.get("varName")},
      {"checkAbilityLevel", "0"},
  });
  if (ability != "\"\"")
    dataNode.addNode("checkAbility", "true")
        .addNode("ability", abilityNode.to_string(24));
  if (weaponType != "\"\"")
    dataNode.addNode("checkWeaponType", "true")
        .addNode("weaponType", weaponType);

  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"ReleasePower\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}
