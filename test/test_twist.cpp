#include <gtest/gtest.h>

#include "mros_json.hpp"
#include "messages/twist.hpp"

TEST(Twist2d, TestToJson) {
    Messages::Twist2d twist = {1.0, 2.0, 3.0};
    Json twist_as_json;
    twist_as_json = twist;
    ASSERT_EQ(twist_as_json.toString(), "{\"dtheta\": 3.000000,\"dx\": 1.000000,\"dy\": 2.000000}");
}

TEST(Twist2d, TestFromJson) {
    Json twist_as_json;
    twist_as_json["dx"] = 1.0;
    twist_as_json["dy"] = 2.0;
    twist_as_json["dtheta"] = 3.0;
    Messages::Twist2d twist = twist_as_json;
    Messages::Twist2d correct_twist = {1.0, 2.0, 3.0};
    ASSERT_DOUBLE_EQ(twist.dx, 1.0);
    ASSERT_DOUBLE_EQ(twist.dy, 2.0);
    ASSERT_DOUBLE_EQ(twist.dtheta, 3.0);
}