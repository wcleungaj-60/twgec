#include "codegen/instruction/check.h"
#include "keyword.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"

using namespace codegen;
using namespace formatter;
using namespace keyword;

JsonObjectNode
check::CheckActorCount::method(DefaultMap defaultMap,
                               UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode _logiNode = JsonObjectNode("_and_or", "\"\"");
  return JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"operator", defaultMap.get("op", operationKind::keywordEnum)},
      {"varname", defaultMap.get("varname")},
      {"value", defaultMap.get("value")},
      {"_logi", _logiNode.to_string(24)},
      {"_elseEventId", "\"\""},
  });
}

JsonObjectNode
check::CheckForEachActor::method(DefaultMap defaultMap,
                                 UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode _logiNode = JsonObjectNode("_and_or", "\"\"");
  return JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("varname")},
      {"maxTriggers", "\"0\""},
      {"donotActOnEachPass", "false"},
      {"varnameTotalLoops", "\"totalLoops\""},
      {"varnameOfPassCount", "\"totalPasses\""},
      {"_logi", _logiNode.to_string(24)},
      {"_elseEventId", "\"\""},
  });
}

JsonObjectNode check::CheckNumber::method(DefaultMap defaultMap,
                                          UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode _logiNode = JsonObjectNode("_and_or", "\"\"");
  return JsonObjectNode({
      {"value1", defaultMap.get("lhs")},
      {"operator", defaultMap.get("op", operationKind::keywordEnum)},
      {"value2", defaultMap.get("rhs")},
      {"_logi", _logiNode.to_string(24)},
      {"_elseEventId", "\"\""},
  });
}

JsonObjectNode check::CheckString::method(DefaultMap defaultMap,
                                          UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode _logiNode = JsonObjectNode("_and_or", "\"\"");
  return JsonObjectNode({
      {"value", defaultMap.get("value")},
      {"method", defaultMap.get("matchKind", matchKind::keywordEnum)},
      {"str", defaultMap.get("str")},
      {"eventLocal", "\"\""},
      {"_logi", _logiNode.to_string(24)},
      {"_elseEventId", "\"\""},
  });
}
