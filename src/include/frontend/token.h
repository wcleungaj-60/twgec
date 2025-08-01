#ifndef TOKEN_H
#define TOKEN_H

#include "utils/location.h"
#include <iostream>

#define LEXER_TOKEN_TYPE_PRINT(type, output)                                   \
  case (type):                                                                 \
    os << output;                                                              \
    break

#define LEXER_TOKEN_PRINT(type, output)                                        \
  case (type):                                                                 \
    os << token.location << " " << output;                                     \
    break

enum class TokenType {
  /* Keyword */
  TRUE,     // true
  FALSE,    // false
  BLOCK,    // block
  ACTIONS,  // actions
  TRIGGERS, // triggers
  CHECKS,   // checks
  DEF,      // def
  CONST,    // const
  IF,       // if
  POINT,    // Point
  /* Speical Character */
  OPENCUR,            // {
  CLOSECUR,           // }
  OPENPAR,            // (
  CLOSEPAR,           // )
  OPENSQR,            // [
  CLOSESQR,           // ]
  ASSIGN,             // =
  SEMICOLON,          // ;
  COLON,              // :
  DOT,                // .
  COMMA,              // ,
  AND,                // &&
  OR,                 // ||
  EQUAL,              // ==
  NOT_EQUAL,          // !=
  LESS_THAN,          // <
  LESS_THAN_EQUAL,    // <=
  GREATER_THAN,       // >
  GREATER_THAN_EQUAL, // >=
  ADD,                // +
  SUB,                // -
  MUL,                // *
  DIV,                // /
  MOD,                // %
  /* Others */
  INT,
  STRING,
  COMMENT,
  IDENTIFIER,
  METADATA,
  END,
  UNKNOWN
};

