#include <socket/server_socket.hpp>
#include <socket/message_socket/connection_message_socket.hpp>
#include <iostream>
#include <mros_json.hpp>
#include <csignal>
#include <string>

bool status = true;


int main() {
    try {
        std::signal(SIGINT, [](int signum) -> void {
            status = false;
        });
        int const kDomain = AF_INET;
        std::string const kServerAddress = "127.0.0.1";
        int const kServerPort = 8080;
        int const kBacklogSize = 10;

        auto server_socket = std::make_unique<ServerSocket>(kDomain, kServerAddress, kServerPort, kBacklogSize);
        std::shared_ptr<ConnectionMessageSocket> client_message_socket;

        do {
            client_message_socket = server_socket->acceptConnection<ConnectionMessageSocket>();
        } while (!client_message_socket);
        while (status) {
            std::string msgStr = client_message_socket->receiveMessage();
            std::cout << "Message received: " << msgStr << std::endl;
        }
        client_message_socket->close();
        server_socket->close();
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}
