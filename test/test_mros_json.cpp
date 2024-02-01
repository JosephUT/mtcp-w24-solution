#include <gtest/gtest.h>

#include "mros_json.hpp"

TEST(MrosJson, TestBracketOperator) {
    
}

TEST(MrosJson, TestBracketOperatorConst) {

}

TEST(MrosJson, TestOperatorEquals) {

}

TEST(MrosJson, TestToString) {

}

TEST(MrosJson, TestFromString) {
    
}
    /**
     * Modifiable Value reference to a Json::Value.
     * @param key String mapping to the returned Value.
     * @return The Value mapped to by the key.
     */
    Value &operator[](std::string const &key);

    /**
     * Copy or const reference to Json::Value.
     * @param key String mapping to the returned Value.
     * @return The Value mapped to by the key.
     */
    Value const &operator[](std::string const &key) const;

    /**
     * Equality operator for Json.
     * @param json Json to compare with this.
     * @return True if all elements match, false otherwise.
     */
    bool operator==(Json const &json) const;

    /**
     * Convert Json object to string in JSON format.
     * @return The string representation of the Json.
     */
    std::string toString() const;

    /**
     * Set a Json object from a string in JSON format.
     * @param str Json formatted string.
     * @return Json object.
     */
    static Json fromString(std::string const &str);