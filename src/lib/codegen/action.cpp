#include "codegen/action.h"
#include "formatter.h"
#include "keyword.h"
#include "utils/utils.h"
#include <fstream>
#include <unordered_map>

using namespace codegen;
using namespace formatter;
using namespace keyword;

DefaultMap action::ActionAddActor::defaultMap = DefaultMap({
    {"id", {AST_STRING, CODEGEN_STRING, "ai*"}},
    {"name", {AST_STRING, CODEGEN_STRING, ""}},
    {"camp", {AST_STRING, CODEGEN_STRING, "skydow"}},
    {"weapon1", {AST_STRING, CODEGEN_STRING, "default"}},
    {"weapon2", {AST_STRING, CODEGEN_STRING, "default"}},
    {"x", {AST_INT, CODEGEN_STRING, "0"}},
    {"y", {AST_INT, CODEGEN_STRING, "0"}},
    {"hp", {AST_INT, CODEGEN_STRING, "100"}},
    {"range", {AST_INT, CODEGEN_STRING, "10000"}},
    {"role", {AST_STRING, CODEGEN_INT, "0"}},
    {"externRole", {AST_STRING, CODEGEN_STRING, ""}},
    {"patrol", {AST_LIST_POINT, CODEGEN_LIST_PATROL, "[]"}},
});

DefaultMap action::ActionAddStuff::defaultMap = DefaultMap({
    {"code", {AST_STRING, CODEGEN_STRING, "item*"}},
    {"item", {AST_STRING, CODEGEN_STRING, "magazine"}},
    {"refill", {AST_BOOL, CODEGEN_BOOL, "true"}},
    {"refillInterval", {AST_INT, CODEGEN_STRING, "10000"}},
    {"x", {AST_INT, CODEGEN_STRING, "0"}},
    {"y", {AST_INT, CODEGEN_STRING, "0"}},
    {"range", {AST_INT, CODEGEN_STRING, "0"}},
    {"rotation", {AST_INT, CODEGEN_STRING, "0"}},
});

DefaultMap action::ActionConsole::defaultMap = DefaultMap({
    {"type", {AST_STRING, CODEGEN_STRING, "log"}},
    {"text", {AST_STRING, CODEGEN_STRING, ""}},
});

DefaultMap action::ActionEnblastEffect::defaultMap = DefaultMap({
    {"fromType", {AST_STRING, CODEGEN_STRING, "actor"}},
    {"fromActor", {AST_STRING, CODEGEN_STRING, "instance"}},
    {"toType", {AST_STRING, CODEGEN_STRING, "angle"}},
    {"toAngle", {AST_INT, CODEGEN_STRING, "0"}},
    {"damage", {AST_INT, CODEGEN_STRING, "30"}},
    {"scale", {AST_INT, CODEGEN_STRING, "1"}},
    {"speed", {AST_INT, CODEGEN_STRING, "0.7"}},
});

DefaultMap action::ActionSetGlobal::defaultMap = DefaultMap({
    {"key", {AST_STRING, CODEGEN_STRING, ""}},
    {"type", {AST_STRING, CODEGEN_STRING, "string"}},
    {"value", {AST_INT, CODEGEN_STRING, ""}},
});

DefaultMap action::ActionSetObjectVar::defaultMap = DefaultMap({
    {"object", {AST_STRING, CODEGEN_STRING, ""}},
    {"key", {AST_STRING, CODEGEN_STRING, ""}},
    {"type", {AST_STRING, CODEGEN_STRING, "string"}},
    {"value", {AST_INT, CODEGEN_STRING, ""}},
});

