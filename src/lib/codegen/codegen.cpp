#include "codegen.h"
#include "ast.h"
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

void CodeGenerator::codegenAction(std::ofstream &of,
                                  std::unique_ptr<InstructionNode> &action) {
  // Standard Module
  CODEGEN_ACTION("actorAttributes", ActorAttributes);
  CODEGEN_ACTION("設定角色屬性", ActorAttributes);
  CODEGEN_ACTION("actorDisappear", ActorDisappear);
  CODEGEN_ACTION("角色消失", ActorDisappear);
  CODEGEN_ACTION("actorFollow", ActorFollow);
  CODEGEN_ACTION("跟隨人物", ActorFollow);
  CODEGEN_ACTION("actorTalk", ActorTalk);
  CODEGEN_ACTION("角色說話", ActorTalk);
  CODEGEN_ACTION("addActor", AddActor);
  CODEGEN_ACTION("新增角色", AddActor);
  CODEGEN_ACTION("addDropItem", AddDropItem);
  CODEGEN_ACTION("新增放置可拾取道具", AddDropItem);
  CODEGEN_ACTION("addMapSign", AddMapSign);
  CODEGEN_ACTION("新增告示牌", AddMapSign);
  CODEGEN_ACTION("addStuff", AddStuff);
  CODEGEN_ACTION("新增武器道具", AddStuff);
  CODEGEN_ACTION("deltaHp", DeltaHp);
  CODEGEN_ACTION("角色加減血", DeltaHp);
  CODEGEN_ACTION("enblastEffect", EnblastEffect);
  CODEGEN_ACTION("光彈特效", EnblastEffect);
  CODEGEN_ACTION("getCookie", GetCookie);
  CODEGEN_ACTION("取得數位存根", GetCookie);
  CODEGEN_ACTION("getUserState", GetUserState);
  CODEGEN_ACTION("取得玩家狀態", GetUserState);
  CODEGEN_ACTION("mapWarp", MapWarp);
  CODEGEN_ACTION("設定地圖傳送點", MapWarp);
  CODEGEN_ACTION("missionComplete", MissionComplete);
  CODEGEN_ACTION("任務完成", MissionComplete);
  CODEGEN_ACTION("longBo", LongBo);
  CODEGEN_ACTION("龍波", LongBo);
  CODEGEN_ACTION("print", Console);
  CODEGEN_ACTION("控制台輸出", Console);
  CODEGEN_ACTION("setCookie", SetCookie);
  CODEGEN_ACTION("儲存數位存根", SetCookie);
  CODEGEN_ACTION("setGlobal", SetGlobal);
  CODEGEN_ACTION("儲存全域變數", SetGlobal);
  CODEGEN_ACTION("setObjectVar", SetObjectVar);
  CODEGEN_ACTION("儲存物件變數", SetObjectVar);
  CODEGEN_ACTION("setUserState", SetUserState);
  CODEGEN_ACTION("儲存玩家狀態", SetUserState);
  CODEGEN_ACTION("setWeaponAbility", SetWeaponAbility);
  CODEGEN_ACTION("設定武器技能", SetWeaponAbility);
  CODEGEN_ACTION("wait", Wait);
  CODEGEN_ACTION("等待", Wait);
  // EnhFF Module
  CODEGEN_ACTION("EnhFF::playerMousePosition", EnhFFPlayerMousePosition);
  CODEGEN_ACTION("EnhFF::玩家滑鼠座標", EnhFFPlayerMousePosition);
  std::cerr << "Codegen error: Cannot found the corresponding action name \""
            << action->identifier << "\" at " << action.get()->loc << "\n";
}

void CodeGenerator::codegenCheck(std::ofstream &of,
                                 std::unique_ptr<InstructionNode> &check) {
  CODEGEN_CHECK("actorCount", ActorCount);
  CODEGEN_CHECK("計算人數", ActorCount);
  CODEGEN_CHECK("forEachActor", ForEachActor);
  CODEGEN_CHECK("找出所有角色", ForEachActor);
  CODEGEN_CHECK("checkString", String);
  CODEGEN_CHECK("比對字串", String);
  CODEGEN_CHECK("checkNumber", Number);
  CODEGEN_CHECK("比較數字", Number);
  std::cerr << "Codegen error: Cannot found the corresponding check name \""
            << check->identifier << "\" at " << check.get()->loc << "\n";
}

void CodeGenerator::codegenTrigger(std::ofstream &of,
                                   std::unique_ptr<InstructionNode> &trigger) {
  CODEGEN_TRIGGER("actorAdded", ActorAdded);
  CODEGEN_TRIGGER("角色進入戰場", ActorAdded);
  CODEGEN_TRIGGER("actorDead", ActorDead);
  CODEGEN_TRIGGER("角色死亡", ActorDead);
  CODEGEN_TRIGGER("actorFire", ActorFire);
  CODEGEN_TRIGGER("角色發動攻擊", ActorFire);
  CODEGEN_TRIGGER("actorHit", ActorHit);
  CODEGEN_TRIGGER("角色受傷", ActorHit);
  CODEGEN_TRIGGER("clickButton", ClickButton);
  CODEGEN_TRIGGER("告示牌按鈕", ClickButton);
  CODEGEN_TRIGGER("keyboardPressed", KeyboardPressed);
  CODEGEN_TRIGGER("鍵盤按鍵", KeyboardPressed);
  CODEGEN_TRIGGER("itemPickup", ItemPickup);
  CODEGEN_TRIGGER("拾取武器道具", ItemPickup);
  CODEGEN_TRIGGER("releasePower", ReleasePower);
  CODEGEN_TRIGGER("發動技能", ReleasePower);
  std::cerr << "Codegen error: Cannot found the corresponding trigger name \""
            << trigger->identifier << "\" at " << trigger.get()->loc << "\n";
}
