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

DefaultMap action::ActionConsole::defaultMap =
    DefaultMap({{"type", {AST_STRING, CODEGEN_STRING, "log"}},
                {"text", {AST_STRING, CODEGEN_STRING, ""}}});

DefaultMap action::ActionSetGlobal::defaultMap =
    DefaultMap({{"key", {AST_STRING, CODEGEN_STRING, ""}},
                {"type", {AST_STRING, CODEGEN_STRING, "string"}},
                {"value", {AST_INT, CODEGEN_STRING, ""}}});

void action::ActionAddActor::addActor(
    std::ofstream &of, std::unique_ptr<InstructionNode> &action) {
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode roleNode =
      JsonObjectNode().addNode("dr", defaultMap.get("role", role::keywordEnum));
  JsonObjectNode weapon0Node = JsonObjectNode().addNode(
      "w0Type", defaultMap.get("weapon1", weapon::keywordEnum));
  JsonObjectNode weapon1Node = JsonObjectNode().addNode(
      "w1Type", defaultMap.get("weapon2", weapon::keywordEnum));
  JsonObjectNode locationNode = JsonObjectNode()
                                    .addNode("x", defaultMap.get("x"))
                                    .addNode("y", defaultMap.get("y"))
                                    .addNode("range", "\"0\"");
  JsonObjectNode dataNode =
      JsonObjectNode()
          .addNode("actorCode", defaultMap.get("id"))
          .addNode("name", defaultMap.get("name"))
          .addNode("role", std::make_shared<JsonObjectNode>(roleNode))
          .addNode("actorType", "\"defaultType\"")
          .addNode("weapon0", std::make_shared<JsonObjectNode>(weapon0Node))
          .addNode("weapon1", std::make_shared<JsonObjectNode>(weapon1Node))
          .addNode("camp", defaultMap.get("camp", camp::keywordEnum))
          .addNode("group", "\"0\"")
          .addNode("location", std::make_shared<JsonObjectNode>(locationNode))
          .addNode("shiftX", "0")
          .addNode("shiftY", "0")
          .addNode("spawnLoc", "true")
          .addNode("rotation", "\"0\"")
          .addNode("idleRotate", "true")
          .addNode("maxhp", defaultMap.get("hp"))
          .addNode("manaPower", "\"0\"")
          .addNode("lives", "\"1\"")
          .addNode("preferAbilityLevel", "\"smart\"")
          .addNode("maxAbilityLevel", "1")
          .addNode("weight", "\"4\"")
          .addNode("strength", "\"1\"")
          .addNode("vision", "\"300\"")
          .addNode("range", defaultMap.get("range"))
          .addNode("score", "\"10\"")
          .addNode("bloodType", "\"default\"")
          .addNode("unbrokenArmor", "true")
          .addNode("tornadoRes", "\"none\"")
          .addNode("distractWhenHit", "true")
          .addNode("thinkInterval", "60")
          .addNode("farAutoLevel", "\"0\"")
          .addNode("patrols", "true")
          .addNode("patrolPath", defaultMap.get("patrol"))
          .addNode("bornDelayDuration", "\"0\"")
          .addNode("bornDuration", "\"1000\"")
          .addNode("bornLockDuration", "\"1000\"")
          .addNode("bornEffect", "\"none\"")
          .addNode("bornAnim", "\"fadein\"")
          .addNode("localVarname", "\"actor\"")
          .addNode("globalVarname", "\"\"");
  JsonObjectNode rootNode =
      JsonObjectNode()
          .addNode("type", "\"AddActor\"")
          .addNode("data", std::make_shared<JsonObjectNode>(dataNode));
  of << inden(16) << rootNode.to_string(16);
}

void action::ActionAddStuff::addStuff(
    std::ofstream &of, std::unique_ptr<InstructionNode> &action) {
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode locationNode = JsonObjectNode()
                                    .addNode("x", defaultMap.get("x"))
                                    .addNode("y", defaultMap.get("y"))
                                    .addNode("range", defaultMap.get("range"));
  JsonObjectNode dataNode =
      JsonObjectNode()
          .addNode("itemCode", defaultMap.get("code"))
          .addNode("itemType", defaultMap.get("item"))
          .addNode("loop", defaultMap.get("refill"))
          .addNode("loopInterval", defaultMap.get("refillInterval"))
          .addNode("location", std::make_shared<JsonObjectNode>(locationNode))
          .addNode("rotation", defaultMap.get("rotation"))
          .addNode("cameraZoomPolicy", "\"none\"")
          .addNode("localVarname", "\"item\"");
  JsonObjectNode rootNode =
      JsonObjectNode()
          .addNode("type", "\"AddStuff\"")
          .addNode("data", std::make_shared<JsonObjectNode>(dataNode));
  of << inden(16) << rootNode.to_string(16);
}

void action::ActionConsole::console(std::ofstream &of,
                                    std::unique_ptr<InstructionNode> &action) {
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode dataNode = JsonObjectNode()
                                .addNode("logType", defaultMap.get("type"))
                                .addNode("text", defaultMap.get("text"))
                                .addNode("value", "\"\"");
  JsonObjectNode rootNode =
      JsonObjectNode()
          .addNode("type", "\"Console\"")
          .addNode("data", std::make_shared<JsonObjectNode>(dataNode));
  of << inden(16) << rootNode.to_string(16);
}

void action::ActionSetGlobal::setGlobal(
    std::ofstream &of, std::unique_ptr<InstructionNode> &action) {
  defaultMap.addInputMap(action->named_args);
  JsonObjectNode dataNode =
      JsonObjectNode()
          .addNode("key", defaultMap.get("key"))
          .addNode("valueType", defaultMap.get("type", valueType::keywordEnum))
          .addNode("value", defaultMap.get("value"));
  JsonObjectNode rootNode =
      JsonObjectNode()
          .addNode("type", "\"SetGlobal\"")
          .addNode("data", std::make_shared<JsonObjectNode>(dataNode));
  of << inden(16) << rootNode.to_string(16);
}