#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <cctype>
#include <string>
#include <vector>

class Lexer {
public:
  Lexer(std::string filename, const std::string &input)
      : filename(filename), input(input), pos(0) {}
  std::vector<Token> getTokens();
  static bool raiseLexicalError(std::vector<Token> tokens);
  static void print(std::vector<Token> tokens);

private:
  static int line;
  static int column;
  std::string input;
  std::string filename;
  size_t pos;

  Token nextToken();
  Token metadataToken();
  Token commentToken();
  Token integerToken();
  Token stringToken();
  Token identifierToken();
  Token keywordToken(std::string keyword);

  inline bool matchKeyword(std::string keyword);
  inline bool next(char character, unsigned offset = 1);
};

#endif