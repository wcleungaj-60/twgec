#include "codegen/trigger.h"
#include "formatter.h"
#include "keyword.h"
#include "utils/utils.h"
#include <fstream>
#include <map>
#include <memory>

using namespace codegen;
using namespace formatter;
using namespace keyword;

DefaultMap trigger::TriggerActorDead::defaultMap = DefaultMap(
    {
        {"actor", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, ""}},
        {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
        {"hitterVarName", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "actorDead");

DefaultMap trigger::TriggerActorFire::defaultMap = DefaultMap(
    {
        {"actor", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, ""}},
        {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
    },
    "actorFire");

DefaultMap trigger::TriggerClickButton::defaultMap = DefaultMap(
    {
        {"actor", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, ""}},
        {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
        {"buttonId", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "clickButton");

DefaultMap trigger::TriggerKeyboardPressed::defaultMap = DefaultMap(
    {
        {"actorId", {AST_STRING, CODEGEN_STRING, "*"}},
        {"varName", {AST_STRING, CODEGEN_STRING, ""}},
        {"timing", {AST_STRING, CODEGEN_STRING, ""}},
        {"key", {AST_STRING, CODEGEN_INT, "65"}},
    },
    "keyboardPressed");

DefaultMap trigger::TriggerReleasePower::defaultMap = DefaultMap(
    {
        {"actor", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, ""}},
        {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
        {"ability", {AST_STRING, CODEGEN_STRING, ""}},
        {"preventDefault", {AST_BOOL, CODEGEN_STRING, "false"}},
        {"manaUsage", {AST_INT, CODEGEN_STRING, "0"}},
        {"weapon", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "releasePower");

// TODO: Make it general
DefaultMap actorMatchDefaultMap = DefaultMap(
    {
        {"matchKind", {AST_STRING, CODEGEN_STRING, "contain"}},
        {"id", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "actorMatch");

JsonObjectNode getActorMatchNode(std::unique_ptr<InstructionNode> &instr,
                                 std::string key) {
  actorMatchDefaultMap.clearInputMap();
  for (auto &namedArg : instr->named_args)
    if (namedArg->key == key) {
      auto actorMatchNode =
          dynamic_cast<ActorMatchValueNode *>(namedArg->expNode->value.get());
      actorMatchDefaultMap.addInputMap(actorMatchNode->named_args);
    }
  JsonObjectNode actorCodeNode = JsonObjectNode({
      {"method", actorMatchDefaultMap.get("matchKind", matchKind::keywordEnum)},
      {"actorCode", actorMatchDefaultMap.get("id")},
  });
  JsonArrayNode actorCodesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(actorCodeNode));
  JsonObjectNode campNode = JsonObjectNode("campAll", "true");
  return JsonObjectNode({
      {"actorCodes", actorCodesNode.to_string(32)},
      {"brain", "\"all\""},
      {"camp", campNode.to_string(32)},
      {"excludeActorCodes", "[]"},
  });
}

void trigger::TriggerActorDead::method(
    std::ofstream &of, std::unique_ptr<InstructionNode> &trigger) {
  auto clonedTrigger = trigger->clone(); // TODO: avoid the clone
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode actorMatchNode = getActorMatchNode(clonedTrigger, "actor");
  JsonArrayNode actorMatchesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(actorMatchNode));
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", actorMatchesNode.to_string(24)},
      {"varname", defaultMap.get("varName")},
      {"timing", "\"over\""},
      {"deathsVarname", "\"deaths\""},
      {"hitterMatches", "[]"},
      {"hitterVarname", defaultMap.get("hitterVarName")},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"ActorDead\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerActorFire::method(
    std::ofstream &of, std::unique_ptr<InstructionNode> &trigger) {
  auto clonedTrigger = trigger->clone(); // TODO: avoid the clone
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode actorMatchNode = getActorMatchNode(clonedTrigger, "actor");
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
  auto clonedTrigger = trigger->clone(); // TODO: avoid the clone
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode actorMatchNode = getActorMatchNode(clonedTrigger, "actor");
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

void trigger::TriggerKeyboardPressed::method(
    std::ofstream &of, std::unique_ptr<InstructionNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"playerId", defaultMap.get("actorId")},
      // `{instance.id}` is needed for parsing the playerLocal
      {"playerLocal", defaultMap.get("varName")},
      {"timing", defaultMap.get("timing", keyPressTiming::keywordEnum)},
      {"key", defaultMap.get("key", keyPressKey::keywordEnum)},
      {"keyLocal", "\"\""},
      {"ctrl", "\"\""},
      {"shift", "\"\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"KeyboardPressed\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerReleasePower::method(
    std::ofstream &of, std::unique_ptr<InstructionNode> &trigger) {
  auto clonedTrigger = trigger->clone(); // TODO: avoid the clone
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode actorMatchNode = getActorMatchNode(clonedTrigger, "actor");
  std::string ability = defaultMap.get("ability", ability::keywordEnum);
  std::string weaponType = defaultMap.get("weapon", weapon::keywordEnum);
  JsonArrayNode actorMatchesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(actorMatchNode));

  JsonObjectNode abilityNode = JsonObjectNode({
      {"value", ability},
  });
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", actorMatchesNode.to_string(24)},
      {"varname", defaultMap.get("varName")},
      {"preventDefault", defaultMap.get("preventDefault")},
      {"manaUsage", defaultMap.get("manaUsage")},
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
