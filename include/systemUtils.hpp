#pragma once

#include <vector>
#include <cmath>
#include <messageTypes/pose.hpp>
#include <messageTypes/twist.hpp>

/**
 * Mutates Pose. This function is used as a substitute for something you will learn later
 * @param twist Twist2d Message
 * @param old_pose Pose Message
 * @return updated pose
 */
Messages::Pose2d mutatePose(Messages::Twist2d const &twist, Messages::Pose2d pose) {
    static double const constexpr dt = 0.1;
    pose.x += twist.dx * dt;
    pose.y += twist.dy * dt;
    pose.theta += twist.dtheta * dt;
    return pose;
}