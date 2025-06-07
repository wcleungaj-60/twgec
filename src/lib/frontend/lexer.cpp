#include "frontend/lexer.h"

namespace keyword {
std::string block = "block";
std::string actions = "actions";
std::string triggers = "triggers";
std::string checks = "checks";
} // namespace keyword

int Lexer::line = 1;
int Lexer::column = 1;

Token Lexer::nextToken() {
  while (pos < input.length()) {
    char current = input[pos];
    switch (current) {
    case ' ':
      Lexer::column++;
      pos++;
      continue;
    case '\t':
      Lexer::column += 4;
      pos++;
      continue;
    case '\n':
      Lexer::column = 1;
      Lexer::line++;
      pos++;
      continue;
    case '"':
      return stringToken();
    case '=':
      Lexer::column++;
      pos++;
      return Token(TokenType::ASSIGN, Lexer::line, Lexer::column - 1, "=");
    case '{':
      Lexer::column++;
      pos++;
      return Token(TokenType::OPENCUR, Lexer::line, Lexer::column - 1, "{");
    case '}':
      Lexer::column++;
      pos++;
      return Token(TokenType::CLOSECUR, Lexer::line, Lexer::column - 1, "}");
    case '(':
      Lexer::column++;
      pos++;
      return Token(TokenType::OPENPAR, Lexer::line, Lexer::column - 1, "(");
    case ')':
      Lexer::column++;
      pos++;
      return Token(TokenType::CLOSEPAR, Lexer::line, Lexer::column - 1, ")");
    case '.':
      Lexer::column++;
      pos++;
      return Token(TokenType::DOT, Lexer::line, Lexer::column - 1, ".");
    case ',':
      Lexer::column++;
      pos++;
      return Token(TokenType::COMMA, Lexer::line, Lexer::column - 1, ",");
    case ';':
      Lexer::column++;
      pos++;
      return Token(TokenType::SEMICOLON, Lexer::line, Lexer::column - 1, ";");
    case '_':
      return metadataToken();
    default:
      if (!std::isalpha(current))
        return Token(TokenType::UNKNOWN, Lexer::line, Lexer::column++,
                     std::string(1, input[pos++]));
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::actions,
                                     TokenType::ACTIONS);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::block,
                                     TokenType::BLOCK);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::checks,
                                     TokenType::CHECKS);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::triggers,
                                     TokenType::TRIGGERS);
      return identifierToken();
    }
  }
  return Token(TokenType::END, Lexer::line, Lexer::column - 1);
}

Token Lexer::metadataToken() {
  int startColumn = column;
  size_t start = pos;
  if (pos + 1 >= input.length() || input[pos++] != '_' || input[pos++] != '_') {
    column += 3;
    return Token(TokenType::UNKNOWN, Lexer::line, startColumn,
                 std::string(1, input[pos++]));
  }
  column += 2;
  while (pos < input.length() && (isalnum(input[pos]) && input[pos] != '_')) {
    column++;
    pos++;
  }
  if (pos + 1 >= input.length() || input[pos++] != '_' || input[pos++] != '_') {
    column += 3;
    return Token(TokenType::UNKNOWN, Lexer::line, startColumn,
                 std::string(1, input[pos++]));
  }
  column += 2;
  return Token(TokenType::METADATA, Lexer::line, startColumn,
               input.substr(start, pos - start));
}

Token Lexer::stringToken() {
  int startColumn = column;
  size_t start = pos++;
  column++;
  while (pos < input.length() && input[pos] != '"') {
    column++;
    pos++;
  }
  if (pos < input.length()) {
    column++;
    pos++; // Skip closing quote
  }
  return Token(TokenType::STRING, Lexer::line, startColumn,
               input.substr(start, pos - start));
}

Token Lexer::identifierToken() {
  size_t start = pos;
  int startColumn = column;
  while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_')) {
    column++;
    pos++;
  }
  return Token(TokenType::IDENTIFIER, Lexer::line, startColumn,
               input.substr(start, pos - start));
}
