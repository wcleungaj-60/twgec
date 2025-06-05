#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>
#include <vector>

inline std::string join(const std::vector<std::string> &elements,
                        const std::string &separator) {
  std::ostringstream result;
  for (size_t i = 0; i < elements.size(); ++i) {
    result << elements[i];
    if (i < elements.size() - 1)
      result << separator;
  }
  return result.str();
}

inline std::string
join(const std::vector<std::pair<std::string, std::string>> &pairs,
     const std::string &outerSeparator,
     const std::string &innerSeparator = "=") {
  std::ostringstream result;
  for (size_t i = 0; i < pairs.size(); ++i) {
    result << pairs[i].first << innerSeparator << pairs[i].second;
    if (i < pairs.size() - 1)
      result << outerSeparator;
  }
  return result.str();
}

inline std::string inden(int _) { return std::string(_, ' '); }

#endif