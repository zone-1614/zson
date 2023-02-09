#include <zson/Parser.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "please input the filepath and the save path";
        return 0;
    }
    std::string input = argv[1];
    std::string output = argv[2];
    auto scanner = zson::Scanner::fromFile(input);
    auto parser = zson::Parser(scanner.scanTokens());
    auto json = *parser.parse();

    auto users = *json["users"];
    auto user1 = *users[1];
    user1["name"]->set_var(std::string("Gu Ai Lin"));
    user1["address"]->set_var(std::string("理塘"));

    auto coordinates = *json["coordinates"];
    coordinates["x"]->set_var(100.0);

    auto image = *json["images"];
    image[0]->set_var(std::string("after edit.png"));

    json.save(output);
}