void action::ActionAddActor::addActor(
    std::ofstream &of, std::unique_ptr<InstructionNode> &action) {
  defaultMap.addInputMap(action->named_args);
  std::string externRole = defaultMap.get("externRole");
  bool hasExternRole = externRole != "\"\"";
  std::string role =
      hasExternRole ? "8" : defaultMap.get("role", role::keywordEnum);
  JsonObjectNode roleNode = JsonObjectNode({{"dr", role}});
  if (hasExternRole)
    roleNode.addNode("rsrc", externRole);

  JsonObjectNode weapon0Node = JsonObjectNode({
      {"w0Type", defaultMap.get("weapon1", weapon::keywordEnum)},
  });
  JsonObjectNode weapon1Node = JsonObjectNode({
      {"w1Type", defaultMap.get("weapon2", weapon::keywordEnum)},
  });
  JsonObjectNode locationNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", "\"0\""},
  });
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorCode", defaultMap.get("id")},
      {"name", defaultMap.get("name")},
      {"role", roleNode.to_string(24)},
      {"actorType", "\"defaultType\""},
      {"weapon0", weapon0Node.to_string(24)},
      {"weapon1", weapon1Node.to_string(24)},
      {"camp", defaultMap.get("camp", camp::keywordEnum)},
      {"group", "\"0\""},
      {"location", locationNode.to_string(24)},
      {"shiftX", "0"},
      {"shiftY", "0"},
      {"spawnLoc", "true"},
      {"rotation", "\"0\""},
      {"idleRotate", "true"},
      {"maxhp", defaultMap.get("hp")},
      {"manaPower", "\"0\""},
      {"lives", "\"1\""},
      {"preferAbilityLevel", "\"smart\""},
      {"maxAbilityLevel", "1"},
      {"weight", "\"4\""},
      {"strength", "\"1\""},
      {"vision", "\"300\""},
      {"range", defaultMap.get("range")},
      {"score", "\"10\""},
      {"bloodType", "\"default\""},
      {"unbrokenArmor", "true"},
      {"tornadoRes", "\"none\""},
      {"distractWhenHit", "true"},
      {"thinkInterval", "60"},
      {"farAutoLevel", "\"0\""},
      {"patrols", "true"},
      {"patrolPath", defaultMap.get("patrol")},
      {"bornDelayDuration", "\"0\""},
      {"bornDuration", "\"1000\""},
      {"bornLockDuration", "\"1000\""},
      {"bornEffect", "\"none\""},
      {"bornAnim", "\"fadein\""},
      {"localVarname", "\"actor\""},
      {"globalVarname", "\"\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"AddActor\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void action::ActionAddStuff::addStuff(
    std::ofstream &of, std::unique_ptr<InstructionNode> &action) {
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode locationNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", defaultMap.get("range")},
  });
  JsonObjectNode dataNode = JsonObjectNode({
      {"itemCode", defaultMap.get("code")},
      {"itemType", defaultMap.get("item")},
      {"loop", defaultMap.get("refill")},
      {"loopInterval", defaultMap.get("refillInterval")},
      {"location", locationNode.to_string(24)},
      {"rotation", defaultMap.get("rotation")},
      {"cameraZoomPolicy", "\"none\""},
      {"localVarname", "\"item\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"AddStuff\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void action::ActionConsole::console(std::ofstream &of,
                                    std::unique_ptr<InstructionNode> &action) {
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"logType", defaultMap.get("type")},
      {"text", defaultMap.get("text")},
      {"value", "\"\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"Console\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void action::ActionEnblastEffect::enblastEffect(
    std::ofstream &of, std::unique_ptr<InstructionNode> &action) {
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode fromPosNode = JsonObjectNode({
      {"posType", defaultMap.get("fromType")},
      {"actorCode", defaultMap.get("fromActor")},
      {"alive", "true"},
  });
  JsonObjectNode dataNode = JsonObjectNode({
      {"fromPos", fromPosNode.to_string(24)},
      {"toType", defaultMap.get("toType")},
      {"toAngle", defaultMap.get("toAngle")},
      {"damage", defaultMap.get("damage")},
      {"scale", defaultMap.get("scale")},
      {"speed", defaultMap.get("speed")},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"EnblastEffect\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void action::ActionSetGlobal::setGlobal(
    std::ofstream &of, std::unique_ptr<InstructionNode> &action) {
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"key", defaultMap.get("key")},
      {"valueType", defaultMap.get("type", valueType::keywordEnum)},
      {"value", defaultMap.get("value")},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"SetGlobal\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void action::ActionSetObjectVar::setObjectVar(
    std::ofstream &of, std::unique_ptr<InstructionNode> &action) {
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode dataNode = JsonObjectNode({
      {"object", defaultMap.get("object")},
      {"key", defaultMap.get("key")},
      {"valueType", defaultMap.get("type", valueType::keywordEnum)},
      {"value", defaultMap.get("value")},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"SetObjectVar\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}