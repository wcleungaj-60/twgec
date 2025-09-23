#include "check.h"
#include "codegen/check.h"
#include "formatter.h"
#include "keyword.h"
#include "utils/utils.h"
#include <fstream>
#include <map>
#include <memory>

using namespace codegen;
using namespace formatter;
using namespace keyword;

DefaultMap check::CheckNumber::defaultMap = DefaultMap(
    {
        {"lhs", {AST_INT, CODEGEN_STRING, "0"}},
        {"rhs", {AST_INT, CODEGEN_STRING, "0"}},
        {"op", {AST_STRING, CODEGEN_STRING, "=="}},
    },
    "checkNumber");

DefaultMap check::CheckString::defaultMap = DefaultMap(
    {
        {"value", {AST_STRING, CODEGEN_STRING, ""}},
        {"matchKind", {AST_STRING, CODEGEN_STRING, "contain"}},
        {"str", {AST_STRING, CODEGEN_STRING, ""}},
    },
    "checkString");

void check::CheckNumber::method(std::ofstream &of,
                                std::unique_ptr<InstructionNode> &check) {
  defaultMap.addInputMap(check->named_args);
  JsonObjectNode _logiNode = JsonObjectNode("_and_or", "\"\"");
  JsonObjectNode dataNode = JsonObjectNode({
      {"value1", defaultMap.get("lhs")},
      {"operator", defaultMap.get("op", _operator::keywordEnum)},
      {"value2", defaultMap.get("rhs")},
      {"_logi", _logiNode.to_string(24)},
      {"_elseEventId", "\"\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"NumberCheck\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

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
