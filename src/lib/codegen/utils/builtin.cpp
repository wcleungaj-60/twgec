#include "utils/builtin.h"
#include <string>

using namespace codegen;
using namespace keyword;
using namespace codegen::formatter;

namespace {
JsonObjectNode getCustomWeaponsNode(std::unique_ptr<ExpressionNode> &expNode) {
  customWeaponDefaultMap.clearInputMap();
  auto customWeaponValueNode =
      dynamic_cast<CustomWeaponValueNode *>(expNode->value.get());
  if (!customWeaponValueNode) {
    std::cerr << "CustomWeapon-typed value is expected at " << expNode->loc
              << "\n";
    return JsonObjectNode();
  }
  customWeaponDefaultMap.addInputMap(customWeaponValueNode->named_args);
  JsonObjectNode fireNode = JsonObjectNode({
      {"fireType", customWeaponDefaultMap.get(
                       "fireType", CustomWeaponAttackKind::keywordEnum)},
      {"deltaDamage", "0"},
  });
  JsonArrayNode firesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(fireNode));
  JsonObjectNode pivotOnHandScaleNode = JsonObjectNode({
      {"x", customWeaponDefaultMap.get("pivotOnHandXScale")},
      {"y", customWeaponDefaultMap.get("pivotOnHandYScale")},
  });
  JsonObjectNode pivotOnHandNode = JsonObjectNode({
      {"x", customWeaponDefaultMap.get("pivotOnHandX")},
      {"y", customWeaponDefaultMap.get("pivotOnHandY")},
      {"degrees", customWeaponDefaultMap.get("pivotOnHandDegree")},
      {"scale", pivotOnHandScaleNode.to_string(40)},
  });
  JsonObjectNode pivotOnIconScaleNode = JsonObjectNode({
      {"x", customWeaponDefaultMap.get("pivotOnIconXScale")},
      {"y", customWeaponDefaultMap.get("pivotOnIconYScale")},
  });
  JsonObjectNode pivotOnIconNode = JsonObjectNode({
      {"x", customWeaponDefaultMap.get("pivotOnIconX")},
      {"y", customWeaponDefaultMap.get("pivotOnIconY")},
      {"degrees", customWeaponDefaultMap.get("pivotOnIconDegree")},
      {"scale", pivotOnIconScaleNode.to_string(40)},
  });
  JsonObjectNode spriteNode = JsonObjectNode({
      {"pivotOnHand", pivotOnHandNode.to_string(36)},
      {"pivotOnIcon", pivotOnIconNode.to_string(36)},
  });
  JsonObjectNode configNode = JsonObjectNode({
      {"type", "\"close\""},
      {"clipAlias", customWeaponDefaultMap.get("reference")},
      {"scaleOnGround", customWeaponDefaultMap.get("scaleOnGround")},
      {"scaleOnIcon", customWeaponDefaultMap.get("scaleOnIcon")},
      {"pickTestFunc", "\"wider\""},
      {"weight", customWeaponDefaultMap.get("weight")},
      {"damage", customWeaponDefaultMap.get("damage")},
      {"swapTime", customWeaponDefaultMap.get("swapTime")},
      {"frameName", "\"BLADE_TYPE\""},
      {"clip", "\"\""},
      {"fireTime", customWeaponDefaultMap.get("fireTime")},
      {"fires", firesNode.to_string(32)},
      {"sprite", spriteNode.to_string(32)},
  });
  JsonObjectNode actorMatchNode = JsonObjectNode({
      {"code", customWeaponDefaultMap.get("code")},
      {"config", configNode.to_string(28)},
  });
  return actorMatchNode;
}

JsonObjectNode getPatrolPathNode(std::unique_ptr<ExpressionNode> &expNode) {
  customWeaponDefaultMap.clearInputMap();
  auto pointValueNode = dynamic_cast<PointValueNode *>(expNode->value.get());
  if (!pointValueNode) {
    std::cerr << "Point-typed value is expected at " << expNode->loc << "\n";
    return JsonObjectNode();
  }
  auto xValueNode =
      dynamic_cast<IntValueNode *>(pointValueNode->x->value.get());
  if (!xValueNode) {
    std::cerr << "Int-typed value is expected at " << pointValueNode->x->loc
              << "\n";
    return JsonObjectNode();
  }
  auto yValueNode =
      dynamic_cast<IntValueNode *>(pointValueNode->y->value.get());
  if (!yValueNode) {
    std::cerr << "Int-typed value is expected at " << pointValueNode->y->loc
              << "\n";
    return JsonObjectNode();
  }
  JsonObjectNode locNode = JsonObjectNode({
      {"x", "\"" + std::to_string(xValueNode->value) + "\""},
      {"y", "\"" + std::to_string(yValueNode->value) + "\""},
      {"range", "\"0\""},
  });
  JsonObjectNode patrolPathNode = JsonObjectNode({
      {"loc", locNode.to_string(32)},
      {"rotation", "\"0\""},
      {"duration", "\"3000\""},
  });
  return patrolPathNode;
}
} // namespace

