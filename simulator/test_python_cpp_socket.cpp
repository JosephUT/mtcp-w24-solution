#include <iostream>
#include <memory>

#include "socket/message_socket/client_message_socket.hpp"

int main() {
    auto client_socket = std::make_shared<ClientMessageSocket>(AF_INET, "127.0.0.1", 3000);
    client_socket->connect();

    std::string message;
    while (true) {
        std::cout << "enter message: ";
        std::getline(std::cin, message);
        client_socket->sendMessage(message);
        if (message == "close") break;
    }
    client_socket->close();
}
