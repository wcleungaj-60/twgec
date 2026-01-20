#include "instruction/action.h"
#include "codegen.h"
#include "keyword.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"
#include <map>

using namespace codegen;
using namespace formatter;
using namespace keyword;

JsonObjectNode
action::ActionActorAttributes::method(DefaultMap defaultMap,
                                      UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode opNode = JsonObjectNode({
      {"attr", defaultMap.get("attr", actorAttrKind::keywordEnum)},
      {"operation", "\"set\""},
      {"value", defaultMap.get("value")},
  });
  JsonArrayNode opsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(opNode));
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"ops", opsNode.to_string(24)},
      {"takeFriendAIFire", "\"u\""},
      {"takeFriendPlayerFire", "\"u\""},
      {"exitWormhole", "\"u\""},
      {"setStatic", "\"u\""},
      {"setTornadoRes", "\"u\""},
      {"bloodType", "\"u\""},
  });
}

JsonObjectNode
action::ActionActorDisappear::method(DefaultMap defaultMap,
                                     UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"duration", defaultMap.get("duration")},
      {"delay", defaultMap.get("delay")},
  });
}

JsonObjectNode
action::ActionActorFollow::method(DefaultMap defaultMap,
                                  UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"followType", defaultMap.get("type", followActorKind::keywordEnum)},
      {"targetCode", defaultMap.get("targetId")},
  });
}

JsonObjectNode
action::ActionActorTalk::method(DefaultMap defaultMap,
                                UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"cleanTalk", defaultMap.get("cleanTalk")},
      {"speech", defaultMap.get("text")},
      {"duration", defaultMap.get("duration")},
      {"wait", defaultMap.get("wait")},
      {"waitMore", "\"100\""},
      {"actorCode", defaultMap.get("actorId")},
  });
}

JsonObjectNode
action::ActionAddActor::method(DefaultMap defaultMap,
                               UserDefinedMetadata userDefinedMeta) {

  std::string externRole = defaultMap.get("externRole");
  bool hasExternRole = externRole != "\"\"";
  std::string role =
      hasExternRole ? "8" : defaultMap.get("role", roleKind::keywordEnum);
  JsonObjectNode roleNode = JsonObjectNode({{"dr", role}});
  if (hasExternRole)
    roleNode.addNode("rsrc", externRole);

  JsonObjectNode weapon0Node = JsonObjectNode({
      {"w0Type", defaultMap.get("weapon1", weaponKind::keywordEnum)},
  });
  JsonObjectNode weapon1Node = JsonObjectNode({
      {"w1Type", defaultMap.get("weapon2", weaponKind::keywordEnum)},
  });
  JsonObjectNode locationNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", "\"0\""},
  });
  return JsonObjectNode({
      {"actorCode", defaultMap.get("id")},
      {"name", defaultMap.get("name")},
      {"role", roleNode.to_string(24)},
      {"actorType", "\"defaultType\""},
      {"weapon0", weapon0Node.to_string(24)},
      {"weapon1", weapon1Node.to_string(24)},
      {"camp", defaultMap.get("camp", campKind::keywordEnum)},
      {"group", defaultMap.get("teamId")},
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
      {"strength", defaultMap.get("strength")},
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
      {"localVarname", defaultMap.get("localVarname")},
      {"globalVarname", "\"\""},
  });
}

JsonObjectNode
action::ActionAddDropItem::method(DefaultMap defaultMap,
                                  UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locationNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", defaultMap.get("range")},
  });
  return JsonObjectNode({
      {"location", locationNode.to_string(24)},
      {"shiftX", "0"},
      {"shiftY", "0"},
      {"itemCode", defaultMap.get("itemCode")},
      {"itemType", defaultMap.get("type", DropItemKind::keywordEnum)},
      {"itemScale", defaultMap.get("scale")},
      {"localVarname", defaultMap.get("localVarname")},
  });
}

