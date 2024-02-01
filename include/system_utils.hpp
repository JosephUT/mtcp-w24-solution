#pragma once

#include <cmath>

#include "messages/pose.hpp"
#include "messages/twist.hpp"

/**
 * Mutates pose according to twist. This function is used as a substitute for something 
 * you will learn about in P4.
 * @param twist Command to be applied to pose.
 * @param pose Pose to be updated.
 * @return The updated pose.
 */
Messages::Pose2d mutatePose(Messages::Twist2d const &twist, Messages::Pose2d pose) {
    static double const constexpr dt = 0.1;
    pose.theta += twist.dtheta * dt;
    pose.x += twist.dx*dt*cos(pose.theta) + -twist.dy*dt*sin(pose.theta);
    pose.y += twist.dx*dt*sin(pose.theta) + twist.dy*dt*cos(pose.theta);
    return pose;
}
