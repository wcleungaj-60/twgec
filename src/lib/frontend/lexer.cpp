#include "frontend/lexer.h"

namespace keyword {
std::string block = "block";
std::string actions = "actions";
std::string triggers = "triggers";
std::string checks = "checks";
} // namespace keyword

Token Lexer::nextToken() {
  while (pos < input.length()) {
    char current = input[pos];
    switch (current) {
    case ' ':
    case '\t':
    case '\n':
      pos++;
      continue;
    case '"':
      return stringToken();
    case '=':
      pos++;
      return Token(TokenType::ASSIGN);
    case '{':
      pos++;
      return Token(TokenType::OPENCUR);
    case '}':
      pos++;
      return Token(TokenType::CLOSECUR);
    case ';':
      pos++;
      return Token(TokenType::SEMICOLON);
    case 'a':
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::actions,
                                     TokenType::ACTIONS);
    case 'b':
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::block,
                                     TokenType::BLOCK);
    case 'c':
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::checks,
                                     TokenType::CHECKS);
    case 't':
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::triggers,
                                     TokenType::TRIGGERS);
    case '_':
      return metadataToken();
    default:
      if (std::isalpha(current))
        return identifierToken();
      else
        return Token(TokenType::UNKNOWN, std::string(1, input[pos++]));
    }
  }
  return Token(TokenType::END);
}

Token Lexer::metadataToken() {
  if (pos + 1 >= input.length() || input[pos++] != '_' || input[pos++] != '_')
    return Token(TokenType::UNKNOWN, std::string(1, input[pos++]));
  size_t start = pos;
  while (pos < input.length() && (isalnum(input[pos]) && input[pos] != '_'))
    pos++;
  if (pos + 1 >= input.length() || input[pos++] != '_' || input[pos++] != '_')
    return Token(TokenType::UNKNOWN, std::string(1, input[pos++]));
  return Token(TokenType::METADATA, input.substr(start, pos - 2 - start));
}

Token Lexer::stringToken() {
  size_t start = pos++;
  while (pos < input.length() && input[pos] != '"')
    pos++;
  if (pos < input.length())
    pos++; // Skip closing quote
  return Token(TokenType::STRING, input.substr(start, pos - start));
}

Token Lexer::identifierToken() {
  size_t start = pos;
  while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_'))
    pos++;
  return Token(TokenType::IDENTIFIER, input.substr(start, pos - start));
}
