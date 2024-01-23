#include <iostream>
#include <mros_json.hpp>
#include <socket/message_socket/client_message_socket.hpp>
#include <memory>
#include <fstream>
#include <messageTypes/twist.hpp>

int main() {
    try {
        int const kDomain = AF_INET;
        std::string const kServerAddress = "127.0.0.1";
        int const kServerPort = 13348;

        auto client_sock = std::make_shared<ClientMessageSocket>(kDomain, kServerAddress, kServerPort);
        client_sock->connect();

        double dx, dy, dtheta;
        std::ifstream ifs("sensorReadings.txt");
        while (ifs >> dx >> dy >> dtheta) {
            Messages::Twist2d twistMsg = {dx, dy, dtheta};
            Json twistJson;
            twistJson = twistMsg;
            client_sock->sendMessage(twistJson.toString());
        }
        client_sock->close();
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
