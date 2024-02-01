#pragma once

#include "mros_json.hpp"

namespace Messages {
    struct Pose2d {
        double x;
        double y;
        double theta;

        static void fromJson(Messages::Pose2d& pose2D, Json const& json) {
            pose2D.x = json["x"].get<double>();
            pose2D.y = json["y"].get<double>();
            pose2D.theta = json["theta"].get<double>();
        }

        static void toJson(Json &json, Messages::Pose2d const& pose2D) {
            json["x"] = pose2D.x;
            json["y"] = pose2D.y;
            json["theta"] = pose2D.theta;
        }
    };
}
