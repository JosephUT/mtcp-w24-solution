#include <socket/client_socket.hpp>
#include <iostream>
#include <mros_json.hpp>
#include <memory>
#include <socket/message_socket/client_message_socket.hpp>
#include <csignal>

// Timing
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

bool status = true;

int main() {
    std::signal(SIGINT, [](int signum)->void {
       status = false;
    });
    try {
        int const kDomain = AF_INET;
        std::string const kServerAddress = "127.0.0.1";
        int const kServerPort = 8080;

        auto client_sock = std::make_unique<ClientMessageSocket>(kDomain, kServerAddress, kServerPort);
        client_sock->connect();

        int count = 0;
        while (status) {
            Json msg;
            msg["message"] = "Hello World " + std::to_string(count++);
            std::cout << "Sent message" << std::endl;
            client_sock->sendMessage(msg.toString());
            std::this_thread::sleep_for(1s);
        }
        client_sock->close();

    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
