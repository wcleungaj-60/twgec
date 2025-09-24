#ifndef OPTION_H
#define OPTION_H

#include "option/help.h"
#include <iostream>
#include <string>

struct Option {
  bool printToken = false;
  bool printASTBefore = false;
  bool printASTAfter = false;
  bool help = false;
  std::string argFilePath = "";
  std::string outputFile = "game.events";
};

inline Option parseCommand(int argc, char *argv[]) {
  if (argc < 2)
    std::cerr << "Please run `twgec --help` for help.\n";
  Option option;
  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "--print-token") {
      option.printToken = true;
    } else if (std::string(argv[i]) == "--print-ast-all") {
      option.printASTBefore = true;
      option.printASTAfter = true;
    } else if (std::string(argv[i]) == "--print-ast-before") {
      option.printASTBefore = true;
    } else if (std::string(argv[i]) == "--print-ast-after") {
      option.printASTAfter = true;
    } else if (std::string(argv[i]) == "--help") {
      std::cerr << helpMessage << "\n";
    } else if (std::string(argv[i]).substr(0, 3) == "-o=") {
      option.outputFile = std::string(argv[i]).substr(3);
    } else if (option.argFilePath == "") {
      option.argFilePath = argv[i];
    } else {
      std::cerr << "Unknown arugment \"" << argv[i]
                << "\" found.\nPlease run `twgec --help` for help.\n";
    }
  }
  return option;
}

#endif