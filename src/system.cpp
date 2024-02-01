#include <cmath>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

#include "messages/pose.hpp"
#include "messages/twist.hpp"
#include "mros_json.hpp"
#include "socket/message_socket/client_message_socket.hpp"
#include "socket/message_socket/connection_message_socket.hpp"
#include "socket/server_socket.hpp"
#include "system_utils.hpp"

bool status = true;

static void signalHandler(int signal) {
    status = false;
}

int main() {
    std::signal(SIGINT, signalHandler);

    int const kDomain = AF_INET;
    std::string const kLocalAddress = "127.0.0.1";
    int const kServerPort = 13348;
    int const kBridgePort = 8002;
    int const kBacklogSize = 1;

    auto controller_server_socket = std::make_shared<ServerSocket>(kDomain, kLocalAddress, kServerPort, kBacklogSize);
    std::shared_ptr<ConnectionMessageSocket> controller_connection_socket;
    auto bridge_client_socket = std::make_shared<ClientMessageSocket>(kDomain, kLocalAddress, kBridgePort);

    int bridgeTimeout = 1;
    bool connected = false;
    while (!connected && status) {
        try {
            std::cout << "Attempting to connect to bridge" << std::endl;
            bridge_client_socket->connect();
            connected = true;
        } catch (std::exception const& e) {
            std::cerr << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(bridgeTimeout));
    }
    std::cout << "Connected to bridge" << std::endl;

    int controllerTimeout = 1;
    do {
        std::cout << "Attempting to accept controller connection" << std::endl;
        controller_connection_socket = controller_server_socket->acceptConnection<ConnectionMessageSocket>();
        std::this_thread::sleep_for(std::chrono::seconds(controllerTimeout));
    } while (!controller_connection_socket && status);
    std::cout << "Connected to controller" << std::endl;

    Messages::Pose2d current_pose{};
    try {
        while (status) {
            std::string recvMessage = controller_connection_socket->receiveMessage();
            Json recvJson = Json::fromString(recvMessage);
            Messages::Twist2d twistMsg = recvJson;
            current_pose = mutatePose(twistMsg, current_pose);
            Json sendJson;
            sendJson = current_pose;
            bridge_client_socket->sendMessage(sendJson.toString());
        }
    } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
    }

    controller_connection_socket->close();
    bridge_client_socket->close();
    controller_server_socket->close();

    return 0;
}
