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

namespace weapon {
const std::string keywordDefault = "default";
const std::string keywordKungFu = "kungfu";
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
                    {keywordKungFu, keywordKungFu},
                    {keywordFist, keywordFist},
                    {keywordFistZh, keywordFist},
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

} // namespace keyword

#endif