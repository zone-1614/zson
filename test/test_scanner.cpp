#include <iostream>
#include <zson/Scanner.h>
#include <zson/Parser.h>

std::string json = R"(
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

int main() {
    zson::Scanner scanner(json);
    auto tokens = scanner.scanTokens();
    for (auto tk : tokens) {
        std::cout << tk << std::endl;
    }
}