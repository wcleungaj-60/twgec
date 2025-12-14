#include "frontend/lexer.h"
#include <algorithm>

namespace keyword {
std::string _true = "true";
std::string _false = "false";
std::string block = "block";
std::string actions = "actions";
std::string triggers = "triggers";
std::string checks = "checks";
std::string def = "def";
std::string _const = "const";
std::string _if = "if";
std::string _else = "else";
std::string _for = "for";
std::string in = "in";
std::string Point = "Point";
std::string ActorMatch = "ActorMatch";
std::string CustomWeapon = "CustomWeapon";
} // namespace keyword

int Lexer::line = 1;
int Lexer::column = 1;

std::vector<Token> Lexer::getTokens() {
  std::vector<Token> tokens;
  Token token = nextToken();

  while (token.type != TokenType::END) {
    tokens.push_back(token);
    token = nextToken();
  }

  tokens.erase(std::remove_if(tokens.begin(), tokens.end(),
                              [](const Token &token) {
                                return token.type == TokenType::COMMENT;
                              }),
               tokens.end());

  return tokens;
}

bool Lexer::raiseLexicalError(std::vector<Token> tokens) {
  bool hasLexicalError = false;
  for (const auto &t : tokens)
    if (t.type == TokenType::UNKNOWN) {
      hasLexicalError = true;
      std::vector<int> intArray;
      for (unsigned char c : t.value)
        intArray.push_back(static_cast<int>(c));
      std::cerr << "InvalidCharacterError: Unexpected character \'" << t.value
                << " (";
      for (int v : intArray)
        std::cerr << v << ", ";
      std::cerr << ")"
                << "\' at " << t.location << "\n";
    }
  return hasLexicalError;
}

void Lexer::print(std::vector<Token> tokens) {
  std::cout << "===== Print Tokens =====\n";
  for (const auto &t : tokens)
    std::cout << t << "\n";
}

