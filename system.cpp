#include <socket/server_socket.hpp>
#include <socket/message_socket/connection_message_socket.hpp>
#include <mros_json.hpp>
#include <csignal>
#include <cmath>
#include <iostream>
#include <messageTypes/pose.hpp>
#include <messageTypes/twist.hpp>
#include <systemUtils.hpp>

bool status = true;

static void signalHandler(int signal) {
    status = false;
}


int main() {
    int const kDomain = AF_INET;
    std::string const kServerAddress = "127.0.0.1";
    int const kServerPort = 13349;
    int const kBacklogSize = 20;
    Messages::Pose2d current_pose{};

    std::signal(SIGINT, signalHandler);

    auto server_socket = std::make_shared<ServerSocket>(kDomain, kServerAddress, kServerPort, kBacklogSize);
    std::shared_ptr<ConnectionMessageSocket> controllerConnectionMessageSocket;
    std::shared_ptr<ConnectionMessageSocket> monitorConnectionMessageSocket;


    do {
        monitorConnectionMessageSocket = server_socket->acceptConnection<ConnectionMessageSocket>();
    } while (!monitorConnectionMessageSocket);
    do {
        controllerConnectionMessageSocket = server_socket->acceptConnection<ConnectionMessageSocket>();
    } while (!controllerConnectionMessageSocket);

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