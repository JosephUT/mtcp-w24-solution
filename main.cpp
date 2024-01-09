#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <fstream>
#include <algorithm>
#include "mros_json.hpp"

int main() {
    std::ifstream ifs("pose.json");
    if (!ifs.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    std::string jsonString(std::istreambuf_iterator<char>{ifs}, {});
    Json json = Json::fromString(jsonString);

    std::cout << json.toString() << std::endl;
    return 0;
}
