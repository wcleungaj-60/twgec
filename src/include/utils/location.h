#ifndef LOCATION_H
#define LOCATION_H

#include <iostream>

struct Location {
  std::string filename;
  int line;
  int column;

  Location(std::string filename, int line, int column)
      : filename(filename), line(line), column(column) {}

  friend std::ostream &operator<<(std::ostream &os, const Location &location) {
    os << location.filename << ":" << location.line << ":" << location.column;
    return os;
  }
};

#endif