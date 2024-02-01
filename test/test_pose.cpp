#include <gtest/gtest.h>

#include "mros_json.hpp"
#include "messages/pose.hpp"

TEST(Pose2d, TestToJson) {
    Messages::Pose2d pose = {1.0, 2.0, 3.0};
    Json pose_as_json;
    pose_as_json = pose;
    ASSERT_EQ(pose_as_json.toString(), "{\"theta\": 3.000000,\"x\": 1.000000,\"y\": 2.000000}");
}

TEST(Pose2d, TestFromJson) {
    Json pose_as_json;
    pose_as_json["x"] = 1.0;
    pose_as_json["y"] = 2.0;
    pose_as_json["theta"] = 3.0;
    Messages::Pose2d pose = pose_as_json;
    Messages::Pose2d correct_pose = {1.0, 2.0, 3.0};
    ASSERT_DOUBLE_EQ(pose.x, 1.0);
    ASSERT_DOUBLE_EQ(pose.y, 2.0);
    ASSERT_DOUBLE_EQ(pose.theta, 3.0);
}