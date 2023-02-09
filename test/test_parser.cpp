#include <iostream>
#include <zson/Parser.h>

std::string str = R"(
{
    "firstName": "John",
    "lastName": "Smith",
    "isAlive": true,
    "age": 27,
    "address": {
        "streetAddress": "21 2nd Street",
        "city": "New York",
        "state": "NY",
        "postalCode": "10021-3100"
    },
    "phoneNumbers": [
        {
            "type": "home",
            "number": "212 555-1234"
        },
        {
            "type": "office",
            "number": "646 555-4567"
        }
    ],
    "children": [
        "Catherine",
        "Thomas",
        "Trevor"
    ],
    "spouse": null
}
)";

int main(int argc, char* argv[]) {
    zson::Scanner scanner(str);
    if (argc == 2) {
        std::string filepath = argv[1];
        scanner = zson::Scanner::fromFile(filepath);
    }
    auto tokens = scanner.scanTokens();
    zson::Parser parser(tokens);
    auto j = parser.parse();
    std::cout << j->toString() << std::endl;
}