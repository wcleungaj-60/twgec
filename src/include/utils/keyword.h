#ifndef KEYWORD_H
#define KEYWORD_H

#include <iostream>
#include <map>
#include <string>
#include <utility>

namespace keyword {

class KeywordEnum {
public:
  const std::string enumType;
  const std::map<std::string, std::string> codegenMap;
  KeywordEnum(const std::string enumType,
              const std::map<std::string, std::string> codegenMap)
      : enumType(enumType), codegenMap(codegenMap){};
  bool isEmpty() { return codegenMap.empty(); }
  const std::pair<bool, std::string> get(std::string key,
                                         bool warningEn = true) {
    if (codegenMap.find(key) != codegenMap.end()) {
      return {true, codegenMap.at(key)};
    }
    if (warningEn) {
      std::cerr << "Codegen Warnings: Found invalid " << enumType
                << " enumeration \"" << key
                << "\". Please use the enumerations below:\n -> ";
      for (std::pair<std::string, std::string> pair : codegenMap)
        std::cerr << " " << pair.first;
      std::cerr << "\n";
    }
    return {false, key};
  }
};

namespace camp {
const std::string keywordSkydow = "skydow";
const std::string keywordSkydowZh = "天影";
const std::string keywordRoyal = "royal";
const std::string keywordRoyalZh = "皇家";
const std::string keywordThird = "third";
const std::string keywordThirdZh = "第三";
const std::string keywordCivil = "civil";
const std::string keywordCivilZh = "平民";
static KeywordEnum keywordEnum("camp", {
                                           {keywordSkydow, keywordSkydow},
                                           {keywordSkydowZh, keywordSkydow},
                                           {keywordRoyal, keywordRoyal},
                                           {keywordRoyalZh, keywordRoyal},
                                           {keywordThird, keywordThird},
                                           {keywordThirdZh, keywordThird},
                                           {keywordCivil, keywordCivil},
                                           {keywordCivilZh, keywordCivil},
                                       });
} // namespace camp

namespace campMissionComplete {
const std::string keywordSkydow = "skydow";
const std::string keywordSkydowZh = "天影";
const std::string keywordRoyal = "royal";
const std::string keywordRoyalZh = "皇家";
const std::string keywordThird = "third";
const std::string keywordThirdZh = "第三";
const std::string keywordAll = "all";
const std::string keywordAllZh = "所有";
static KeywordEnum keywordEnum("camp", {
                                           {keywordSkydow, keywordSkydow},
                                           {keywordSkydowZh, keywordSkydow},
                                           {keywordRoyal, keywordRoyal},
                                           {keywordRoyalZh, keywordRoyal},
                                           {keywordThird, keywordThird},
                                           {keywordThirdZh, keywordThird},
                                           {keywordAll, keywordAll},
                                           {keywordAllZh, keywordAll},
                                       });
} // namespace campMissionComplete

namespace weapon {
const std::string keywordDefault = "default";
const std::string keywordKongFu = "kongfu";
const std::string keywordFist = "fist";
const std::string keywordFistZh = "拳頭";
const std::string keywordKnife = "knife";
const std::string keywordKnifeZh = "小刀";
const std::string keywordHook = "hook";
const std::string keywordHookZh = "爪子";
const std::string keywordSword = "sword";
const std::string keywordSwordZh = "大刀";
const std::string keywordShield = "shield";
const std::string keywordShieldZh = "盾牌";
const std::string keywordHandgun = "handgun";
const std::string keywordHandgunZh = "手槍";
const std::string keywordRifle = "rifle";
const std::string keywordRifleZh = "步槍";
const std::string keywordSnipegun = "snipegun";
const std::string keywordSnipegunZh = "狙擊槍";
const std::string keywordShotgun = "shotgun";
const std::string keywordShotgunZh = "霰彈槍";
const std::string keywordMinigun = "minigun";
const std::string keywordMinigunZh = "迷你砲機槍";
const std::string keywordBite = "bite";
const std::string keywordBiteZh = "屍爪";
const std::string keywordGiantAxe = "giantAxe";
const std::string keywordGiantAxeZh = "雙刃巨斧";
const std::string keywordIceSaber = "iceSaber";
const std::string keywordIceSaberZh = "冰劍";
const std::string keywordRocketLauncher = "rocketLauncher";
const std::string keywordRocketLauncherZh = "導航火箭炮";
const std::string keywordBowSet = "bowSet";
const std::string keywordBowSetZh = "水晶弓";
const std::string keywordDoubleGuns = "doubleGuns";
const std::string keywordDoubleGunsZh = "白銀雙鷹";
const std::string keywordLaserGun = "laserGun";
const std::string keywordLaserGunZh = "激光槍";
const std::string keywordAnaestheticRifle = "anaestheticRifle";
const std::string keywordAnaestheticRifleZh = "麻醉槍";
const std::string keywordShurikens = "shurikens";
const std::string keywordShurikensZh = "手裏劍";
const std::string keywordNunchaku = "nunchaku";
const std::string keywordNunchakuZh = "雙截刃";
const std::string keywordScythe = "scythe";
const std::string keywordScytheZh = "飛鎖鐮刀";
static KeywordEnum
    keywordEnum("weapon",
                {
                    {keywordDefault, keywordDefault},
                    {keywordKongFu, keywordKongFu},
                    {keywordFist, keywordKongFu},
                    {keywordFistZh, keywordKongFu},
                    {keywordKnife, keywordKnife},
                    {keywordKnifeZh, keywordKnife},
                    {keywordHook, keywordHook},
                    {keywordHookZh, keywordHook},
                    {keywordSword, keywordSword},
                    {keywordSwordZh, keywordSword},
                    {keywordShield, keywordShield},
                    {keywordShieldZh, keywordShield},
                    {keywordHandgun, keywordHandgun},
                    {keywordHandgunZh, keywordHandgun},
                    {keywordRifle, keywordRifle},
                    {keywordRifleZh, keywordRifle},
                    {keywordSnipegun, keywordSnipegun},
                    {keywordSnipegunZh, keywordSnipegun},
                    {keywordShotgun, keywordShotgun},
                    {keywordShotgunZh, keywordShotgun},
                    {keywordMinigun, keywordMinigun},
                    {keywordMinigunZh, keywordMinigun},
                    {keywordBite, keywordBite},
                    {keywordBiteZh, keywordBite},
                    {keywordGiantAxe, keywordGiantAxe},
                    {keywordGiantAxeZh, keywordGiantAxe},
                    {keywordIceSaber, keywordIceSaber},
                    {keywordIceSaberZh, keywordIceSaber},
                    {keywordRocketLauncher, keywordRocketLauncher},
                    {keywordRocketLauncherZh, keywordRocketLauncher},
                    {keywordBowSet, keywordBowSet},
                    {keywordBowSetZh, keywordBowSet},
                    {keywordDoubleGuns, keywordDoubleGuns},
                    {keywordDoubleGunsZh, keywordDoubleGuns},
                    {keywordLaserGun, keywordLaserGun},
                    {keywordLaserGunZh, keywordLaserGun},
                    {keywordAnaestheticRifle, keywordAnaestheticRifle},
                    {keywordAnaestheticRifleZh, keywordAnaestheticRifle},
                    {keywordShurikens, keywordShurikens},
                    {keywordShurikensZh, keywordShurikens},
                    {keywordNunchaku, keywordNunchaku},
                    {keywordNunchakuZh, keywordNunchaku},
                    {keywordScythe, keywordScythe},
                    {keywordScytheZh, keywordScythe},
                });
} // namespace weapon

namespace role {
const std::string keywordRole0 = "影俠";
const std::string keywordRole1 = "影娘";
const std::string keywordRole2 = "天影武士1";
const std::string keywordRole3 = "天影武士2";
const std::string keywordRole4 = "皇家騎士";
const std::string keywordRole5 = "公主騎士";
const std::string keywordRole6 = "皇家生化人";
const std::string keywordRole7 = "皇家機械人";
const std::string keywordRole8 = "男傭兵";
const std::string keywordRole9 = "女傭兵";
const std::string keywordRole10 = "黑森盜賊團首領";
const std::string keywordRole11 = "盜賊隊長";
const std::string keywordRole12 = "盜賊嘍囉";
const std::string keywordRole13 = "男平民";
const std::string keywordRole14 = "女平民";
const std::string keywordRole15 = "平民老人";
const std::string keywordRole16 = "平民小孩";
const std::string keywordRole17 = "天影士兵";
const std::string keywordRole18 = "皇家士兵";
const std::string keywordRole19 = "克拉瑪族男性";
const std::string keywordRole20 = "克拉瑪族女性";
const std::string keywordRole21 = "晶石怪";
const std::string keywordRole22 = "石怪";
const std::string keywordRole23 = "冰石怪";
const std::string keywordRole24 = "伊蒂絲";
const std::string keywordRole25 = "伊蒂絲(戴眼鏡)";
const std::string keywordRole26 = "冰兒";
const std::string keywordRole27 = "role27"; // 冰魂
const std::string keywordRole28 = "冰妖王";
const std::string keywordRole29 = "阿薩斯";
const std::string keywordRole30 = "研究員查理";
const std::string keywordRole31 = "冰魂";
const std::string keywordRole32 = "隱居士";
const std::string keywordRole33 = "馬茲席塔";
const std::string keywordRole34 = "隱之墨";
const std::string keywordRole35 = "隱心";
const std::string keywordRole36 = "七月教教主";
const std::string keywordRole37 = "馬茲席塔元精";
const std::string keywordRole38 = "馬茲席塔元靈";
const std::string keywordRole39 = "兔子";
const std::string keywordRole40 = "鹿";
const std::string keywordRole41 = "藍猩猩";
const std::string keywordRole42 = "憤怒藍猩猩";
const std::string keywordRole43 = "男傭兵2";
const std::string keywordRole44 = "女傭兵2";
const std::string keywordRole45 = "阿波羅元帥";
const std::string keywordRole46 = "亞提米絲";
const std::string keywordRole47 = "黑炎軍士";
const std::string keywordRole48 = "黑炎軍相";
const std::string keywordRole49 = "黑炎軍馬";
const std::string keywordRole50 = "黑炎軍卒";
const std::string keywordRole51 = "男靈能訓練官";
const std::string keywordRole52 = "女靈能訓練官";
const std::string keywordRole53 = "阿薩斯研究員";
const std::string keywordRole54 = "九方一色";
const std::string keywordRole55 = "靈骨";
const std::string keywordRole56 = "獵豹";
const std::string keywordRole57 = "隱形人";
const std::string keywordRole58 = "隱形精靈";
const std::string keywordRole59 = "破腦屍怪";
const std::string keywordRole60 = "影破腦";
const std::string keywordRole61 = "影破腦屍怪";
const std::string keywordRole62 = "肥腦屍怪";
const std::string keywordRole63 = "影肥腦屍怪";
const std::string keywordRole64 = "瘋狗屍";
const std::string keywordRole65 = "千年屍妖";
const std::string keywordRole66 = "噁吐屍妖";
const std::string keywordRole67 = "青面鬼";
const std::string keywordRole68 = "屍血蟲";
const std::string keywordRole69 = "role71"; // 躺平的阿波羅姐姐
const std::string keywordRole70 = "role72"; // 不認識
const std::string keywordRole71 = "role73"; // 赤焰精靈 (重複)
const std::string keywordRole72 = "熊";
const std::string keywordRole73 = "飛魚";
const std::string keywordRole74 = "赤焰王女";
const std::string keywordRole75 = "赤焰王女(躺下)";
const std::string keywordRole76 = "不動佛";
const std::string keywordRole77 = "赤焰精靈";
const std::string keywordRole78 = "火球";
const std::string keywordRole79 = "杭特博士";
const std::string keywordRole80 = "杭特博士(低頭)";
const std::string keywordRole81 = "崔酒中尉";
const std::string keywordRole82 = "role85"; // 老人
const std::string keywordRole83 = "role86"; // 白猩猩
const std::string keywordRole84 = "蜜蜂";
static KeywordEnum keywordEnum(
    "role",
    {
        {keywordRole0, "0"},   {keywordRole1, "1"},   {keywordRole2, "2"},
        {keywordRole3, "3"},   {keywordRole4, "4"},   {keywordRole5, "5"},
        {keywordRole6, "6"},   {keywordRole7, "7"},   {keywordRole8, "8"},
        {keywordRole9, "9"},   {keywordRole10, "10"}, {keywordRole11, "11"},
        {keywordRole12, "12"}, {keywordRole13, "13"}, {keywordRole14, "14"},
        {keywordRole15, "15"}, {keywordRole16, "16"}, {keywordRole17, "17"},
        {keywordRole18, "18"}, {keywordRole19, "19"}, {keywordRole20, "20"},
        {keywordRole21, "21"}, {keywordRole22, "22"}, {keywordRole23, "23"},
        {keywordRole24, "24"}, {keywordRole25, "25"}, {keywordRole26, "26"},
        {keywordRole27, "27"}, {keywordRole28, "28"}, {keywordRole29, "29"},
        {keywordRole30, "30"}, {keywordRole31, "31"}, {keywordRole32, "32"},
        {keywordRole33, "33"}, {keywordRole34, "34"}, {keywordRole35, "35"},
        {keywordRole36, "36"}, {keywordRole37, "37"}, {keywordRole38, "38"},
        {keywordRole39, "39"}, {keywordRole40, "40"}, {keywordRole41, "41"},
        {keywordRole42, "42"}, {keywordRole43, "43"}, {keywordRole44, "44"},
        {keywordRole45, "45"}, {keywordRole46, "46"}, {keywordRole47, "47"},
        {keywordRole48, "48"}, {keywordRole49, "49"}, {keywordRole50, "50"},
        {keywordRole51, "51"}, {keywordRole52, "52"}, {keywordRole53, "53"},
        {keywordRole54, "54"}, {keywordRole55, "55"}, {keywordRole56, "56"},
        {keywordRole57, "57"}, {keywordRole58, "58"}, {keywordRole59, "59"},
        {keywordRole60, "60"}, {keywordRole61, "61"}, {keywordRole62, "62"},
        {keywordRole63, "63"}, {keywordRole64, "64"}, {keywordRole65, "65"},
        {keywordRole66, "66"}, {keywordRole67, "67"}, {keywordRole68, "68"},
        {keywordRole69, "69"}, {keywordRole70, "70"}, {keywordRole71, "71"},
        {keywordRole72, "72"}, {keywordRole73, "73"}, {keywordRole74, "74"},
        {keywordRole75, "75"}, {keywordRole76, "76"}, {keywordRole77, "77"},
        {keywordRole78, "78"}, {keywordRole79, "79"}, {keywordRole80, "80"},
        {keywordRole81, "81"}, {keywordRole82, "82"}, {keywordRole83, "83"},
        {keywordRole84, "84"},
    });
} // namespace role

namespace config {
// Int
const std::string stageWidth = "stageWidth";
const std::string stageHeight = "stageHeight";
const std::string roomSize = "roomSize";
const std::string lives = "lives";
const std::string maxAbilityLevel = "maxAbilityLevel";
const std::string bornDuration = "bornDuration";
const std::string bornLockDuration = "bornLockDuration";
const std::string minPlayers = "minPlayers";
// Boolean
const std::string supportSignin = "supportSignin";
const std::string mustLogin = "mustLogin";
const std::string allowGuest = "allowGuest";
const std::string supportMsgServer = "supportMsgServer";
const std::string runGame = "runGame";
const std::string campOpSkydow = "campOpSkydow";
const std::string campOpRoyal = "campOpRoyal";
const std::string campOpThird = "campOpThird";
const std::string setInitFocus = "setInitFocus";
const std::string setBornDuration = "setBornDuration";
const std::string nextGameEnabled = "nextGameEnabled";
const std::string playDefaultMusic = "playDefaultMusic";
const std::string disableNextGameOnMissionComplete =
    "disableNextGameOnMissionComplete";
const std::string useDefaultItems = "useDefaultItems";
const std::string defCarryItems = "defCarryItems";
const std::string useDefaultCampLocs = "useDefaultCampLocs";
const std::string useCustomWeapons = "useCustomWeapons";
const std::string useCustomFarWeapons = "useCustomFarWeapons";
const std::string useCustomItems = "useCustomItems";
// String
const std::string title = "title";
const std::string map = "map";
const std::string schema = "schema";
const std::string stageBackgroundColor = "stageBackgroundColor";
const std::string gamezoneCode = "gamezoneCode";
// List
const std::string skydowLocs = "skydowLocs";
const std::string royalLocs = "royalLocs";
const std::string thirdLocs = "thirdLocs";
const std::string initFocus = "initFocus";
const std::string preloadSources = "preloadSources";
const std::string preloadResourcesExclude = "preloadResourcesExclude";
const std::string carryItemCodes = "carryItemCodes";
// Unsupport
const std::string customWeapons = "customWeapons";
const std::string customFarWeapons = "customFarWeapons";
const std::string customItems = "customItems";
} // namespace config

namespace valueType {
const std::string keywordString = "string";
const std::string keywordInteger = "integer";
const std::string keywordInt = "int";
const std::string keywordNumber = "number";
const std::string keywordNum = "num";
const std::string keywordBoolean = "boolean";
const std::string keywordBool = "bool";
const std::string keywordJson = "json";
static KeywordEnum keywordEnum("value type",
                               {
                                   {keywordString, keywordString},
                                   {keywordInteger, keywordInteger},
                                   {keywordInt, keywordInteger},
                                   {keywordNumber, keywordNumber},
                                   {keywordNum, keywordNumber},
                                   {keywordBoolean, keywordBoolean},
                                   {keywordBool, keywordBoolean},
                                   {keywordJson, keywordJson},
                               });
} // namespace valueType

namespace matchKind {
const std::string keywordContain = "contain";
const std::string keywordEqual = "equal";
const std::string keywordRegexp = "regexp";
const std::string keywordRegex = "regex";
static KeywordEnum keywordEnum("match kind",
                               {
                                   {keywordContain, keywordContain},
                                   {keywordEqual, keywordEqual},
                                   {keywordRegexp, keywordRegexp},
                                   {keywordRegex, keywordRegexp},
                               });
} // namespace matchKind

namespace ability {
const std::string keywordEnblastShield = "enblastShield";
const std::string keywordWarp = "warp";
const std::string keywordBugform = "bugform";
const std::string keywordGiant = "giant";
const std::string keywordShrink = "shrink";
const std::string keywordShocked = "shocked";
const std::string keywordAnaestheticShot = "anaestheticShot";
const std::string keywordDizzy = "dizzy";
const std::string keywordDrain = "drain";
const std::string keywordFireShield = "fireShield";
const std::string keywordHiddenEyes = "hiddenEyes";
const std::string keywordHelong = "helong";
const std::string keywordIceShocked = "iceShocked";
const std::string keywordJump = "jump";
const std::string keywordNinjatsu = "ninjatsu";
const std::string keywordNunchakuPower = "nunchakuPower";
const std::string keywordRaged = "raged";
const std::string keywordRebirth = "rebirth";
const std::string keywordShield = "shield";
const std::string keywordEagleEyes = "eagleEyes";
const std::string keywordShieldRepair = "shieldRepair";
const std::string keywordShieldWave = "shieldWave";
const std::string keywordTornado = "tornado";
const std::string keywordTornadoUp = "tornadoUp";
const std::string keywordZombie = "zombie";
const std::string keywordSkeleton = "skeleton";
const std::string keywordTreeman = "treeman";
const std::string keywordSoulsplit = "soulsplit";
const std::string keywordDazed = "dazed";
const std::string keywordAxeWhirl = "axeWhirl";
const std::string keywordSurroundFrisbee = "surroundFrisbee";
const std::string keywordNineDragons = "nineDragons";
const std::string keywordSkyJumps = "skyJumps";
const std::string keywordDarkRainstorm = "darkRainstorm";
const std::string keywordEnblastShieldZh = "光牙風甲";
const std::string keywordWarpZh = "閃現術";
const std::string keywordBugformZh = "蟲化術";
const std::string keywordGiantZh = "巨人術";
const std::string keywordShrinkZh = "縮骨功";
const std::string keywordShockedZh = "被震暈";
const std::string keywordAnaestheticShotZh = "被麻醉";
const std::string keywordDizzyZh = "暈眩";
const std::string keywordDrainZh = "吸血術";
const std::string keywordFireShieldZh = "天火護甲";
const std::string keywordHiddenEyesZh = "天隱之眼";
const std::string keywordHelongZh = "黑龍領主";
const std::string keywordIceShockedZh = "被冰凍";
const std::string keywordJumpZh = "輕功跳躍";
const std::string keywordNinjatsuZh = "分身術";
const std::string keywordNunchakuPowerZh = "雙刃亂舞";
const std::string keywordRagedZh = "血刃";
const std::string keywordRebirthZh = "爆血轉生";
const std::string keywordShieldZh = "防護罩";
const std::string keywordEagleEyesZh = "鷹眼";
const std::string keywordShieldRepairZh = "修復盾牌";
const std::string keywordShieldWaveZh = "盾衝擊波";
const std::string keywordTornadoZh = "真氣風暴";
const std::string keywordTornadoUpZh = "被真氣風暴捲入";
const std::string keywordZombieZh = "噬魂秘藥";
const std::string keywordSkeletonZh = "響骨衣";
const std::string keywordTreemanZh = "樹人";
const std::string keywordSoulsplitZh = "靈魂出竅";
const std::string keywordDazedZh = "眼冒金星";
const std::string keywordAxeWhirlZh = "旋風斧";
const std::string keywordSurroundFrisbeeZh = "護身迴旋鏢";
const std::string keywordNineDragonsZh = "九龍閃";
const std::string keywordSkyJumpsZh = "影天縱";
const std::string keywordDarkRainstormZh = "黑炎暴雨";

static KeywordEnum
    keywordEnum("ability",
                {
                    {keywordEnblastShield, keywordEnblastShield},
                    {keywordWarp, keywordWarp},
                    {keywordBugform, keywordBugform},
                    {keywordGiant, keywordGiant},
                    {keywordShrink, keywordShrink},
                    {keywordShocked, keywordShocked},
                    {keywordAnaestheticShot, keywordAnaestheticShot},
                    {keywordDizzy, keywordDizzy},
                    {keywordDrain, keywordDrain},
                    {keywordFireShield, keywordFireShield},
                    {keywordHiddenEyes, keywordHiddenEyes},
                    {keywordHelong, keywordHelong},
                    {keywordIceShocked, keywordIceShocked},
                    {keywordJump, keywordJump},
                    {keywordNinjatsu, keywordNinjatsu},
                    {keywordNunchakuPower, keywordNunchakuPower},
                    {keywordRaged, keywordRaged},
                    {keywordRebirth, keywordRebirth},
                    {keywordShield, keywordShield},
                    {keywordEagleEyes, keywordEagleEyes},
                    {keywordShieldRepair, keywordShieldRepair},
                    {keywordShieldWave, keywordShieldWave},
                    {keywordTornado, keywordTornado},
                    {keywordTornadoUp, keywordTornadoUp},
                    {keywordZombie, keywordZombie},
                    {keywordSkeleton, keywordSkeleton},
                    {keywordTreeman, keywordTreeman},
                    {keywordSoulsplit, keywordSoulsplit},
                    {keywordDazed, keywordDazed},
                    {keywordAxeWhirl, keywordAxeWhirl},
                    {keywordSurroundFrisbee, keywordSurroundFrisbee},
                    {keywordNineDragons, keywordNineDragons},
                    {keywordSkyJumps, keywordSkyJumps},
                    {keywordDarkRainstorm, keywordDarkRainstorm},
                    {keywordEnblastShieldZh, keywordEnblastShield},
                    {keywordWarpZh, keywordWarp},
                    {keywordBugformZh, keywordBugform},
                    {keywordGiantZh, keywordGiant},
                    {keywordShrinkZh, keywordShrink},
                    {keywordShockedZh, keywordShocked},
                    {keywordAnaestheticShotZh, keywordAnaestheticShot},
                    {keywordDizzyZh, keywordDizzy},
                    {keywordDrainZh, keywordDrain},
                    {keywordFireShieldZh, keywordFireShield},
                    {keywordHiddenEyesZh, keywordHiddenEyes},
                    {keywordHelongZh, keywordHelong},
                    {keywordIceShockedZh, keywordIceShocked},
                    {keywordJumpZh, keywordJump},
                    {keywordNinjatsuZh, keywordNinjatsu},
                    {keywordNunchakuPowerZh, keywordNunchakuPower},
                    {keywordRagedZh, keywordRaged},
                    {keywordRebirthZh, keywordRebirth},
                    {keywordShieldZh, keywordShield},
                    {keywordEagleEyesZh, keywordEagleEyes},
                    {keywordShieldRepairZh, keywordShieldRepair},
                    {keywordShieldWaveZh, keywordShieldWave},
                    {keywordTornadoZh, keywordTornado},
                    {keywordTornadoUpZh, keywordTornadoUp},
                    {keywordZombieZh, keywordZombie},
                    {keywordSkeletonZh, keywordSkeleton},
                    {keywordTreemanZh, keywordTreeman},
                    {keywordSoulsplitZh, keywordSoulsplit},
                    {keywordDazedZh, keywordDazed},
                    {keywordAxeWhirlZh, keywordAxeWhirl},
                    {keywordSurroundFrisbeeZh, keywordSurroundFrisbee},
                    {keywordNineDragonsZh, keywordNineDragons},
                    {keywordSkyJumpsZh, keywordSkyJumps},
                    {keywordDarkRainstormZh, keywordDarkRainstorm},
                });
} // namespace ability

namespace _operator {
const std::string keywordGT = ">";
const std::string keywordGTE = ">=";
const std::string keywordLT = "<";
const std::string keywordLTE = "<=";
const std::string keywordEQ = "==";
const std::string keywordNE = "!=";
static KeywordEnum keywordEnum("operator", {
                                               {keywordGT, keywordGT},
                                               {keywordGTE, keywordGTE},
                                               {keywordLT, keywordLT},
                                               {keywordLTE, keywordLTE},
                                               {keywordEQ, keywordEQ},
                                               {keywordNE, keywordNE},
                                           });
} // namespace _operator

namespace followType {
const std::string keywordStop = "stop";
const std::string keywordStopZh = "停止跟隨";
const std::string keywordSelf = "self";
const std::string keywordSelfZh = "跟隨自己";
const std::string keywordActor = "actor";
const std::string keywordActorZh = "跟隨人物";
const std::string keywordClosestPlayer = "closestPlayer";
const std::string keywordClosestPlayerZh = "跟隨離最近的玩家角色";
const std::string keywordClosestFriendPlayer = "closestFriendPlayer";
const std::string keywordClosestFriendPlayerZh = "跟隨離最近的友好玩家";
const std::string keywordClosestEnemyPlayer = "closestEnemyPlayer";
const std::string keywordClosestEnemyPlayerZh = "跟隨離最近的敵方玩家";
static KeywordEnum
    keywordEnum("followType",
                {
                    {keywordStop, keywordStop},
                    {keywordStopZh, keywordStop},
                    {keywordSelf, keywordSelf},
                    {keywordSelfZh, keywordSelf},
                    {keywordActor, keywordActor},
                    {keywordActorZh, keywordActor},
                    {keywordClosestPlayer, keywordClosestPlayer},
                    {keywordClosestPlayerZh, keywordClosestPlayer},
                    {keywordClosestFriendPlayer, keywordClosestFriendPlayer},
                    {keywordClosestFriendPlayerZh, keywordClosestFriendPlayer},
                    {keywordClosestEnemyPlayer, keywordClosestEnemyPlayer},
                    {keywordClosestEnemyPlayerZh, keywordClosestEnemyPlayer},
                });
} // namespace followType

namespace keyPressTiming {
const std::string keywordPressed = "pressed";
const std::string keywordPressedZh = "點擊";
const std::string keywordDown = "down";
const std::string keywordDownZh = "按下";
const std::string keywordRelease = "release";
const std::string keywordReleaseZh = "放開";
static KeywordEnum keywordEnum("keyPressTiming",
                               {
                                   {keywordPressed, keywordPressed},
                                   {keywordPressedZh, keywordPressed},
                                   {keywordDown, keywordDown},
                                   {keywordDownZh, keywordDown},
                                   {keywordRelease, keywordRelease},
                                   {keywordReleaseZh, keywordRelease},
                               });
} // namespace keyPressTiming

namespace keyPressKey {
const std::string keywordA = "A";
const std::string keywordB = "B";
const std::string keywordC = "C";
const std::string keywordD = "D";
const std::string keywordE = "E";
const std::string keywordF = "F";
const std::string keywordG = "G";
const std::string keywordH = "H";
const std::string keywordI = "I";
const std::string keywordJ = "J";
const std::string keywordK = "K";
const std::string keywordL = "L";
const std::string keywordM = "M";
const std::string keywordN = "N";
const std::string keywordO = "O";
const std::string keywordP = "P";
const std::string keywordQ = "Q";
const std::string keywordR = "R";
const std::string keywordS = "S";
const std::string keywordT = "T";
const std::string keywordU = "U";
const std::string keywordV = "V";
const std::string keywordW = "W";
const std::string keywordX = "X";
const std::string keywordY = "Y";
const std::string keywordZ = "Z";
static KeywordEnum keywordEnum(
    "keyPressKey",
    {{keywordA, "65"}, {keywordB, "66"}, {keywordC, "67"}, {keywordD, "68"},
     {keywordE, "69"}, {keywordF, "70"}, {keywordG, "71"}, {keywordH, "72"},
     {keywordI, "73"}, {keywordJ, "74"}, {keywordK, "75"}, {keywordL, "76"},
     {keywordM, "77"}, {keywordN, "78"}, {keywordO, "79"}, {keywordP, "80"},
     {keywordQ, "81"}, {keywordR, "82"}, {keywordS, "83"}, {keywordT, "84"},
     {keywordU, "85"}, {keywordV, "86"}, {keywordW, "87"}, {keywordX, "88"},
     {keywordY, "89"}, {keywordZ, "90"}});
} // namespace keyPressKey

namespace actorAttrKind {
const std::string keywordMaxHp = "maxHp";
const std::string keywordMaxHpZh = "最大血量"; // 1 ~ 99999
const std::string keywordMinHp = "minHp";
const std::string keywordMinHpZh = "最低血量"; // 0 ~ 99999
const std::string keywordHp = "hp";
const std::string keywordHpZh = "血量"; // 0 ~ 999999
const std::string keywordMp = "mp";
const std::string keywordMpZh = "氣"; // 0 ~ 100
const std::string keywordGroup = "group";
const std::string keywordGroupZh = "團隊號碼"; // -9999 ~ 9999
const std::string keywordWeight = "weight";
const std::string keywordWeightZh = "體重"; // -9999 ~ 9999
const std::string keywordStrength = "strength";
const std::string keywordStrengthZh = "拳頭傷害力"; // -9999 ~ 9999
const std::string keywordLumi = "lumi";
const std::string keywordLumiZh = "發光範圍"; // -1 ~ 9
const std::string keywordPoisonDmgInt = "poisonDmgInt";
const std::string keywordPoisonDmgIntZh = "中毒受傷間隔"; // 100 ~ 999999
const std::string keywordReputation = "reputation";
const std::string keywordReputationZh = "陣營聲望"; // 0 ~ 10000
const std::string keywordBodyScale = "bodyScale";
const std::string keywordBodyScaleZh = "體型縮放"; // 0.1 ~ 3
const std::string keywordHitChargeScale = "hitChargeScale";
const std::string keywordHitChargeScaleZh = "受傷時的回氣率"; // 0 ~ 10
const std::string keywordWarpStunPower = "warpStunPower";
const std::string keywordWarpStunPowerZh = "閃現術的閃光強度"; // 0 ~ 1
const std::string keywordCloseWeaponSpeed = "closeWeaponSpeed";
const std::string keywordCloseWeaponSpeedZh =
    "近身武器的揮動速度"; // 0.01 ~ 100
static KeywordEnum
    keywordEnum("actorAttribute",
                {
                    {keywordMaxHp, keywordMaxHp},
                    {keywordMaxHpZh, keywordMaxHp},
                    {keywordMinHp, keywordMinHp},
                    {keywordMinHpZh, keywordMinHp},
                    {keywordHp, keywordHp},
                    {keywordHpZh, keywordHp},
                    {keywordMp, keywordMp},
                    {keywordMpZh, keywordMp},
                    {keywordGroup, keywordGroup},
                    {keywordGroupZh, keywordGroup},
                    {keywordWeight, keywordWeight},
                    {keywordWeightZh, keywordWeight},
                    {keywordStrength, keywordStrength},
                    {keywordStrengthZh, keywordStrength},
                    {keywordLumi, keywordLumi},
                    {keywordLumiZh, keywordLumi},
                    {keywordPoisonDmgInt, keywordPoisonDmgInt},
                    {keywordPoisonDmgIntZh, keywordPoisonDmgInt},
                    {keywordReputation, keywordReputation},
                    {keywordReputationZh, keywordReputation},
                    {keywordBodyScale, keywordBodyScale},
                    {keywordBodyScaleZh, keywordBodyScale},
                    {keywordHitChargeScale, keywordHitChargeScale},
                    {keywordHitChargeScaleZh, keywordHitChargeScale},
                    {keywordWarpStunPower, keywordWarpStunPower},
                    {keywordWarpStunPowerZh, keywordWarpStunPower},
                    {keywordCloseWeaponSpeed, keywordCloseWeaponSpeed},
                    {keywordCloseWeaponSpeedZh, keywordCloseWeaponSpeed},
                });
} // namespace actorAttrKind

namespace actorBrainKind {
const std::string keywordAI = "ai";
const std::string keywordAIZh = "電腦";
const std::string keywordPlayer = "player";
const std::string keywordPlayerZh = "玩家";
const std::string keywordAll = "all";
const std::string keywordAllZh = "全部";
static KeywordEnum keywordEnum("actor controller kind",
                               {
                                   {keywordAI, keywordAI},
                                   {keywordAIZh, keywordAI},
                                   {keywordPlayer, keywordPlayer},
                                   {keywordPlayerZh, keywordPlayer},
                                   {keywordAll, keywordAll},
                                   {keywordAllZh, keywordAll},
                               });
} // namespace actorBrainKind

namespace deltaHpKind {
const std::string keywordHeal = "heal";
const std::string keywordHealZh = "治療";
const std::string keywordDamage = "damage";
const std::string keywordDamageZh = "流血受傷";
static KeywordEnum keywordEnum("actor controller kind",
                               {
                                   {keywordHeal, keywordHeal},
                                   {keywordHealZh, keywordHeal},
                                   {keywordDamage, keywordDamage},
                                   {keywordDamageZh, keywordDamage},
                               });
} // namespace actorBrainKind

} // namespace keyword

#endif