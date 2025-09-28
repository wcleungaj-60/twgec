#include "utils/builtin.h"

using namespace codegen;
using namespace keyword;
using namespace codegen::formatter;

DefaultMap actorMatchDefaultMap = DefaultMap(
    {
        {"matchKind", {AST_STRING, CODEGEN_STRING, "contain"}},
        {"controller", {AST_STRING, CODEGEN_STRING, "all"}},
        {"id", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "actorMatch");

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