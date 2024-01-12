#pragma once

#include <vector>
#include <cmath>
#include <mros_json.hpp>

namespace Messages {
    struct Twist2d {
        double dx;
        double dy;
        double dtheta;

        static void fromJson(Messages::Twist2d &twist, Json const &json) {
            twist.dx = json["dx"].get<double>();
            twist.dy = json["dy"].get<double>();
            twist.dtheta = json["dtheta"].get<double>();
        }

        static void toJson(Json &json, Messages::Twist2d const &twist) {
            json["dx"] = twist.dx;
            json["dy"] = twist.dy;
            json["dtheta"] = twist.dtheta;
        }

    };
}