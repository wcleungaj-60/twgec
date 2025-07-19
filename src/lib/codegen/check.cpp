#include "check.h"
#include "codegen/check.h"
#include "formatter.h"
#include "keyword.h"
#include "utils/utils.h"
#include <fstream>
#include <memory>
#include <unordered_map>

using namespace codegen;
using namespace formatter;
using namespace keyword;

DefaultMap check::CheckString::defaultMap = DefaultMap({
    {"value", {AST_STRING, CODEGEN_STRING, ""}},
    {"matchKind", {AST_STRING, CODEGEN_STRING, "contain"}},
    {"str", {AST_STRING, CODEGEN_STRING, ""}},
});

void check::CheckString::method(std::ofstream &of,
                                     std::unique_ptr<InstructionNode> &check) {
  defaultMap.addInputMap(check->named_args);
  JsonObjectNode _logiNode = JsonObjectNode("_and_or", "\"\"");
  JsonObjectNode dataNode = JsonObjectNode({
      {"value", defaultMap.get("value")},
      {"method", defaultMap.get("matchKind", matchKind::keywordEnum)},
      {"str", defaultMap.get("str")},
      {"eventLocal", "\"\""},
      {"_logi", _logiNode.to_string(24)},
      {"_elseEventId", "\"\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"String\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}
