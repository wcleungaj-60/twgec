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
const std::string keywordKnife = "knife";
const std::string keywordHook = "hook";
const std::string keywordSword = "sword";
const std::string keywordShield = "shield";
const std::string keywordHandgun = "handgun";
const std::string keywordRifle = "rifle";
const std::string keywordSnipegun = "snipegun";
const std::string keywordShotgun = "shotgun";
const std::string keywordMinigun = "minigun";
const std::string keywordBite = "bite";
const std::string keywordGiantAxe = "giantAxe";
const std::string keywordIceSaber = "iceSaber";
static KeywordEnum keywordEnum("weapon", {
                                             {keywordDefault, keywordDefault},
                                             {keywordKungFu, keywordKungFu},
                                             {keywordFist, keywordKungFu},
                                             {keywordKnife, keywordKnife},
                                             {keywordHook, keywordHook},
                                             {keywordSword, keywordSword},
                                             {keywordShield, keywordShield},
                                             {keywordHandgun, keywordHandgun},
                                             {keywordRifle, keywordRifle},
                                             {keywordSnipegun, keywordSnipegun},
                                             {keywordShotgun, keywordShotgun},
                                             {keywordMinigun, keywordMinigun},
                                             {keywordBite, keywordBite},
                                             {keywordGiantAxe, keywordGiantAxe},
                                             {keywordIceSaber, keywordIceSaber},
                                         });
} // namespace weapon

} // namespace keyword

#endif