#ifndef HELP_H
#define HELP_H

#include "codegen/instruction/action.h"
#include "codegen/instruction/check.h"
#include "codegen/instruction/metadata.h"
#include "codegen/instruction/trigger.h"
#include "utils.h"
#include <sstream>
#include <string>

namespace {

using namespace codegen::action;
using namespace codegen::check;
using namespace codegen::metadata;
using namespace codegen::trigger;

#define CONCATENATE_IMPL(a, b) a##b
#define CONCATENATE(a, b) CONCATENATE_IMPL(a, b)

std::string getDefaultMapItem(codegen::DefaultMap defaultMap,
                              std::string nameZh) {
  int padding = 28;
  std::map<std::string, codegen::DefaultMapValue> defaultMapParam =
      defaultMap.defaultMap;
  std::string instrName = defaultMap.functionName;
  std::string ret;
  assert(instrName.size() <= padding);
  ret += inden(4) + instrName + inden(padding - instrName.size()) + nameZh + "\n";
  ret += inden(7);
  for (auto param : defaultMapParam) {
    std::ostringstream oss;
    oss << param.second.astType;
    ret += " " + param.first + ": " + oss.str() + ",";
  }
  ret.pop_back();
  ret += "\n";
  return ret;
}

const std::string introduction =
    "twgec (Twilight game.events compiler) is a transpiler that can generate "
    "`game.events` file from `*.twge` file.";

const std::string sectionOption = R"(
Usage: twgec <option>* file
Options:
    --help                                    Display this information.
    -o=<file>                                 Set the output file path.
    --run-only=<pass>(,<pass>)*               Run specific pass(es).
                                              Please refer to the pass in the next section.
    --print-ast-before                        Print the AST before each transformation.
    --print-ast-after                         Print the AST after each transformation.
    --print-token                             Print the Token of the language.
    
Passes:
    symbolChecking
    argBinding
    blockInling
    blockLegalization
    functionInling
    constantFolding
    forLoopUnrolling
    ifStatementPropagation
    implicitListPromotion)";

const std::string sectionBuiltinStruct = R"(
Builtin Struct:
    Point
        x: int, y: int
    ActorMatch
        controller: string, id: string, matchKind: string
    CustomWeapon
        reference: string, code: string, scaleOnGround: int, scaleOnIcon: int, weight: int, damage: int, swapTime: int, fireTime: int, fireType: int, pivotOnHandX: int, pivotOnHandXScale: int, pivotOnHandY: int, pivotOnHandYScale: int, pivotOnHandDegree: int, pivotOnIconX: int, pivotOnIconXScale: int, pivotOnIconY: int, pivotOnIconYScale: int, pivotOnIconDegree)";

std::string getGlobalMetadata() {
  std::string ret = "Supported Global Metadata:\n";
  const std::map<std::string, codegen::DefaultMapValue> defaultMap =
      ConfigSetup::defaultMap.defaultMap;
  for (auto metadata : defaultMap) {
    std::ostringstream oss;
    oss << metadata.second.astType;
    ret += inden(4) + "__" + metadata.first + "__ : " + oss.str() + "\n";
  }
  return ret;
}

std::string getBlockMetadata() {
  std::string ret = "Supported Block Metadata:\n";
  const std::map<std::string, codegen::DefaultMapValue> defaultMap =
      BlockSetup::defaultMap.defaultMap;
  for (auto metadata : defaultMap) {
    std::ostringstream oss;
    oss << metadata.second.astType;
    ret += inden(4) + "__" + metadata.first + "__ : " + oss.str() + "\n";
  }
  return ret;
}

std::string getActions() {
  std::string ret = "Supported Actions:\n";
  ret += getDefaultMapItem(ActionActorAttributes::defaultMap, "設定角色屬性");
  ret += getDefaultMapItem(ActionActorDisappear::defaultMap, "-角色消失");
  ret += getDefaultMapItem(ActionActorFollow::defaultMap, "跟隨人物");
  ret += getDefaultMapItem(ActionActorTalk::defaultMap, "角色說話");
  ret += getDefaultMapItem(ActionAddActor::defaultMap, "+角色");
  ret += getDefaultMapItem(ActionAddDropItem::defaultMap, "+放置可拾取道具");
  ret += getDefaultMapItem(ActionAddMapSign::defaultMap, "+告示牌");
  ret += getDefaultMapItem(ActionAddStuff::defaultMap, "+武器道具");
  ret += getDefaultMapItem(ActionDeltaHp::defaultMap, "角色加減血");
  ret += getDefaultMapItem(ActionEnblastEffect::defaultMap, "光彈特效");
  ret += getDefaultMapItem(ActionGetCookie::defaultMap, "取得Cookies");
  ret += getDefaultMapItem(ActionGetUserState::defaultMap, "取得玩家狀態");
  ret += getDefaultMapItem(ActionMapWarp::defaultMap, "設定地圖傳送點");
  ret += getDefaultMapItem(ActionMissionComplete::defaultMap, "任務完成");
  ret += getDefaultMapItem(ActionLongBo::defaultMap, "+龍波");
  // `print` is used in the front-end instead of `console`
  ret += getDefaultMapItem(ActionConsole::defaultMap, "控制台輸出");
  ret += getDefaultMapItem(ActionSetCookie::defaultMap, "儲存Cookies");
  ret += getDefaultMapItem(ActionSetGlobal::defaultMap, "儲存全域變數");
  ret += getDefaultMapItem(ActionSetObjectVar::defaultMap, "儲存物件變數");
  ret += getDefaultMapItem(ActionSetWeaponAbility::defaultMap, "設定武器技能");
  ret += getDefaultMapItem(ActionSetUserState::defaultMap, "儲存玩家狀態");
  ret += getDefaultMapItem(ActionWait::defaultMap, "等待");
  ret += getDefaultMapItem(ActionEnhFFPlayerMousePosition::defaultMap, "玩家滑鼠座標");
  return ret;
}

std::string getChecks() {
  std::string ret = "Supported Checks:\n";
  ret += getDefaultMapItem(CheckActorCount::defaultMap, "計算人數");
  ret += getDefaultMapItem(CheckNumber::defaultMap, "比較數字");
  ret += getDefaultMapItem(CheckString::defaultMap, "比對字串");
  ret += getDefaultMapItem(CheckForEachActor::defaultMap, "找出所有角色");
  return ret;
}

std::string getTriggers() {
  std::string ret = "Supported Triggers:\n";
  ret += getDefaultMapItem(TriggerActorAdded::defaultMap, "角色進入戰場");
  ret += getDefaultMapItem(TriggerActorDead::defaultMap, "角色死亡");
  ret += getDefaultMapItem(TriggerActorFire::defaultMap, "角色發動攻擊");
  ret += getDefaultMapItem(TriggerActorHit::defaultMap, "角色受傷");
  ret += getDefaultMapItem(TriggerClickButton::defaultMap, "告示牌按鈕");
  ret += getDefaultMapItem(TriggerKeyboardPressed::defaultMap, "鍵盤按鍵");
  ret += getDefaultMapItem(TriggerReleasePower::defaultMap, "發動技能");
  return ret;
}
} // namespace

static const std::string helpMessage =
    introduction + "\n" + sectionOption + "\n" + sectionBuiltinStruct + "\n\n" +
    getGlobalMetadata() + "\n" + getBlockMetadata() + "\n" + getActions() +
    "\n" + getChecks() + "\n" + getTriggers();

#endif