JsonObjectNode
action::ActionAddMapSign::method(DefaultMap defaultMap,
                                 UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode btnNode = JsonObjectNode({
      {"buttonCode", defaultMap.get("buttonCode")},
      {"close", "true"},
      {"label", defaultMap.get("buttonLabel")},
      {"icon", "\"\""},
  });
  JsonArrayNode btnsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(btnNode));
  JsonObjectNode btnsGroupNode = JsonObjectNode({
      {"rowSize", "\"1\""},
      {"btnMinWidth", "\"1\""},
      {"textAlign", "\"center\""},
      {"varnameButton", "\"\""},
      {"btns", btnsNode.to_string(28)},
  });
  JsonObjectNode locNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", defaultMap.get("range")},
  });
  JsonArrayNode locsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(locNode));
  return JsonObjectNode({
      {"localVarname", "\"\""},
      {"locs", locsNode.to_string(24)},
      {"shape", "\"wood\""},
      {"rotation", defaultMap.get("rotation")},
      {"speech", defaultMap.get("text")},
      {"parsePolicy", "\"view\""},
      {"showButtons", defaultMap.get("showButtons")},
      {"btnsGroup", btnsGroupNode.to_string(24)},
  });
}

JsonObjectNode
action::ActionAddStuff::method(DefaultMap defaultMap,
                               UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locationNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", defaultMap.get("range")},
  });
  return JsonObjectNode({
      {"itemCode", defaultMap.get("code")},
      {"itemType", defaultMap.get("item")},
      {"loop", defaultMap.get("refill")},
      {"loopInterval", defaultMap.get("refillInterval")},
      {"location", locationNode.to_string(24)},
      {"rotation", defaultMap.get("rotation")},
      {"cameraZoomPolicy", "\"none\""},
      {"localVarname", "\"item\""},
  });
}

JsonObjectNode
action::ActionConsole::method(DefaultMap defaultMap,
                              UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"logType", defaultMap.get("type")},
      {"text", defaultMap.get("text")},
      {"value", "\"\""},
  });
}

JsonObjectNode
action::ActionDeltaHp::method(DefaultMap defaultMap,
                              UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorCode")},
      {"deltaType", defaultMap.get("type", deltaHpKind::keywordEnum)},
      {"hp", defaultMap.get("value")},
      {"casterCode", defaultMap.get("casterCode")},
  });
}

JsonObjectNode
action::ActionEnblastEffect::method(DefaultMap defaultMap,
                                    UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode fromPosNode = JsonObjectNode({
      {"posType", defaultMap.get("fromType")},
      {"actorCode", defaultMap.get("fromActor")},
      {"alive", "true"},
  });
  return JsonObjectNode({
      {"fromPos", fromPosNode.to_string(24)},
      {"toType", defaultMap.get("toType")},
      {"toAngle", defaultMap.get("toAngle")},
      {"damage", defaultMap.get("damage")},
      {"scale", defaultMap.get("scale")},
      {"speed", defaultMap.get("speed")},
  });
}

JsonObjectNode
action::ActionEquipWeapon::method(DefaultMap defaultMap,
                                  UserDefinedMetadata userDefinedMeta) {
  std::string handIndex = defaultMap.get("hand");
  std::string weaponIndex = "-1";
  if (handIndex == "1")
    weaponIndex = "0";
  if (handIndex == "2")
    weaponIndex = "1";
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorCode")},
      {"weaponType", defaultMap.get("type", weaponKind::keywordEnum)},
      {"weaponIndex", weaponIndex},
      {"checkOwnership", "false"},
      {"makeDefault", defaultMap.get("isDefault")},
  });
}

JsonObjectNode
action::ActionGetCookie::method(DefaultMap defaultMap,
                                UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"ckey", defaultMap.get("cookies")},
      {"key", defaultMap.get("varName")},
  });
}

JsonObjectNode
action::ActionGetUserState::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"playerId", defaultMap.get("playerId")},
      {"category", defaultMap.get("category")},
      {"key", defaultMap.get("key")},
      {"varname", defaultMap.get("varName")},
  });
}

