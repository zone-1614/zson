#include <zson/Scanner.h>

#include <stdexcept>
#include <sstream>
#include <iostream>

namespace zson {

Scanner Scanner::fromFile(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("Not exist file: " + filename);
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    return Scanner(ss.str());
}

bool Scanner::isAtEnd() {
    return current_ >= source_.size();
}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current_;
        scanToken();
    }
    addToken(TokenType::END_OF_FILE);
    return tokens;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case '[': addToken(TokenType::LEFT_BRACKET); break;
        case ']': addToken(TokenType::RIGHT_BRACKET); break;
        case ',': addToken(TokenType::COMMA); break;
        case '"': json_string(); break;
        case ':': addToken(TokenType::COLON); break;

        case 't': 
        case 'f': json_boolean(); break;

        case 'n': json_null(); break;

        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': json_number(); break;

        case ' ':
        case '\t':
        case '\r': break;

        case '\n': line++; break;

        default: error(line, "Unexpected character. ");
    }
}

char Scanner::advance() {
    return source_[current_++];
}

char Scanner::peek() {
    if (isAtEnd()) {
        return '\0';
    } else {
        return source_[current_];
    }
}

char Scanner::peekNext() {
    if (current_ + 1 >= source_.size()) {
        return '\0';
    } else {
        return source_[current_ + 1];
    }
}

void Scanner::addToken(TokenType type) {
    addToken(type, {});
}

void Scanner::addToken(TokenType type, TokenLiteral literal) {
    std::string lexeme = source_.substr(start, current_ - start);
    tokens.push_back(Token(type, lexeme, literal, line));
}

void Scanner::json_string() {
    while (peek() != '"' && !isAtEnd()) {
        advance();
    }
    if (isAtEnd()) {
        error(line, "Unterminated string");
    }
    advance();
    std::string val = source_.substr(start + 1, current_ - start - 2);
    addToken(TokenType::STRING, val);
}

void Scanner::json_boolean() {
    if (peek() == 'r') {
        if (source_.compare(current_, 3, "rue") == 0) {
            current_ += 3;
            addToken(TokenType::TRUE);
        } else {
            error(line, "Match 'true' failed. ");
        }
    } else if (peek() == 'a') {
        std::cout << peek() << std::endl;
        if (source_.compare(current_, 4, "alse") == 0) {
            current_ += 4;
            addToken(TokenType::FALSE);
        } else {
            error(line, "Match 'false' failed. ");
        }
    } else {
        error(line, "Match boolean failed. ");
    }
}

void Scanner::json_null() {
    if (source_.compare(current_, 3, "ull") == 0) {
        current_ += 3;
        addToken(TokenType::NIL);
    } else {
        error(line, "Match 'null' failed. ");
    }
}

void Scanner::json_number() {
    while (isdigit(peek())) {
        advance();
    }
    if (peek() == '.' && isdigit(peekNext())) {
        advance();
        while (isdigit(peek())) {
            advance();
        }
    }
    double number = std::atof( source_.substr(start, current_ - start).c_str() );
    addToken(TokenType::NUMBER, number);
}

void Scanner::error(int line, const std::string& log) {
    throw std::logic_error(log + ", in line: " + std::to_string(line));
}

}