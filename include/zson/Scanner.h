#pragma once

#include <zson/Token.h>

#include <string>
#include <fstream>
#include <vector>

namespace zson {

class Scanner {
public:
    Scanner(const std::string& source) : source_(source), current_(0) { }
    static Scanner fromFile(const std::string& filename);

    std::vector<Token> scanTokens();
    void scanToken();
private:
    bool isAtEnd();
    char advance();
    char peek();
    char peekNext();

    void addToken(TokenType type);
    void addToken(TokenType type, TokenLiteral literal);

    void json_string();
    void json_boolean();
    void json_null();
    void json_number();

    void error(int line, const std::string& log);

private:
    std::string source_;
    int current_;
    int start;
    int line;
    std::vector<Token> tokens;
};

}