JsonObjectNode
action::ActionMapWarp::method(DefaultMap defaultMap,
                              UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locationNode = JsonObjectNode({
      {"x", defaultMap.get("fromX")},
      {"y", defaultMap.get("fromY")},
      {"range", "\"0\""},
  });
  JsonObjectNode targetNode = JsonObjectNode({
      {"x", defaultMap.get("toX")},
      {"y", defaultMap.get("toY")},
      {"range", "\"0\""},
  });
  return JsonObjectNode({
      {"location", locationNode.to_string(24)},
      {"op", "\"add\""},
      {"direction", defaultMap.get("direction", MapWarpKind::keywordEnum)},
      {"warpSpeed", "\"imme\""},
      {"target", targetNode.to_string(24)},
  });
}

JsonObjectNode
action::ActionMissionComplete::method(DefaultMap defaultMap,
                                      UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"targetGroup",
       defaultMap.get("camp", campMissionCompleteKind::keywordEnum)},
  });
}

JsonObjectNode
action::ActionLongBo::method(DefaultMap defaultMap,
                             UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"posType", "\"actor\""},
      {"actorCode", defaultMap.get("actorCode")},
      {"alive", "true"},
      {"longBoType", "\"he\""},
      {"moveType", "\"path\""},
      {"setRotation", "\"default\""},
      {"pathLen", "\"40\""},
      {"maxTurns", "\"5\""},
      {"headScale", "\"1\""},
      {"tailScale", "\"1\""},
      {"speed", "\"1\""},
  });
}

JsonObjectNode
action::ActionSetCookie::method(DefaultMap defaultMap,
                                UserDefinedMetadata userDefinedMeta) {

  string playerId = defaultMap.get("playerId");
  JsonObjectNode dataNode;
  dataNode = (playerId == "\"\"") ? JsonObjectNode("playerTarget", "\"all\"")
                                  : JsonObjectNode({{"playerTarget", "\"one\""},
                                                    {"playerId", playerId}});

  dataNode.addNode("key", defaultMap.get("cookies"))
      .addNode("valueType", defaultMap.get("type", valueTypeKind::keywordEnum))
      .addNode("value", defaultMap.get("value"));
  return dataNode;
}

JsonObjectNode
action::ActionSetGlobal::method(DefaultMap defaultMap,
                                UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"key", defaultMap.get("key")},
      {"valueType", defaultMap.get("type", valueTypeKind::keywordEnum)},
      {"value", defaultMap.get("value")},
  });
}

JsonObjectNode
action::ActionSetObjectVar::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"objectId", defaultMap.get("object")},
      {"key", defaultMap.get("key")},
      {"valueType", defaultMap.get("type", valueTypeKind::keywordEnum)},
      {"value", defaultMap.get("value")},
  });
}

JsonObjectNode
action::ActionSetUserState::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"playerId", defaultMap.get("playerId")},
      {"category", defaultMap.get("category")},
      {"key", defaultMap.get("key")},
      {"valueType", defaultMap.get("type", valueTypeKind::keywordEnum)},
      {"value", defaultMap.get("value")},
  });
}

JsonObjectNode
action::ActionSetWeaponAbility::method(DefaultMap defaultMap,
                                       UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode campNode = JsonObjectNode("campAll", "true");
  JsonObjectNode abilityNode = JsonObjectNode(
      "value", defaultMap.get("ability", abilityKind::keywordEnum));
  return JsonObjectNode({
      {"weaponType", defaultMap.get("weapon", weaponKind::keywordEnum,
                                    userDefinedMeta.customWeaponsKeywordEnum)},
      {"camp", campNode.to_string(24)},
      {"abilityLevel", defaultMap.get("level")},
      {"operation",
       defaultMap.get("operation", SetWeaponAbilityOperationKind::keywordEnum)},
      {"ability", abilityNode.to_string(24)},
  });
}

JsonObjectNode action::ActionWait::method(DefaultMap defaultMap,
                                          UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"duration", defaultMap.get("duration")},
  });
}