inline std::ostream &operator<<(std::ostream &os, const TokenType &type) {
  switch (type) {
    LEXER_TOKEN_TYPE_PRINT(TokenType::TRUE, "\'true\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::FALSE, "\'false\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::BLOCK, "\'block\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::ACTIONS, "\'actions\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::TRIGGERS, "\'triggers\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::CHECKS, "\'checks\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::DEF, "\'def\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::CONST, "\'const\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::IF, "\'if\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::POINT, "\'Point\'");
    // Speical Character
    LEXER_TOKEN_TYPE_PRINT(TokenType::OPENCUR, "\'{\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::CLOSECUR, "\'}\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::OPENPAR, "\'(\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::CLOSEPAR, "\')\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::OPENSQR, "\'[\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::CLOSESQR, "\']\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::ASSIGN, "\'=\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::SEMICOLON, "\';\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::COLON, "\':\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::DOT, "\'.\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::COMMA, "\',\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::AND, "\'&&\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::OR, "\'||\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::EQUAL, "\'==\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::NOT_EQUAL, "\'!=\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::LESS_THAN, "\'<\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::LESS_THAN_EQUAL, "\'<=\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::GREATER_THAN, "\'>\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::GREATER_THAN_EQUAL, "\'>=\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::ADD, "\'+\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::SUB, "\'-\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::MUL, "\'*\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::DIV, "\'/\'");
    LEXER_TOKEN_TYPE_PRINT(TokenType::MOD, "\'%\'");
    // Otherwise
    LEXER_TOKEN_TYPE_PRINT(TokenType::INT, "int");
    LEXER_TOKEN_TYPE_PRINT(TokenType::STRING, "string");
    LEXER_TOKEN_TYPE_PRINT(TokenType::COMMENT, "comment");
    LEXER_TOKEN_TYPE_PRINT(TokenType::IDENTIFIER, "identifier");
    LEXER_TOKEN_TYPE_PRINT(TokenType::METADATA, "metadata");
    LEXER_TOKEN_TYPE_PRINT(TokenType::UNKNOWN, "unknown");
    LEXER_TOKEN_TYPE_PRINT(TokenType::END, "end");
  }
  return os;
}

struct Token {
  TokenType type;
  std::string value;
  Location location;

  Token(TokenType t, int line, int column, const std::string &v = "")
      : type(t), value(v), location(line, column) {}

  friend std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << "twge.";
    switch (token.type) {
      // Keywords
      LEXER_TOKEN_PRINT(TokenType::TRUE, "bool@true");
      LEXER_TOKEN_PRINT(TokenType::FALSE, "bool@false");
      LEXER_TOKEN_PRINT(TokenType::BLOCK, "block");
      LEXER_TOKEN_PRINT(TokenType::ACTIONS, "actions");
      LEXER_TOKEN_PRINT(TokenType::TRIGGERS, "triggers");
      LEXER_TOKEN_PRINT(TokenType::CHECKS, "checks");
      LEXER_TOKEN_PRINT(TokenType::DEF, "def");
      LEXER_TOKEN_PRINT(TokenType::CONST, "const");
      LEXER_TOKEN_PRINT(TokenType::IF, "if");
      LEXER_TOKEN_TYPE_PRINT(TokenType::POINT, "Point");
      // Speical Character
      LEXER_TOKEN_PRINT(TokenType::OPENCUR, "{");
      LEXER_TOKEN_PRINT(TokenType::CLOSECUR, "}");
      LEXER_TOKEN_PRINT(TokenType::OPENPAR, "(");
      LEXER_TOKEN_PRINT(TokenType::CLOSEPAR, ")");
      LEXER_TOKEN_PRINT(TokenType::OPENSQR, "[");
      LEXER_TOKEN_PRINT(TokenType::CLOSESQR, "]");
      LEXER_TOKEN_PRINT(TokenType::ASSIGN, "=");
      LEXER_TOKEN_PRINT(TokenType::SEMICOLON, ";");
      LEXER_TOKEN_PRINT(TokenType::COLON, ":");
      LEXER_TOKEN_PRINT(TokenType::DOT, ".");
      LEXER_TOKEN_PRINT(TokenType::COMMA, ",");
      LEXER_TOKEN_TYPE_PRINT(TokenType::AND, "&&");
      LEXER_TOKEN_TYPE_PRINT(TokenType::OR, "||");
      LEXER_TOKEN_TYPE_PRINT(TokenType::EQUAL, "==");
      LEXER_TOKEN_TYPE_PRINT(TokenType::NOT_EQUAL, "!=");
      LEXER_TOKEN_TYPE_PRINT(TokenType::LESS_THAN, "<");
      LEXER_TOKEN_TYPE_PRINT(TokenType::LESS_THAN_EQUAL, "<=");
      LEXER_TOKEN_TYPE_PRINT(TokenType::GREATER_THAN, ">");
      LEXER_TOKEN_TYPE_PRINT(TokenType::GREATER_THAN_EQUAL, ">=");
      LEXER_TOKEN_TYPE_PRINT(TokenType::ADD, "+");
      LEXER_TOKEN_TYPE_PRINT(TokenType::SUB, "-");
      LEXER_TOKEN_TYPE_PRINT(TokenType::MUL, "*");
      LEXER_TOKEN_TYPE_PRINT(TokenType::DIV, "/");
      LEXER_TOKEN_TYPE_PRINT(TokenType::MOD, "%");
      // Otherwise
      LEXER_TOKEN_PRINT(TokenType::INT, "int@" + token.value);
      LEXER_TOKEN_PRINT(TokenType::STRING, "string@" + token.value);
      LEXER_TOKEN_PRINT(TokenType::COMMENT, "comment@" + token.value);
      LEXER_TOKEN_PRINT(TokenType::IDENTIFIER, "identifier@" + token.value);
      LEXER_TOKEN_PRINT(TokenType::METADATA, "metadata@" + token.value);
      LEXER_TOKEN_PRINT(TokenType::UNKNOWN, "unknown@" + token.value);
      LEXER_TOKEN_PRINT(TokenType::END, "end");
    }
    return os;
  }
};

#endif