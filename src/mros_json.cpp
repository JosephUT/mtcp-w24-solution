#include <mros_json.hpp>
#include <exception>
#include <stdexcept>
#include <cstdint>
#include <variant>
#include <algorithm>

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
    static Json::Value emptyValue;
    return emptyValue;
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
    } // do nothing
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
        case 2: // string
            str = "\"" + std::get<std::string>(value.data_) + "\"";
            break;
        case 3: { // std::vector<int>
            auto const &vec = std::get<std::vector<int>>(value.data_);
            str += '[';
            for (int const val: vec) {
                str += std::to_string(val) + ',' + ' ';
            }
            if (!vec.empty()) {
                str.pop_back();
                str.pop_back();
            }
            str += ']';
            break;
        }
        case 4: { // std::vector<double>
            auto const &vec = std::get<std::vector<double>>(value.data_);
            str += '[';
            for (double const val: vec) {
                str += std::to_string(val) + ',' + ' ';
            }
            if (!vec.empty()) {
                str.pop_back();
                str.pop_back();
            }
            str += ']';
            break;
        }
        case 5: { // std::vector<std::string>
            auto const &vec = std::get<std::vector<std::string>>(value.data_);
            str += '[';
            for (std::string const &val: vec) {
                str += "\"" + val + "\"" + ',' + ' ';
            }
            if (!vec.empty()) {
                str.pop_back();
                str.pop_back();
            }
            str += ']';
            break;
        }
        default:
            throw std::runtime_error("Unsupported type in Json::stringify");
    }
    return str;
}

Json Json::fromString(const std::string &str) {
    Json json;
    std::string currentKey;
    std::string currentValue;

    std::vector<int> currentIntArray;
    std::vector<double> currentDoubleArray;
    std::vector<std::string> currentStringArray;

    enum class State {
        INIT,           // Initial state
        KEY,            // Parsing key
        VALUE,          // Parsing value
        ARRAY_ELEMENT   // Parsing array element
    };

    State currentState = State::INIT;
    auto it = str.begin();
    while (it != str.end()) {
        char c = *it;
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
                // if (c == '"') {
                //     currentState = State::VALUE;
                // } else
                if (c == '{') {
                    currentState = State::VALUE;
                } else if (c == '[') {
                    currentState = State::ARRAY_ELEMENT;
                } else if (c == '}') {
                    if (!currentKey.empty()) {
                        Json::Value value = getType(currentValue);
                        json[currentKey] = value;

                        currentKey.clear();
                        currentValue.clear();
                    }
                    currentState = State::INIT;
                } else if (c == ',') {
                    if (!currentKey.empty()) {
                        Json::Value value = getType(currentValue);
                        json[currentKey] = value;

                        currentKey.clear();
                        currentValue.clear();
                    }
                    currentState = State::KEY;
                } else if (std::isspace(c)) {
                    // Skip whitespace
                } else {
                    currentValue += c;
                }
                break;
            case State::ARRAY_ELEMENT:
                if (c == '[') {
                    // Nested arrays are not supported in this example
                    throw std::runtime_error("Nested arrays are not supported.");
                } else if (c == '{') {
                    throw std::logic_error("Nested Json not supported");
                } else if (c == '}') {
                    throw std::logic_error("Grabbed closing json bracket when should be closing array bracket");
                } else if (c == ']') {
                    // End of array, store the current array
                    if (!currentKey.empty()) {
                        if (!currentIntArray.empty()) {
                            json[currentKey] = currentIntArray;
                        } else if (!currentDoubleArray.empty()) {
                            json[currentKey] = currentDoubleArray;
                        } else if (!currentStringArray.empty()) {
                            json[currentKey] = currentStringArray;
                        }
                        currentKey.clear();
                        currentIntArray.clear();
                        currentDoubleArray.clear();
                        currentDoubleArray.clear();
                    }
                    currentState = State::INIT;
                } else if (c == ',') {
                    // Separator between array elements
                    currentState = State::ARRAY_ELEMENT;
                } else if (std::isspace(c)) {
                    // Skip whitespace
                } else {
                    // Parse array element based on the type
                    std::string arrayElementValue;
                    while (it != str.end() && c != ',' && c != ']' && !std::isspace(c)) {
                        arrayElementValue += c;
                        c = *(++it);
                    }
                    // Need type converter here
                    Json::Value val = getType(arrayElementValue);
                    if (val.data_.index() == 0) { //int
                        currentIntArray.push_back(val.get<int>());
                    } else if (val.data_.index() == 1) { // double
                        currentDoubleArray.push_back(val.get<double>());
                    } else if (val.data_.index() == 2) {
                        currentStringArray.push_back(val.get<std::string>());
                    } else {
                        throw std::runtime_error("invalid type");
                    }

                    currentState = State::ARRAY_ELEMENT;
                    --it; // the while conditions skip ']', so we step back once
                }
                break;
        }
        ++it;
    }
    return json;
}

Json::Value Json::getType(const std::string &str) {
    if (str.empty()) {
        throw std::logic_error("Error: string is invalid/empty");
    }
    if (str[0] == '"' || str[0] == '\"') {
        return str;
    }
    size_t pos = 0;
    try {
        int intResult = std::stoi(str, &pos);
        if (pos == str.size()) {
            return intResult;
        }
        return std::stod(str);
    } catch( std::invalid_argument const&) {
        return str;
    }

}
