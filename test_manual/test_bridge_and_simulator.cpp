#include <csignal>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "messages/pose.hpp"
#include "mros_json.hpp"
#include "socket/message_socket/client_message_socket.hpp"
#include "system_utils.hpp"

using namespace std::chrono_literals;

bool USER_INPUT = false;
bool status = true;

static void signalHandler(int signal) {
    status = false;
}

int main() {
    // Set up bridge address.
    int const kDomain = AF_INET;
    std::string const kBridgeServerAddress = "127.0.0.1";
    int const kBridgeServerPort = 8002;

    // Connect client socket to bridge.
    auto bridge_client_socket = std::make_shared<ClientMessageSocket>(kDomain, kBridgeServerAddress, kBridgeServerPort);
    bridge_client_socket->connect();

    // Loop to send poses.
    double x = 0;
    double y = 0;
    double theta = 0;

    while (status) {    
        if (USER_INPUT) {
            // Get pose data from the user.
            std::cout << "connected. input (x, y, theta) pose information" << std::endl;
            std::cin >> x >> y >> theta;
            std::cout << "got x: " << x << ", y: " << y << ", theta: " << theta << std::endl;

        } else {
            std::this_thread::sleep_for(100ms);  // 50 fps
            theta += 0.05;
        }

        // Convert the pose to JSON format and send as a string message.
        Messages::Pose2d pose {x, y, theta};
        Json pose_json;
        pose_json = pose;
        std::cout << "sending: " << pose_json.toString() << std::endl;
        bridge_client_socket->sendMessage(pose_json.toString());
    }

    return 0;
}
