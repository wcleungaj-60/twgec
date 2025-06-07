#ifndef LOCATION_H
#define LOCATION_H

#include <iostream>

struct Location {
  int line;
  int column;

  Location(int line, int column) : line(line), column(column) {}

  friend std::ostream &operator<<(std::ostream &os, const Location &location) {
    os << location.line << ":" << location.column;
    return os;
  }
};

#endif