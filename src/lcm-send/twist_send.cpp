#include <memory>

#include "socket/message_socket/client_message_socket.hpp"
#include "messages/twist.hpp"

static bool status = true;
void ctrlc(int)
{
    status = false;
}

int main() {
    auto client = std::make_shared<ClientMessageSocket>(AF_INET, "127.0.0.1", 13340);
    client->connect();
    while(status) {
        // Get twist from keyboard.

        // Send the client request from the keyboard socket.
        double dx = 0;
        double dy = 0;
        double dtheta = 2.5;
        Messages::Twist2d twistMsg = {dx, dy, dtheta};
        Json twistJson;
        twistJson = twistMsg;
        client->sendMessage(twistJson.toString());
    }
    return 0;
}
