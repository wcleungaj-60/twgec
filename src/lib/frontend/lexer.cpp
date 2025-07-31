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
      std::cerr << "InvalidCharacterError: Unexpected character \'" << t.value
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
      if (input[pos + 1] == '=') {
        Lexer::column += 2;
        pos += 2;
        return Token(TokenType::EQUAL, Lexer::line, Lexer::column - 2, "==");
      } else {
        Lexer::column++;
        pos++;
        return Token(TokenType::ASSIGN, Lexer::line, Lexer::column - 1, "=");
      }
    case '>':
      if (input[pos + 1] == '=') {
        Lexer::column += 2;
        pos += 2;
        return Token(TokenType::GREATER_THAN_EQUAL, Lexer::line,
                     Lexer::column - 2, ">=");
      } else {
        Lexer::column++;
        pos++;
        return Token(TokenType::GREATER_THAN, Lexer::line, Lexer::column - 1,
                     ">");
      }
    case '<':
      if (input[pos + 1] == '=') {
        Lexer::column += 2;
        pos += 2;
        return Token(TokenType::LESS_THAN_EQUAL, Lexer::line, Lexer::column - 2,
                     "<=");
      } else {
        Lexer::column++;
        pos++;
        return Token(TokenType::LESS_THAN, Lexer::line, Lexer::column - 1, "<");
      }
    case '!':
      if (input[pos + 1] == '=') {
        Lexer::column += 2;
        pos += 2;
        return Token(TokenType::NOT_EQUAL, Lexer::line, Lexer::column - 2,
                     "!=");
      } else {
        return Token(TokenType::UNKNOWN, Lexer::line, Lexer::column++,
                     std::string(1, input[pos++]));
      }
    case '&':
      if (input[pos + 1] == '&') {
        Lexer::column += 2;
        pos += 2;
        return Token(TokenType::AND, Lexer::line, Lexer::column - 2, "&&");
      } else {
        return Token(TokenType::UNKNOWN, Lexer::line, Lexer::column++,
                     std::string(1, input[pos++]));
      }
    case '|':
      if (input[pos + 1] == '|') {
        Lexer::column += 2;
        pos += 2;
        return Token(TokenType::OR, Lexer::line, Lexer::column - 2, "||");
      } else {
        return Token(TokenType::UNKNOWN, Lexer::line, Lexer::column++,
                     std::string(1, input[pos++]));
      }
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
    case '[':
      Lexer::column++;
      pos++;
      return Token(TokenType::OPENSQR, Lexer::line, Lexer::column - 1, "[");
    case ']':
      Lexer::column++;
      pos++;
      return Token(TokenType::CLOSESQR, Lexer::line, Lexer::column - 1, "]");
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
    case ':':
      Lexer::column++;
      pos++;
      return Token(TokenType::COLON, Lexer::line, Lexer::column - 1, ":");
    case '+':
      Lexer::column++;
      pos++;
      return Token(TokenType::ADD, Lexer::line, Lexer::column - 1, "+");
    case '-':
      Lexer::column++;
      pos++;
      return Token(TokenType::SUB, Lexer::line, Lexer::column - 1, "-");
    case '*':
      Lexer::column++;
      pos++;
      return Token(TokenType::MUL, Lexer::line, Lexer::column - 1, "*");
    case '/':
      if (input[pos + 1] == '/') {
        return commentToken();
      } else {
        Lexer::column++;
        pos++;
        return Token(TokenType::DIV, Lexer::line, Lexer::column - 1, "/");
      }
    case '%':
      Lexer::column++;
      pos++;
      return Token(TokenType::MOD, Lexer::line, Lexer::column - 1, "%");
    case '_':
      return metadataToken();
    default:
      if (std::isdigit(current))
        return integerToken();
      else if (!std::isalpha(current))
        return Token(TokenType::UNKNOWN, Lexer::line, Lexer::column++,
                     std::string(1, input[pos++]));
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
  if (pos + 1 >= input.length() || input[pos++] != '/' || input[pos++] != '/') {
    column += 3;
    return Token(TokenType::UNKNOWN, Lexer::line, startColumn,
                 std::string(1, input[pos++]));
  }
  column += 2;
  while (pos < input.length() && input[pos] != '\n') {
    column++;
    pos++;
  }
  return Token(TokenType::COMMENT, Lexer::line, startColumn,
               input.substr(start, pos - start));
}