#include "codegen.h"
#include "ast.h"
#include "instruction.h"
#include "instruction/action.h"
#include "instruction/check.h"
#include "instruction/metadata.h"
#include "instruction/trigger.h"
#include "keyword.h"
#include "utils/utils.h"
#include <fstream>
#include <iostream>

using namespace codegen;

void CodeGenerator::codegen(std::string filepath) {
  std::ofstream outputFile(filepath);
  if (!outputFile)
    return;
  preCodegen();
  codegenModuleNode(outputFile);
  outputFile.close();
}

void CodeGenerator::preCodegen() {
  for (auto &metadata : moduleNode->metadatas) {
    if (metadata->key != keyword::metadataKind::customWeapons)
      continue;
    auto listNode =
        dynamic_cast<ListValueNode *>(metadata->expNode->value.get());
    if (!listNode)
      continue;
    for (auto &item : listNode->items) {
      auto cwNode = dynamic_cast<CustomWeaponValueNode *>(item->value.get());
      if (!cwNode)
        continue;
      for (auto &namedArg : cwNode->paramApps->named_args)
        if (namedArg->key == "code") {
          auto stringNode =
              dynamic_cast<StringValueNode *>(namedArg->expNode->value.get());
          if (!stringNode)
            break;
          string keyName = "custom_" + stringNode->value;
          userDefinedMeta.customWeaponsKeywordEnum.insert({keyName, keyName});
        }
    }
  }
}

void CodeGenerator::codegenModuleNode(std::ofstream &of) {
  of << "{" << std::endl;
  metadata::ConfigSetup::setup(of, moduleNode->metadatas);
  codegenBlocks(of);
  of << "}" << std::endl;
}

