#include <socket/server_socket.hpp>
#include <socket/message_socket/connection_message_socket.hpp>
#include <mros_json.hpp>
#include <csignal>
#include <cmath>
#include <iostream>
#include <messageTypes/pose.hpp>
#include <messageTypes/twist.hpp>
#include <systemUtils.hpp>

// Used for timer
#include <chrono>
#include <thread>
bool status = true;

static void signalHandler(int signal) {
    status = false;
}


int main() {
    // Controller info
    int const kDomain = AF_INET;
    std::string const kServerAddress = "127.0.0.1";
    int const kServerPort = 13348;
    int const kBacklogSize = 20;
    Messages::Pose2d current_pose{};


    std::signal(SIGINT, signalHandler);

    auto server_socket = std::make_shared<ServerSocket>(kDomain, kServerAddress, kServerPort, kBacklogSize);
    std::shared_ptr<ConnectionMessageSocket> controllerConnectionMessageSocket;
    std::shared_ptr<ConnectionMessageSocket> monitorConnectionMessageSocket;

    int monitorTimeout = 1;
    int controllerTimeout = 1;
    do {
        std::cout << "Attempting to connect to monitor" << std::endl;
        monitorConnectionMessageSocket = server_socket->acceptConnection<ConnectionMessageSocket>();
        std::this_thread::sleep_for(std::chrono::seconds(monitorTimeout));
        monitorTimeout *= monitorTimeout;

    } while (!monitorConnectionMessageSocket && status);
    do {
        std::cout << "Attempting to connect to controller" << std::endl;
        controllerConnectionMessageSocket = server_socket->acceptConnection<ConnectionMessageSocket>();
        std::this_thread::sleep_for(std::chrono::seconds(controllerTimeout));
        controllerTimeout *= controllerTimeout;
    } while (!controllerConnectionMessageSocket && status);

    try {
        while (status) {
            std::string recvMessage = controllerConnectionMessageSocket->receiveMessage();
            Json recvJson = Json::fromString(recvMessage);
            // Do something with the Json
            Messages::Twist2d twistMsg = recvJson;
            current_pose = mutatePose(twistMsg, current_pose);
            Json sendJson;
            sendJson = current_pose;
            monitorConnectionMessageSocket->sendMessage(sendJson.toString());
        }
    } catch (std::exception const &) {}


    controllerConnectionMessageSocket->close();
    monitorConnectionMessageSocket->close();
    server_socket->close();


    return 0;
}