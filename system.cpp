#include <socket/server_socket.hpp>
#include <socket/message_socket/connection_message_socket.hpp>
#include <mros_json.hpp>
#include <csignal>
#include <cmath>
#include <iostream>

bool status = true;
static void signalHandler(int signal) {
    status = false;
}


/**
 * Converts contiguous vector to matrix. Matrix is always square
 * @param vector matrix in contiguous form
 * @return matrix
 */
std::vector<std::vector<double>> vector2matrix(std::vector<double> const& vector) {
    std::size_t const squareDim = static_cast<std::size_t>(std::sqrt(vector.size()));
    std::vector<std::vector<double>> matrix(squareDim, std::vector<double>(squareDim, 0));
    for (size_t row = 0; row < squareDim; ++row) {
        for (size_t col = 0; col < squareDim; ++col) {
            matrix[row][col] = vector[squareDim * row + col];
        }
    }
    return matrix;
}



int main() {
    int const kDomain = AF_INET;
    std::string const kServerAddress = "127.0.0.1";
    int const kServerPort = 13349;
    int const kBacklogSize = 20;

    std::signal(SIGINT, signalHandler);

    auto server_socket = std::make_shared<ServerSocket>(kDomain, kServerAddress, kServerPort, kBacklogSize);
    std::shared_ptr<ConnectionMessageSocket> controllerConnectionMessageSocket;
    std::shared_ptr<ConnectionMessageSocket> monitorConnectionMessageSocket;

    while (status) {
        if (!controllerConnectionMessageSocket) {
            controllerConnectionMessageSocket = server_socket->acceptConnection<ConnectionMessageSocket>();
        }
        if (!monitorConnectionMessageSocket) {
            monitorConnectionMessageSocket = server_socket->acceptConnection<ConnectionMessageSocket>();
        }
        while (status) {
            std::string recvMessage = controllerConnectionMessageSocket->receiveMessage();
            Json recvJson = Json::fromString(recvMessage);
            // Do something with the Json
            std::vector<double> contiguousMatrix = recvJson["twist"].get<std::vector<double>>();
            std::vector<std::vector<double>> matrix = vector2matrix(contiguousMatrix);
            for (auto const& vec : matrix) {
                for (double const val : vec) {
                    std::cout << val << ' ';
                }
                std::cout << std::endl;
            }

            // End

        }
    }
    controllerConnectionMessageSocket->close();
    monitorConnectionMessageSocket->close();
    server_socket->close();


    return 0;
}