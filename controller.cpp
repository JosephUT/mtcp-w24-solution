#include <iostream>
#include <mros_json.hpp>
#include <socket/message_socket/client_message_socket.hpp>
#include <memory>


int main() {
    int const kDomain = AF_INET;
    std::string const kServerAddress = "127.0.0.1";
    int const kServerPort = 13348;

    auto client_sock = std::make_shared<ClientMessageSocket>(kDomain, kServerAddress, kServerPort);
    client_sock->connect();

    return 0;
}
