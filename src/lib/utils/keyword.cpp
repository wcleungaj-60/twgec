#include "utils/keyword.h"
#include <iostream>
#include <string>

using namespace keyword;

bool KeywordEnum::isEmpty() { return codegenMap.empty(); }

const std::pair<bool, std::string>
KeywordEnum::get(std::string key, std::map<string, string> extraEnum) {
  if (codegenMap.find(key) != codegenMap.end()) {
    return {true, codegenMap.at(key)};
  }
  if (extraEnum.find(key) != extraEnum.end()) {
    return {true, extraEnum.at(key)};
  }
  std::cerr << "Codegen Warnings: Found invalid `" << enumType
            << "` enumeration \"" << key
            << "\". Please use the enumerations below:\n -> ";
  for (std::pair<std::string, std::string> pair : codegenMap)
    std::cerr << " " << pair.first;
  for (std::pair<std::string, std::string> pair : extraEnum)
    std::cerr << " " << pair.first;
  std::cerr << "\n";
  return {false, key};
}