#include <mros_json.hpp>
#include <iostream>
#include <cassert>
#include <messages/pose.hpp>
#include <messages/twist.hpp>


int main() {
    Json json;
    Messages::Pose2d pose = {1.0, 2.0, 3.0};
    json = pose;
    std::cout << json["x"].get<double>() << std::endl;
    std::cout << json["theta"].get<double>() << std::endl;

    Messages::Pose2d pose2D = json;
    assert(pose2D.x == pose.x && pose2D.y == pose.y && pose2D.theta == pose.theta);

    return 0;
}