void CodeGenerator::codegenBlocks(std::ofstream &of) {
  of << inden(4) << "\"events\": [";
  if (moduleNode->blocks.size() == 0) {
    of << "]" << std::endl;
    return;
  }
  of << std::endl;
  for (auto i = 0; i < moduleNode->blocks.size(); i++) {
    auto &block = moduleNode->blocks[i];
    of << inden(8) << "{" << std::endl;
    of << inden(12) << "\"id\": \"" << block->identifier << "\"," << std::endl;
    metadata::BlockSetup::setup(of, block->blockBody->metadatas);
    codegenTypedInstrSet(of, block->blockBody->getActions());
    codegenTypedInstrSet(of, block->blockBody->getChecks());
    codegenTypedInstrSet(of, block->blockBody->getTriggers());
    of << inden(8) << "}";
    if (i != moduleNode->blocks.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(4) << "]" << std::endl;
}

void CodeGenerator::codegenTypedInstrSet(
    std::ofstream &of, std::unique_ptr<TypedInstrSetNode> &typedInstrSet) {
  std::string endingStr =
      typedInstrSet->type == FUN_DEF_TYPE_TRIGGERS ? "]\n" : "],\n";
  auto &instructions = typedInstrSet->instrSet->instructions;
  of << inden(12) << "\"" << typedInstrSet->type << "\": [";
  if (instructions.empty()) {
    of << endingStr;
    return;
  }
  of << std::endl;
  for (auto i = 0; i < instructions.size(); i++) {
    switch (typedInstrSet->type) {
    case FUN_DEF_TYPE_ACTIONS:
      codegenAction(of, instructions[i]->instruction);
      break;
    case FUN_DEF_TYPE_CHECKS:
      codegenCheck(of, instructions[i]->instruction);
      break;
    case FUN_DEF_TYPE_TRIGGERS:
      codegenTrigger(of, instructions[i]->instruction);
      break;
    default:
      break;
    }
    if (i != instructions.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(12) << endingStr;
}

#define CODEGEN_PARAM                                                          \
  of, instrNode->paramApps, configMap.at(identifier), userDefinedMeta

void CodeGenerator::codegenAction(std::ofstream &of,
                                  std::unique_ptr<InstructionNode> &instrNode) {
  auto configMap = configVecToMap(config::actionList);
  std::string identifier = instrNode->identifier;
  // Standard Module
  if (identifier == "actorAttributes" || identifier == "設定角色屬性")
    return action::ActionActorAttributes::method(CODEGEN_PARAM);
  if (identifier == "actorDisappear" || identifier == "角色消失")
    return action::ActionActorDisappear::method(CODEGEN_PARAM);
  if (identifier == "actorFollow" || identifier == "跟隨人物")
    return action::ActionActorFollow::method(CODEGEN_PARAM);
  if (identifier == "actorTalk" || identifier == "角色說話")
    return action::ActionActorTalk::method(CODEGEN_PARAM);
  if (identifier == "addActor" || identifier == "新增角色")
    return action::ActionAddActor::method(CODEGEN_PARAM);
  if (identifier == "addDropItem" || identifier == "新增放置可拾取道具")
    return action::ActionAddDropItem::method(CODEGEN_PARAM);
  if (identifier == "addMapSign" || identifier == "新增告示牌")
    return action::ActionAddMapSign::method(CODEGEN_PARAM);
  if (identifier == "addStuff" || identifier == "新增武器道具")
    return action::ActionAddStuff::method(CODEGEN_PARAM);
  if (identifier == "deltaHp" || identifier == "角色加減血")
    return action::ActionDeltaHp::method(CODEGEN_PARAM);
  if (identifier == "enblastEffect" || identifier == "光彈特效")
    return action::ActionEnblastEffect::method(CODEGEN_PARAM);
  if (identifier == "getCookie" || identifier == "取得Cookies")
    return action::ActionGetCookie::method(CODEGEN_PARAM);
  if (identifier == "getUserState" || identifier == "取得玩家狀態")
    return action::ActionGetUserState::method(CODEGEN_PARAM);
  if (identifier == "mapWarp" || identifier == "設定地圖傳送點")
    return action::ActionMapWarp::method(CODEGEN_PARAM);
  if (identifier == "missionComplete" || identifier == "任務完成")
    return action::ActionMissionComplete::method(CODEGEN_PARAM);
  if (identifier == "longBo" || identifier == "龍波")
    return action::ActionLongBo::method(CODEGEN_PARAM);
  if (identifier == "print" || identifier == "控制台輸出")
    return action::ActionConsole::method(CODEGEN_PARAM);
  if (identifier == "setCookie" || identifier == "儲存Cookies")
    return action::ActionSetCookie::method(CODEGEN_PARAM);
  if (identifier == "setGlobal" || identifier == "儲存全域變數")
    return action::ActionSetGlobal::method(CODEGEN_PARAM);
  if (identifier == "setObjectVar" || identifier == "儲存物件變數")
    return action::ActionSetObjectVar::method(CODEGEN_PARAM);
  if (identifier == "setUserState" || identifier == "儲存玩家狀態")
    return action::ActionSetUserState::method(CODEGEN_PARAM);
  if (identifier == "setWeaponAbility" || identifier == "設定武器技能")
    return action::ActionSetWeaponAbility::method(CODEGEN_PARAM);
  if (identifier == "wait" || identifier == "等待")
    return action::ActionWait::method(CODEGEN_PARAM);
  // EnhFF Module
  if (identifier == "EnhFF::playerMousePosition" ||
      identifier == "EnhFF::玩家滑鼠座標")
    return action::ActionEnhFFPlayerMousePosition::method(CODEGEN_PARAM);
  std::cerr << "Codegen error: Cannot found the corresponding action name \""
            << identifier << "\" at " << instrNode.get()->loc << "\n";
}

void CodeGenerator::codegenCheck(std::ofstream &of,
                                 std::unique_ptr<InstructionNode> &instrNode) {
  auto configMap = configVecToMap(config::checkList);
  std::string identifier = instrNode->identifier;
  if (identifier == "actorCount" || identifier == "計算人數")
    return check::CheckActorCount::method(CODEGEN_PARAM);
  if (identifier == "forEachActor" || identifier == "找出所有角色")
    return check::CheckForEachActor::method(CODEGEN_PARAM);
  if (identifier == "checkString" || identifier == "比對字串")
    return check::CheckString::method(CODEGEN_PARAM);
  if (identifier == "checkNumber" || identifier == "比較數字")
    return check::CheckNumber::method(CODEGEN_PARAM);
  std::cerr << "Codegen error: Cannot found the corresponding check name \""
            << identifier << "\" at " << instrNode.get()->loc << "\n";
}

void CodeGenerator::codegenTrigger(
    std::ofstream &of, std::unique_ptr<InstructionNode> &instrNode) {
  auto configMap = configVecToMap(config::triggerList);
  std::string identifier = instrNode->identifier;
  if (identifier == "actorAdded" || identifier == "角色進入戰場")
    return trigger::TriggerActorAdded::method(CODEGEN_PARAM);
  if (identifier == "actorDead" || identifier == "角色死亡")
    return trigger::TriggerActorDead::method(CODEGEN_PARAM);
  if (identifier == "actorFire" || identifier == "角色發動攻擊")
    return trigger::TriggerActorFire::method(CODEGEN_PARAM);
  if (identifier == "actorHit" || identifier == "角色受傷")
    return trigger::TriggerActorHit::method(CODEGEN_PARAM);
  if (identifier == "clickButton" || identifier == "告示牌按鈕")
    return trigger::TriggerClickButton::method(CODEGEN_PARAM);
  if (identifier == "keyboardPressed" || identifier == "鍵盤按鍵")
    return trigger::TriggerKeyboardPressed::method(CODEGEN_PARAM);
  if (identifier == "itemPickup" || identifier == "拾取武器道具")
    return trigger::TriggerItemPickup::method(CODEGEN_PARAM);
  if (identifier == "releasePower" || identifier == "發動技能")
    return trigger::TriggerReleasePower::method(CODEGEN_PARAM);
  std::cerr << "Codegen error: Cannot found the corresponding trigger name \""
            << identifier << "\" at " << instrNode.get()->loc << "\n";
}
#undef CODEGEN_PARAM
