#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

#define LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword, type) \
    if ((input).substr((pos), (keyword).length()) == (keyword) && \
        !isalpha((input)[(pos) + (keyword).length()]))            \
    {                                                             \
        (pos) += (keyword).length();                              \
        return Token((type));                                     \
    }

#define LEXER_TOKEN_PRINT(type, output) \
    case (type):                        \
        os << output;                   \
        break

enum class TokenType
{
    /* Keyword */
    BLOCK,    // block
    ACTIONS,  // actions
    TRIGGERS, // triggers
    CHECKS,   // checks
    /* Speical Character */
    OPENCUR,   // {
    CLOSECUR,  // }
    ASSIGN,    // =
    SEMICOLON, // ;
    /* Others */
    STRING,
    IDENTIFIER,
    END,
    UNKNOWN
};

struct Token
{
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string &v = "") : type(t), value(v) {}

    friend std::ostream &operator<<(std::ostream &os, const Token &token)
    {
        os << "twge.";
        switch (token.type)
        {
            // Keywords
            LEXER_TOKEN_PRINT(TokenType::BLOCK, "block");
            LEXER_TOKEN_PRINT(TokenType::ACTIONS, "actions");
            LEXER_TOKEN_PRINT(TokenType::TRIGGERS, "triggers");
            LEXER_TOKEN_PRINT(TokenType::CHECKS, "checks");
            // Speical Character
            LEXER_TOKEN_PRINT(TokenType::OPENCUR, "{");
            LEXER_TOKEN_PRINT(TokenType::CLOSECUR, "}");
            LEXER_TOKEN_PRINT(TokenType::ASSIGN, "=");
            LEXER_TOKEN_PRINT(TokenType::SEMICOLON, ";");
            // Otherwise
            LEXER_TOKEN_PRINT(TokenType::STRING, "string@" + token.value);
            LEXER_TOKEN_PRINT(TokenType::IDENTIFIER, "identifier@" + token.value);
            LEXER_TOKEN_PRINT(TokenType::UNKNOWN, "unknown@" + token.value);
            LEXER_TOKEN_PRINT(TokenType::END, "end");
        }
        return os;
    }
};

class Lexer
{
public:
    Lexer(const std::string &input) : input(input), pos(0) {}
    Token nextToken();

private:
    std::string input;
    size_t pos;

    Token stringToken();
    Token identifierToken();
};

#endif