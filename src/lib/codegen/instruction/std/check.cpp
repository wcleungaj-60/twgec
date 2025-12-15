#include "codegen/instruction/check.h"
#include "keyword.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"
#include "utils/utils.h"
#include <fstream>

using namespace codegen;
using namespace formatter;
using namespace keyword;

void check::CheckActorCount::method(std::ofstream &of,
                                    std::unique_ptr<ParamAppsNode> &check,
                                    const config::InstructionConfig config,
                                    UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
  defaultMap.addInputMap(check->named_args);
  JsonObjectNode _logiNode = JsonObjectNode("_and_or", "\"\"");
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"operator", defaultMap.get("op", operationKind::keywordEnum)},
      {"varname", defaultMap.get("varname")},
      {"value", defaultMap.get("value")},
      {"_logi", _logiNode.to_string(24)},
      {"_elseEventId", "\"\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void check::CheckForEachActor::method(std::ofstream &of,
                                      std::unique_ptr<ParamAppsNode> &check,
                                      const config::InstructionConfig config,
                                      UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
  defaultMap.addInputMap(check->named_args);
  JsonObjectNode _logiNode = JsonObjectNode("_and_or", "\"\"");
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("varname")},
      {"maxTriggers", "\"0\""},
      {"donotActOnEachPass", "false"},
      {"varnameTotalLoops", "\"totalLoops\""},
      {"varnameOfPassCount", "\"totalPasses\""},
      {"_logi", _logiNode.to_string(24)},
      {"_elseEventId", "\"\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void check::CheckNumber::method(std::ofstream &of,
                                std::unique_ptr<ParamAppsNode> &check,
                                const config::InstructionConfig config,
                                UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
  defaultMap.addInputMap(check->named_args);
  JsonObjectNode _logiNode = JsonObjectNode("_and_or", "\"\"");
  JsonObjectNode dataNode = JsonObjectNode({
      {"value1", defaultMap.get("lhs")},
      {"operator", defaultMap.get("op", operationKind::keywordEnum)},
      {"value2", defaultMap.get("rhs")},
      {"_logi", _logiNode.to_string(24)},
      {"_elseEventId", "\"\""},
  });
  JsonObjectNode rootNode = JsonObjectNode({
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}

void check::CheckString::method(std::ofstream &of,
                                std::unique_ptr<ParamAppsNode> &check,
                                const config::InstructionConfig config,
                                UserDefinedMetadata userDefinedMeta) {
  DefaultMap defaultMap = toDefaultMap(config);
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
      {"type", "\"" + config.codegenName + "\""},
      {"data", dataNode.to_string(20)},
  });
  of << inden(16) << rootNode.to_string(16);
}
