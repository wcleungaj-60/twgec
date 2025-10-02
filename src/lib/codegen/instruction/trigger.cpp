#include "codegen/instruction/trigger.h"
#include "keyword.h"
#include "utils/builtin.h"
#include "utils/formatter.h"
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
        {"weapon", {AST_STRING, CODEGEN_STRING, ""}},
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

void trigger::TriggerActorDead::method(
    std::ofstream &of, std::unique_ptr<InstructionNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args, {"actor"});
  JsonArrayNode actorMatchesNode = getActorMatchesNode(trigger, "actor");
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", actorMatchesNode.to_string(24)},
      {"varname", defaultMap.get("varName")},
      {"timing", "\"preDead\""},
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
  defaultMap.addInputMap(trigger->named_args, {"actor"});
  JsonArrayNode actorMatchesNode = getActorMatchesNode(trigger, "actor");
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", actorMatchesNode.to_string(24)},
      {"varname", defaultMap.get("varName")},
      {"fireTriggerType", "\"all\""},
      {"damageTypes", "[]"},
  });
  if (defaultMap.get("weapon") != "\"\"") {
    dataNode.addNode("checkWeaponType", "true")
        .addNode("weaponTypeCheck", "\"include\"")
        .addNode("weaponType", defaultMap.get("weapon"));
  }
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"ActorFire\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerClickButton::method(
    std::ofstream &of, std::unique_ptr<InstructionNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args, {"actor"});
  JsonArrayNode actorMatchesNode = getActorMatchesNode(trigger, "actor");
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
      {"timing", defaultMap.get("timing", keyPressTimingKind::keywordEnum)},
      {"key", defaultMap.get("key", keyPressKeyKind::keywordEnum)},
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
  defaultMap.addInputMap(trigger->named_args, {"actor"});
  JsonArrayNode actorMatchesNode = getActorMatchesNode(trigger, "actor");
  std::string ability = defaultMap.get("ability", abilityKind::keywordEnum);
  std::string weaponType = defaultMap.get("weapon", weaponKind::keywordEnum);

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
