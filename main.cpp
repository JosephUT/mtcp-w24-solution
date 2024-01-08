#include <iostream>
#include <iomanip>
#include <cassert>
#include "mros_json.hpp"

int main() {
    Json json = Json::fromString("{\"Hello\": \"Hi\"}");

    std::cout << std::fixed << std::setprecision(1) << json.toString() << std::endl;

    Json json2;
    json2["Hello"] = "Hi";

    assert(json2 == json);

    json["Num"] = 1.0;
    json2["Num"] = 1;

    assert(json != json2);


    Json j;
    j["Test"] = 2;

    try {
        double d = j["test"].get<double>();
    } catch (std::exception const& ) {
        std::cout << "Strong typed" << std::endl;
    }
    return 0;
}
