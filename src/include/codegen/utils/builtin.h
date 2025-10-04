#ifndef CODEGEN_BUILTIN_H
#define CODEGEN_BUILTIN_H

#include "defaultMap.h"
#include "formatter.h"

using namespace codegen;
using namespace keyword;
using namespace codegen::formatter;

extern DefaultMap actorMatchDefaultMap;
extern DefaultMap customWeaponDefaultMap;

JsonArrayNode getActorMatchesNode(const std::shared_ptr<ValueNode> &);
JsonArrayNode getCustomWeaponsListNode(const std::shared_ptr<ValueNode> &);
JsonArrayNode getPatrolPathListNode(const std::shared_ptr<ValueNode> &);
JsonArrayNode getSpawnPointListNode(const std::shared_ptr<ValueNode> &);

#endif