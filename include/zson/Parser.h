#pragma once

#include <vector>

#include <zson/Scanner.h>
#include <zson/json.h>

namespace zson {

class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens_(tokens), current_(0) { }

    std::shared_ptr<json> parse();

private:
    bool isAtEnd();
    Token advance();
    Token peek();
    Token previous();
    bool match(TokenType type);
    void error(const std::string& log);


    std::shared_ptr<json_array> array(const Token& token);
    std::shared_ptr<json_object> object(const Token& token);

private:
    std::vector<Token> tokens_;
    int current_;
};

}