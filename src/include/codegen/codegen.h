#ifndef CODEGEN_H
#define CODEGEN_H

#include "metadata.h"

class CodeGen {
private:
  const std::unique_ptr<ModuleNode> &moduleNode;
  MetadataLegalizer &metadataLegalizer;
  void codegenMetaData(std::ofstream &of);
  void codegenModuleNode(std::ofstream &of);

public:
  CodeGen(const std::unique_ptr<ModuleNode> &moduleNode,
          MetadataLegalizer &metadataLegalizer)
      : moduleNode(moduleNode), metadataLegalizer(metadataLegalizer){};
  void codegen(std::string filepath);
};

#endif