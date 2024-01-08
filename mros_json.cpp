#include "mros_json.hpp"
#include <exception>
#include <stdexcept>
#include <cstdint>
#include <variant>

Json::Value &Json::operator[](const std::string &key) {
    auto it = elts_.find(key);
    if (it != elts_.end()) {
        return it->second;
    }
    return elts_.emplace(key, Value{}).first->second;
}

Json::Value const &Json::operator[](const std::string &key) const {
    auto it = elts_.find(key);
    if (it != elts_.end()) {
        return it->second;
    }
    return {};
}

bool Json::operator==(const Json &json) const {
    return this->elts_ == json.elts_;
}

std::string Json::toString() const {
    std::string jsonString = "{";
    for (auto const &[key, value]: elts_) {
        jsonString += "\"" + key + "\": " + stringify(value) + ",";
    }
    if (!elts_.empty()) {
        jsonString.pop_back();
    }
    jsonString += "}";
    return jsonString;
}

std::string Json::stringify(const Json::Value &value) {
    std::string str;
    // Values can be added by simply expanding the indexes with respect
    // to Json::Value::ValueType
    switch (value.data_.index()) {
        case 0: // int
            str = std::to_string(std::get<int>(value.data_));
            break;
        case 1: // double
            str = std::to_string(std::get<double>(value.data_));
            break;
        case 2: // boolean
            str = std::get<bool>(value.data_) ? "true" : "false";
            break;
        case 3: // string
            str = "\"" + std::get<std::string>(value.data_) + "\"";
            break;
        default:
            throw std::runtime_error("Unsupported type in Json::stringify");
    }
    return str;
}

Json Json::fromString(std::string const &str) {
    Json json;
    std::string currentKey;
    std::string currentValue;

    enum class State {
        INIT,  // Initial state
        KEY,   // Parsing key
        VALUE, // Parsing value
    };

    State currentState = State::INIT;

    for (char c: str) {
        switch (currentState) {
            case State::INIT:
                if (c == '{') {
                    currentState = State::KEY;
                }
                break;
            case State::KEY:
                if (c == '"') {
                    currentState = State::KEY;
                } else if (c == ':') {
                    currentState = State::VALUE;
                } else if (c == '}') {
                    if (!currentKey.empty()) {
                        json[currentKey] = currentValue;
                        currentKey.clear();
                        currentValue.clear();
                    }
                    currentState = State::INIT;
                } else if (c == ',') {
                    currentState = State::KEY;
                } else if (std::isspace(c)) {
                    // Skip whitespace
                } else {
                    currentKey += c;
                }
                break;
            case State::VALUE:
                if (c == '"') {
                    currentState = State::VALUE;
                } else if (c == '{') {
                    currentState = State::VALUE;
                }
                    // Where the support for Array Types would go
//                else if (c == '[') {
//                    currentState = State::VALUE;
//                } else if (c == ']') {
//                    currentState = State::VALUE;
//                }
                else if (c == '}') {
                    if (!currentKey.empty()) {
                        try {
                            std::istringstream iss(currentValue);
                            char next;
                            if (iss >> std::ws >> next && iss.eof()) {
                                json[currentKey] = std::stoi(currentValue);
                            } else {
                                json[currentKey] = std::stod(currentValue);
                            }
                        } catch (const std::invalid_argument &) {
                            // Treat as string if conversion fails
                            json[currentKey] = currentValue;
                        }
                        currentKey.clear();
                        currentValue.clear();
                    }
                    currentState = State::INIT;
                } else if (c == ',') {
                    currentState = State::KEY;
                } else if (std::isspace(c)) {
                    // Skip whitespace
                } else {
                    currentValue += c;
                }
                break;

        }
    }
    return json;
}





