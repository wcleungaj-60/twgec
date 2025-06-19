#ifndef METADATA_H
#define METADATA_H

#include "codegen_transformer.h"

namespace codegen {
namespace metadata {
class ConfigSetup {
public:
  static DefaultMap defaultMap;
  static void setup(std::ofstream &of,
                    std::vector<std::unique_ptr<MetadataNode>> &);
};

class BlockSetup {
public:
  static DefaultMap defaultMap;
  static void setup(std::ofstream &of,
                    std::vector<std::unique_ptr<MetadataNode>> &);
};
} // namespace metadata
} // namespace codegen

#endif