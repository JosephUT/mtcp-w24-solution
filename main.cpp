#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include "mros_json.hpp"

int main() {
    Json json = Json::fromString("{\"Hi\": true}");
    json["bool"] = false;
    std::cout << json.toString() << std::endl;
    return 0;
}
