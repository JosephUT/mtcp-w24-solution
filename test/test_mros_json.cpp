#include <gtest/gtest.h>

#include "mros_json.hpp"

TEST(MrosJson, TestBracketOperator) {
    const std::vector<int> kTestVec{1, 2, 3};
    Json test_json;
    test_json["test_int"] = 3;
    test_json["test_double"] = 4.0;
    test_json["test_string"] = "five";
    test_json["test_array"] = kTestVec;
    ASSERT_EQ(test_json["test_int"].get<int>(), 3);
    ASSERT_EQ(test_json["test_double"].get<double>(), 4.0);
    ASSERT_EQ(test_json["test_string"].get<std::string>(), "five");
    ASSERT_EQ(test_json["test_array"].get<std::vector<int>>(), kTestVec);
}

TEST(MrosJson, TestEqualityOperator) {
    Json json1, json2;
    json1["a"] = 1.0;
    json2["a"] = 1.0;
    ASSERT_EQ(json1, json2);
    json2["b"] = 1.0;
    ASSERT_NE(json1, json2);
}

TEST(MrosJson, TestToStringInt) {
    Json test_json;
    test_json["b"] = 9;
    ASSERT_EQ(test_json.toString(), "{\"b\": 9}");
    test_json["c"] = 8;
    ASSERT_EQ(test_json.toString(), "{\"b\": 9,\"c\": 8}");
    test_json["a"] = 7;
    ASSERT_EQ(test_json.toString(), "{\"a\": 7,\"b\": 9,\"c\": 8}") 
    << "Alphabetical ordering missing.";
}

TEST(MrosJson, TestToStringDouble) {
    Json test_json;
    test_json["b"] = 9.0;
    ASSERT_EQ(test_json.toString(), "{\"b\": 9.000000}");
    test_json["c"] = 8.0;
    ASSERT_EQ(test_json.toString(), "{\"b\": 9.000000,\"c\": 8.000000}");
    test_json["a"] = 7.0;
    ASSERT_EQ(test_json.toString(), "{\"a\": 7.000000,\"b\": 9.000000,\"c\": 8.000000}")  
    << "Alphabetical ordering missing.";
}

TEST(MrosJson, TestToStringString) {
    const std::string kTestString = "testing is fun";
    Json test_json;
    test_json["my_string"] = kTestString;
    ASSERT_EQ(test_json.toString(), "{\"my_string\": \"testing is fun\"}");
}

TEST(MrosJson, TestToStringArray) {
    const std::vector<int> kTestIntArray{1, 2, 3};
    const std::vector<double> kTestDoubleArray{4.0, 5.0};
    const std::vector<std::string> kTestStringArray{"six"};
    Json test_json;
    test_json["int"] = kTestIntArray;
    test_json["double"] = kTestDoubleArray;
    test_json["string"] = kTestStringArray;
    ASSERT_EQ(test_json.toString(), "{\"double\": [4.000000, 5.000000],\"int\": [1, 2, 3],\"string\": [\"six\"]}");
}

TEST(MrosJson, TestFromStringInt) {
    Json test_json = Json::fromString("{\"int_key\": 55}");
    ASSERT_EQ(test_json["int_key"].get<int>(), 55);
}

TEST(MrosJson, TestFromStringDouble) {
    Json test_json = Json::fromString("{\"double_key\": 5.000000}");
    ASSERT_DOUBLE_EQ(test_json["double_key"].get<double>(), 5.0);
}

TEST(MrosJson, TestFromStringString) {
    Json test_json = Json::fromString("{\"string_key\": \"string_value\"}");
    ASSERT_EQ(test_json["string_key"].get<std::string>(), "string_value");
}

TEST(MrosJson, TestFromStringArray) {
    const std::vector<int> kTestIntArray{1, 2, 3};
    Json test_json_int = Json::fromString("{\"int\": [1, 2, 3]}");
}
