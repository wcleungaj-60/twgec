#ifndef CODEGEN_BUILTIN_H
#define CODEGEN_BUILTIN_H

#include "defaultMap.h"
#include "formatter.h"

using namespace codegen;
using namespace keyword;
using namespace codegen::formatter;

extern DefaultMap actorMatchDefaultMap;
extern DefaultMap customWeaponDefaultMap;

JsonArrayNode getActorMatchesNode(std::unique_ptr<InstructionNode> &instr,
                                  std::string key);
// TODO: 1. update the keyword for weapon 2. add enum for attack
JsonArrayNode
getCustomWeaponsListNode(std::vector<std::unique_ptr<MetadataNode>> &metadatas,
                         std::string key);

#endif