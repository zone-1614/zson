#pragma once

#include <string>
#include <any>
#include <ostream>
#include <variant>

namespace zson {

enum class TokenType {
    LEFT_BRACE, RIGHT_BRACE, // {  }
    LEFT_BRACKET, RIGHT_BRACKET, // [  ]
    COMMA, QUOTE, COLON, // , " :
    NUMBER, STRING, TRUE, FALSE, NIL, // NULL is a key word, replace by NIL
    END_OF_FILE 
};

using TokenLiteral = std::variant<std::monostate, double, std::string>;
struct Token {
    Token(TokenType type, std::string lexeme, TokenLiteral literal, int line) : type(type), lexeme(lexeme), literal(literal), line(line) { }
    TokenType type;
    std::string lexeme;
    TokenLiteral literal;
    int line;

    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        std::string token_string;
        switch (token.type) {
            case TokenType::LEFT_BRACE: token_string = "left brace"; break;
            case TokenType::RIGHT_BRACE: token_string = "right brace"; break;
            case TokenType::LEFT_BRACKET: token_string = "left bracket"; break;
            case TokenType::RIGHT_BRACKET: token_string = "right bracket"; break;
            case TokenType::COMMA: token_string = "comma"; break;
            case TokenType::QUOTE: token_string = "quote"; break;
            case TokenType::COLON: token_string = "colon"; break;
            case TokenType::NUMBER: token_string = "number"; break;
            case TokenType::STRING: token_string = "string"; break;
            case TokenType::TRUE: token_string = "true"; break;
            case TokenType::FALSE: token_string = "false"; break;
            case TokenType::NIL: token_string = "null"; break;
            case TokenType::END_OF_FILE: token_string = "END_OF_FILE"; break;
        }
        std::string token_literal = "";
        if (token.literal.index() == 1) {
            token_literal = std::to_string(std::get<1>(token.literal));
        } else if (token.literal.index() == 2) {
            token_literal = std::get<2>(token.literal);
        } 
        os << "Token { type: " << token_string << ", lexeme: " << token.lexeme << ", literal: " << token_literal << " }";
        return os;
    }
};

}