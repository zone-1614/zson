#include <zson/Parser.h>
#include <iostream>

namespace zson {

std::shared_ptr<json> Parser::parse() {
    auto j = std::make_shared<json>();
    auto token = advance();
    switch (token.type) {
        case TokenType::NUMBER: {
            j->set_var(std::get<1>(token.literal));
            break;
        }
        case TokenType::STRING: {
            j->set_var(std::get<2>(token.literal));
            break;
        }
        case TokenType::TRUE: {
            j->set_var(true);
            break;
        }
        case TokenType::FALSE: {
            j->set_var(false);
            break;
        }
        case TokenType::NIL: {
            break;
        }
        case TokenType::LEFT_BRACKET: {
            auto arr = array(token);
            j->set_var(arr);
            break;
        }
        case TokenType::LEFT_BRACE: {
            auto obj = object(token);
            j->set_var(obj);
            break;
        }
        case TokenType::END_OF_FILE: break;
        default:
            error("Unexpected token type");
    }
    return j;
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::advance() {
    return tokens_[current_++];
}

Token Parser::peek() {
    return tokens_[current_];
}

Token Parser::previous() {
    return tokens_[current_ - 1];
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

void Parser::error(const std::string& log) {
    throw std::runtime_error(log);
}

std::shared_ptr<json_array> Parser::array(const Token& token) {
    if (isAtEnd()) error("json array lose closing bracket. ");

    auto arr = std::make_shared<json_array>();
    
    if (match(TokenType::RIGHT_BRACKET)) {
        return arr;
    }

    while (true) {
        arr->push_back(parse());
        if (match(TokenType::COMMA)) continue;
        if (match(TokenType::RIGHT_BRACKET)) break;
        else error("Expect a COMMA or RIGHT_BRACKET");
    }
    return arr;
}

std::shared_ptr<json_object> Parser::object(const Token& token) {
    if (isAtEnd()) error("json object lose closing brace. ");

    auto obj = std::make_shared<json_object>();

    if (match(TokenType::RIGHT_BRACE)) {
        return obj;
    }

    while (true) {
        if (!match(TokenType::STRING)) {
            error("Expect a STRING");
        }

        auto key = std::get<2>(previous().literal);

        if (!match(TokenType::COLON)) {
            error("Expect a COLON");
        }
        obj->emplace(key, parse());

        if (match(TokenType::COMMA)) continue;;
        if (match(TokenType::RIGHT_BRACE)) break;
        else error("Expect a COMMA or RIGHT_BRACE");
    }
    return obj;
}

}