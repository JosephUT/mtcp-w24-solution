#include <iostream>
#include <iomanip>
#include "mros_json.hpp"

int main() {
    Json json = Json::fromString("{\"Hello\": \"Hi\"}");

    std::cout << std::fixed << std::setprecision(1) << json.toString() << std::endl;
    return 0;
}
