#include "codegen/instruction/trigger.h"
#include "keyword.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"
#include <fstream>
#include <memory>

using namespace codegen;
using namespace formatter;
using namespace keyword;

void trigger::TriggerActorAdded::method(std::ofstream &of,
                                        std::unique_ptr<ParamAppsNode> &trigger,
                                        const config::InstructionConfig config,
                                        UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("varName")},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerActorDead::method(std::ofstream &of,
                                       std::unique_ptr<ParamAppsNode> &trigger,
                                       const config::InstructionConfig config,
                                       UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
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
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerActorFire::method(std::ofstream &of,
                                       std::unique_ptr<ParamAppsNode> &trigger,
                                       const config::InstructionConfig config,
                                       UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
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
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerActorHit::method(std::ofstream &of,
                                      std::unique_ptr<ParamAppsNode> &trigger,
                                      const config::InstructionConfig config,
                                      UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
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
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerClickButton::method(
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &trigger,
    const config::InstructionConfig config,
    UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("varName")},
      {"checkButtoncode", "true"},
      {"buttonCode", defaultMap.get("buttonId")},
      {"varnameDevice", "\"\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerKeyboardPressed::method(
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &trigger,
    const config::InstructionConfig config,
    UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
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
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerItemPickup::method(std::ofstream &of,
                                        std::unique_ptr<ParamAppsNode> &trigger,
                                        const config::InstructionConfig config,
                                        UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
  defaultMap.addInputMap(trigger->named_args);
  JsonObjectNode itemMatchNode = JsonObjectNode({
      {"method", defaultMap.get("matchKind", matchKind::keywordEnum)},
      {"itemCode", defaultMap.get("itemMatchCode")},
      {"itemType", "\"\""},
  });
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("actorVarname")},
      {"itemMatch", itemMatchNode.to_string(24)},
      {"itemVarname", defaultMap.get("itemVarname")},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void trigger::TriggerReleasePower::method(
    std::ofstream &of, std::unique_ptr<ParamAppsNode> &trigger,
    const config::InstructionConfig config,
    UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
  defaultMap.addInputMap(trigger->named_args);
  std::string ability = defaultMap.get("ability", abilityKind::keywordEnum);
  std::string weaponType =
      defaultMap.get("weapon", weaponKind::keywordEnum,
                     userDefinedMeta.customWeaponsKeywordEnum);

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
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}
