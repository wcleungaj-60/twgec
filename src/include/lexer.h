#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <cctype>
#include "token.h"

#define LEXER_MATCH_KEYWORD_AND_RETURN(input, pos, keyword, type) \
    if ((input).substr((pos), (keyword).length()) == (keyword) && \
        !isalpha((input)[(pos) + (keyword).length()]))            \
    {                                                             \
        (pos) += (keyword).length();                              \
        return Token((type));                                     \
    }

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