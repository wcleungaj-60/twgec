#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <cctype>
#include <string>

#define LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword, type)              \
  if ((input).substr((pos), (keyword).length()) == (keyword) &&                \
      !isalpha((input)[(pos) + (keyword).length()])) {                         \
    int startColumn = Lexer::column;                                           \
    Lexer::column += (keyword).length();                                       \
    (pos) += (keyword).length();                                               \
    return Token(type, Lexer::line, startColumn, keyword);                     \
  }

class Lexer {
public:
  Lexer(const std::string &input) : input(input), pos(0) {}
  Token nextToken();

private:
  static int line;
  static int column;
  std::string input;
  size_t pos;

  Token metadataToken();
  Token commentToken();
  Token integerToken();
  Token stringToken();
  Token identifierToken();
};

#endif