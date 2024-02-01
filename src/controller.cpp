#include <chrono>
#include <csignal>
#include <iostream>
#include <fstream>
#include <memory>
#include <thread>

#include "mros_json.hpp"
#include "messages/twist.hpp"
#include "socket/message_socket/client_message_socket.hpp"

using namespace std::chrono_literals;

bool status = true;

static void signalHandler(int signal) {
    status = false;
}

int main() {
    try {
        int const kDomain = AF_INET;
        std::string const kServerAddress = "127.0.0.1";
        int const kServerPort = 13348;

        auto client_sock = std::make_shared<ClientMessageSocket>(kDomain, kServerAddress, kServerPort);
        client_sock->connect();

        double dx, dy, dtheta;
        std::ifstream ifs("data/control_inputs.txt");
        while (ifs >> dx >> dy >> dtheta) {
            Messages::Twist2d twistMsg = {dx, dy, dtheta};
            Json twistJson;
            twistJson = twistMsg;
            client_sock->sendMessage(twistJson.toString());
            std::this_thread::sleep_for(100ms);
        }

        client_sock->close();
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    while (status) {}
    return 0;
}
