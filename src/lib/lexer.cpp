#include <iostream>
#include <string>
#include <vector>
#include <cctype>

enum class TokenType {
    // Keywords
    BLOCK,      // block
    ACTIONS,     // actions
    // Speical Character
    OPENCUR,    // {
    CLOSECUR,   // }
    ASSIGN,     // =
    // Others
    STRING,
    IDENTIFIER,
    END,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v = "") : type(t), value(v) {}

    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "twge.";
        switch (token.type) {
            // Keywords
            case TokenType::BLOCK: os << "block"; break;
            case TokenType::ACTIONS: os << "actions"; break;
            // Speical Character
            case TokenType::OPENCUR: os << "{"; break;
            case TokenType::CLOSECUR: os << "}"; break;
            case TokenType::ASSIGN: os << "="; break;
            // Otherwise
            case TokenType::STRING: os << "string@" << token.value; break;
            case TokenType::IDENTIFIER: os << "identifier@" << token.value; break;
            case TokenType::END: os << "__end"; break;
            case TokenType::UNKNOWN: os << "__unknown@" << token.value; break;
        }
        return os;
    }
};

class Lexer {
public:
    Lexer(const std::string& input) : input(input), pos(0) {}

    Token nextToken() {
        while (pos < input.length()) {
            char current = input[pos];
            switch(current){
                case ' ': case '\t': case '\n':
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
                case 'a':
                    if (input.substr(pos, 7) == "actions" && !isalpha(input[pos + 7])) {
                        pos += 7;
                        return Token(TokenType::ACTIONS);
                    }
                case 'b':
                    if (input.substr(pos, 5) == "block" && !isalpha(input[pos + 5])) {
                        pos += 5;
                        return Token(TokenType::BLOCK);
                    }
                default:
                    if (std::isalpha(current))
                        return identifierToken();
                    else
                        return Token(TokenType::UNKNOWN, std::string(1, input[pos++]));
            }
        }
        return Token(TokenType::END);
    }

private:
    std::string input;
    size_t pos;

    Token stringToken() {
        size_t start = pos++;
        while (pos < input.length() && input[pos] != '"')
            pos++;
        if (pos < input.length())
            pos++; // Skip closing quote
        return Token(TokenType::STRING, input.substr(start, pos - start));
    }

    Token identifierToken() {
        size_t start = pos;
        while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_'))
            pos++;
        return Token(TokenType::IDENTIFIER, input.substr(start, pos - start));
    }
};