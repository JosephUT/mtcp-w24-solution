#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <fstream>
#include <format>
#include <filesystem>
#include <mros_json.hpp>


int main() {
    std::string directory = "messages";
    std::filesystem::path absolute_path = std::filesystem::current_path() / directory;
    for (auto const& entry : std::filesystem::directory_iterator(absolute_path)) {
        std::ifstream ifs(entry.path());
        if (!ifs.is_open()) {
            std::cerr << "Error opening file" << std::endl;
            return 1;
        }
        std::string jsonString(std::istreambuf_iterator<char>{ifs}, {});
        Json json = Json::fromString(jsonString);

        std::cout << json.toString() << std::endl;
    }

    return 0;
}
