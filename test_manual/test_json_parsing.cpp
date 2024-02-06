#include <mros_json.hpp>
#include <iostream>

int main() {
    std::string json_string = R"({"Name": "Jeff Bezos"})";
    Json json = Json::fromString(json_string);
    json["Name2"] = "Jeff Bozos";

    std::cout << json["Name"].get<std::string>() << std::endl;
    std::cout << json["Name2"].get<std::string>() << std::endl;

    json["Num"] = "6.0";
    std::cout << json["Num"].get<std::string>() << std::endl;
    // Json json1;
    // json1["Name"] = "Jeff Bozos";
    // std::cout << json1.toString() << std::endl;

    return 0;
}