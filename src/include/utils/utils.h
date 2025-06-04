#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <sstream>

inline std::string join(const std::vector<std::string>& elements, const std::string& separator) {
    std::ostringstream result;
    for (size_t i = 0; i < elements.size(); ++i) {
        result << elements[i];
        if (i < elements.size() - 1)
            result << separator;
    }
    return result.str();
}

inline std::string inden(int _) { return std::string(_, ' '); }

#endif