Token Lexer::nextToken() {
  while (pos < input.length()) {
    char current = input[pos];
    if (current == ' ') {
      Lexer::column++;
      pos++;
    } else if (current == '\t') {
      Lexer::column += 4;
      pos++;
    } else if (current == '\n') {
      Lexer::column = 1;
      Lexer::line++;
      pos++;
    } else if (current == '"') {
      return stringToken();
    } else if (current == '=' && next('=')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::EQUAL, Lexer::line, Lexer::column - 2, "==");
    } else if (current == '=') {
      Lexer::column++;
      pos++;
      return Token(TokenType::ASSIGN, Lexer::line, Lexer::column - 1, "=");
    } else if (current == '>' && next('=')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::GREATER_THAN_EQUAL, Lexer::line,
                   Lexer::column - 2, ">=");
    } else if (current == '>') {
      Lexer::column++;
      pos++;
      return Token(TokenType::GREATER_THAN, Lexer::line, Lexer::column - 1,
                   ">");
    } else if (current == '<' && next('=')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::LESS_THAN_EQUAL, Lexer::line, Lexer::column - 2,
                   "<=");
    } else if (current == '<') {
      Lexer::column++;
      pos++;
      return Token(TokenType::LESS_THAN, Lexer::line, Lexer::column - 1, "<");
    } else if (current == '!' && next('=')) {
      // TODO: Support not operation
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::NOT_EQUAL, Lexer::line, Lexer::column - 2, "!=");
    } else if (current == '&' && next('&')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::AND, Lexer::line, Lexer::column - 2, "&&");
    } else if (current == '|' && next('|')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::OR, Lexer::line, Lexer::column - 2, "||");
    } else if (current == '{') {
      Lexer::column++;
      pos++;
      return Token(TokenType::OPENCUR, Lexer::line, Lexer::column - 1, "{");
    } else if (current == '}') {
      Lexer::column++;
      pos++;
      return Token(TokenType::CLOSECUR, Lexer::line, Lexer::column - 1, "}");
    } else if (current == '(') {
      Lexer::column++;
      pos++;
      return Token(TokenType::OPENPAR, Lexer::line, Lexer::column - 1, "(");
    } else if (current == ')') {
      Lexer::column++;
      pos++;
      return Token(TokenType::CLOSEPAR, Lexer::line, Lexer::column - 1, ")");
    } else if (current == '[') {
      Lexer::column++;
      pos++;
      return Token(TokenType::OPENSQR, Lexer::line, Lexer::column - 1, "[");
    } else if (current == ']') {
      Lexer::column++;
      pos++;
      return Token(TokenType::CLOSESQR, Lexer::line, Lexer::column - 1, "]");
    } else if (current == '.' && next('.') && next('.', 2)) {
      Lexer::column += 3;
      pos += 3;
      return Token(TokenType::ELLIPSIS, Lexer::line, Lexer::column - 3, "...");
    } else if (current == '.') {
      Lexer::column++;
      pos++;
      return Token(TokenType::DOT, Lexer::line, Lexer::column - 1, ".");
    } else if (current == ',') {
      Lexer::column++;
      pos++;
      return Token(TokenType::COMMA, Lexer::line, Lexer::column - 1, ",");
    } else if (current == ';') {
      Lexer::column++;
      pos++;
      return Token(TokenType::SEMICOLON, Lexer::line, Lexer::column - 1, ";");
    } else if (current == ':' && next(':')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::SCOPE, Lexer::line, Lexer::column - 2, "::");
    } else if (current == ':') {
      Lexer::column++;
      pos++;
      return Token(TokenType::COLON, Lexer::line, Lexer::column - 1, ":");
    } else if (current == '+') {
      Lexer::column++;
      pos++;
      return Token(TokenType::ADD, Lexer::line, Lexer::column - 1, "+");
    } else if (current == '-') {
      Lexer::column++;
      pos++;
      return Token(TokenType::SUB, Lexer::line, Lexer::column - 1, "-");
    } else if (current == '*') {
      Lexer::column++;
      pos++;
      return Token(TokenType::MUL, Lexer::line, Lexer::column - 1, "*");
    } else if (current == '/' && next('/')) {
      return commentToken();
    } else if (current == '/') {
      Lexer::column++;
      pos++;
      return Token(TokenType::DIV, Lexer::line, Lexer::column - 1, "/");
    } else if (current == '%') {
      Lexer::column++;
      pos++;
      return Token(TokenType::MOD, Lexer::line, Lexer::column - 1, "%");
    } else if (current == '_') {
      // TODO: Support `_` leading identifier
      return metadataToken();
    } else if (current & 0x80) {
      // leading one means multibyte character
      return chineseToken();
    } else if (std::isdigit(current)) {
      return integerToken();
    } else if (std::isalpha(current)) {
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::_true,
                                     TokenType::TRUE);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::_false,
                                     TokenType::FALSE);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::actions,
                                     TokenType::ACTIONS);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::block,
                                     TokenType::BLOCK);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::checks,
                                     TokenType::CHECKS);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::triggers,
                                     TokenType::TRIGGERS);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::def, TokenType::DEF);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::_const,
                                     TokenType::CONST);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::_if, TokenType::IF);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::_else,
                                     TokenType::ELSE);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::_for, TokenType::FOR);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::in, TokenType::IN);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::Point,
                                     TokenType::POINT);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::ActorMatch,
                                     TokenType::ACTOR_MATCH);
      LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword::CustomWeapon,
                                     TokenType::CUSTOM_WEAPON);
      return identifierToken();
    } else {
      return Token(TokenType::UNKNOWN, Lexer::line, Lexer::column++,
                   std::string(1, input[pos++]));
    }
  }
  return Token(TokenType::END, Lexer::line, Lexer::column - 1);
}

Token Lexer::chineseToken() {
  int startColumn = column;
  size_t start = pos;
  while (input[pos] & 0x80) {
    Lexer::column++;
    if ((input[pos] & 0xE0) == 0xC0)
      pos += 2; // leading bits are 110
    else if ((input[pos] & 0xF0) == 0xE0)
      pos += 3; // leading bits are 1110
    else if ((input[pos] & 0xF8) == 0xF0)
      pos += 4; // leading bits are 11110
    else
      // TODO: Support multi-byte and single-byte mixed case
      // for example: "讀取Cookies"
      return Token(TokenType::UNKNOWN, Lexer::line, Lexer::column++,
                   std::string(1, input[pos++]));
  }
  return Token(TokenType::IDENTIFIER, Lexer::line, startColumn,
               input.substr(start, pos - start));
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
  while (pos < input.length() &&
         (input[pos] != '"' || input[pos - 1] == '\\')) {
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

Token Lexer::integerToken() {
  size_t start = pos;
  int startColumn = column;
  while (pos < input.length() && isdigit(input[pos])) {
    column++;
    pos++;
  }
  return Token(TokenType::INT, Lexer::line, startColumn,
               input.substr(start, pos - start));
}

Token Lexer::commentToken() {
  size_t start = pos;
  int startColumn = column;
  column += 2;
  while (pos < input.length() && input[pos++] != '\n')
    column++;
  return Token(TokenType::COMMENT, Lexer::line, startColumn,
               input.substr(start, pos - start));
}

inline bool Lexer::next(char character, unsigned offset) {
  return pos + offset < input.length() && input[pos + offset] == character;
}