DefaultMap actorMatchDefaultMap = DefaultMap(
    {
        {"matchKind", {AST_STRING, CODEGEN_STRING, "contain"}},
        {"controller", {AST_STRING, CODEGEN_STRING, "all"}},
        {"id", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "actorMatch");

DefaultMap customWeaponDefaultMap = DefaultMap(
    {
        {"reference", {AST_STRING, CODEGEN_STRING, ""}},
        {"code", {AST_STRING, CODEGEN_STRING, ""}},
        {"scaleOnGround", {AST_INT, CODEGEN_INT, "0.8"}},
        {"scaleOnIcon", {AST_INT, CODEGEN_INT, "1"}},
        {"weight", {AST_INT, CODEGEN_INT, "6"}},
        {"damage", {AST_INT, CODEGEN_INT, "25"}},
        {"swapTime", {AST_INT, CODEGEN_INT, "800"}},
        {"fireTime", {AST_INT, CODEGEN_INT, "1500"}},
        {"fireType", {AST_INT, CODEGEN_STRING, "backslash"}},
        {"pivotOnHandX", {AST_INT, CODEGEN_INT, "0"}},
        {"pivotOnHandXScale", {AST_INT, CODEGEN_INT, "1"}},
        {"pivotOnHandY", {AST_INT, CODEGEN_INT, "0"}},
        {"pivotOnHandYScale", {AST_INT, CODEGEN_INT, "1"}},
        {"pivotOnHandDegree", {AST_INT, CODEGEN_INT, "0"}},
        {"pivotOnIconX", {AST_INT, CODEGEN_INT, "0"}},
        {"pivotOnIconXScale", {AST_INT, CODEGEN_INT, "1"}},
        {"pivotOnIconY", {AST_INT, CODEGEN_INT, "0"}},
        {"pivotOnIconYScale", {AST_INT, CODEGEN_INT, "1"}},
        {"pivotOnIconDegree", {AST_INT, CODEGEN_INT, "0"}},
    },
    "customWeapon");

JsonArrayNode getActorMatchesNode(std::unique_ptr<InstructionNode> &instr,
                                  std::string key) {
  actorMatchDefaultMap.clearInputMap();
  for (auto &namedArg : instr->named_args)
    if (namedArg->key == key) {
      auto actorMatchValueNode =
          dynamic_cast<ActorMatchValueNode *>(namedArg->expNode->value.get());
      if (!actorMatchValueNode) {
        std::cerr << "ActorMatch-typed value is expected at "
                  << namedArg->expNode->loc << "\n";
        return JsonArrayNode();
      }
      actorMatchDefaultMap.addInputMap(actorMatchValueNode->named_args);
      JsonObjectNode actorCodeNode = JsonObjectNode({
          {"method",
           actorMatchDefaultMap.get("matchKind", matchKind::keywordEnum)},
          {"actorCode", actorMatchDefaultMap.get("id")},
      });
      JsonArrayNode actorCodesNode =
          JsonArrayNode(std::make_shared<JsonObjectNode>(actorCodeNode));
      JsonObjectNode campNode = JsonObjectNode("campAll", "true");
      JsonObjectNode actorMatchNode = JsonObjectNode({
          {"actorCodes", actorCodesNode.to_string(32)},
          {"brain",
           actorMatchDefaultMap.get("controller", actorBrainKind::keywordEnum)},
          {"camp", campNode.to_string(32)},
          {"excludeActorCodes", "[]"},
      });
      return JsonArrayNode(std::make_shared<JsonObjectNode>(actorMatchNode));
    }
  return JsonArrayNode();
}

JsonArrayNode
getCustomWeaponsListNode(std::vector<std::unique_ptr<MetadataNode>> &metadatas,
                         std::string key) {
  JsonArrayNode arrayNode = JsonArrayNode();
  for (auto &metadata : metadatas)
    if (metadata->key == key) {
      if (auto listNode =
              dynamic_cast<ListValueNode *>(metadata->expNode->value.get()))
        for (auto &item : listNode->items)
          arrayNode.addNode(
              std::make_shared<JsonObjectNode>(getCustomWeaponsNode(item)));
      else
        std::cerr << "list-typed value is expected at "
                  << metadata->expNode->loc << "\n";
    }
  return arrayNode;
}

JsonArrayNode getPatrolPathListNode(std::unique_ptr<InstructionNode> &instr,
                                    std::string key) {
  JsonArrayNode arrayNode = JsonArrayNode();
  for (auto &namedArg : instr->named_args)
    if (namedArg->key == key) {
      if (auto listNode =
              dynamic_cast<ListValueNode *>(namedArg->expNode->value.get()))
        for (auto &item : listNode->items)
          arrayNode.addNode(
              std::make_shared<JsonObjectNode>(getPatrolPathNode(item)));
      else
        std::cerr << "list-typed value is expected at "
                  << namedArg->expNode->loc << "\n";
    }
  return arrayNode;
}