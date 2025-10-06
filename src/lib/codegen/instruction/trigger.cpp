#include "codegen/instruction/trigger.h"
#include "keyword.h"
#include "utils/formatter.h"
#include <fstream>
#include <map>
#include <memory>

using namespace codegen;
using namespace formatter;
using namespace keyword;

DefaultMap trigger::TriggerActorAdded::defaultMap = DefaultMap(
    {
        {"actor", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]"}},
        {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
    },
    "actorAdded");

DefaultMap trigger::TriggerActorDead::defaultMap = DefaultMap(
    {
        {"actor", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]"}},
        {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
        {"hitterVarName", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "actorDead");

DefaultMap trigger::TriggerActorFire::defaultMap = DefaultMap(
    {
        {"actor", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]"}},
        {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
        {"weapon", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "actorFire");

DefaultMap trigger::TriggerActorHit::defaultMap = DefaultMap(
    {
        {"actor", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]"}},
        {"hitter", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]"}},
        {"actorVarName", {AST_STRING, CODEGEN_STRING, ""}},
        {"hitterVarName", {AST_STRING, CODEGEN_STRING, ""}},
        {"damageValueVarName", {AST_STRING, CODEGEN_STRING, ""}},
        {"weapon", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "actorHit");

DefaultMap trigger::TriggerClickButton::defaultMap = DefaultMap(
    {
        {"actor", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]"}},
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
        {"actor", {AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]"}},
        {"varName", {AST_STRING, CODEGEN_STRING, "instance"}},
        {"ability", {AST_STRING, CODEGEN_STRING, ""}},
        {"preventDefault", {AST_BOOL, CODEGEN_STRING, "false"}},
        {"manaUsage", {AST_INT, CODEGEN_STRING, "0"}},
        {"weapon", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "releasePower");

void trigger::TriggerActorAdded::method(
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("varName")},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"ActorAdded\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerActorDead::method(
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
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
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
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

void trigger::TriggerActorHit::method(std::ofstream &of,
                                      std::unique_ptr<ParamAppsNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("actorVarName")},
      {"hitterMatches", defaultMap.get("hitter")},
      {"hitterVarname", defaultMap.get("hitterVarName")},
      {"damageTypes", "[]"},
      {"damageVarname", defaultMap.get("damageValueVarName")},
      {"aliveType", "\"skip\""},
      {"checkFallDmg", "\"a\""},
      {"checkDeviceDmg", "\"a\""},
  });
  if (defaultMap.get("weapon") != "\"\"") {
    dataNode.addNode("checkWeaponType", "true")
        .addNode("weaponTypeCheck", "\"include\"")
        .addNode("weaponType", defaultMap.get("weapon"));
  }
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"ActorHit\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerClickButton::method(
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &trigger) {
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
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
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &trigger) {
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
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &trigger,
    std::map<std::string, std::string> extraEnum) {
  defaultMap.addInputMap(trigger->named_args);
  std::string ability = defaultMap.get("ability", abilityKind::keywordEnum);
  std::string weaponType =
      defaultMap.get("weapon", weaponKind::keywordEnum, extraEnum);

  JsonObjectNode abilityNode = JsonObjectNode({
      {"value", ability},
  });
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
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
