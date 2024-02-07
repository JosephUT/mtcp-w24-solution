#include <chrono>

#include <lcm/lcm-cpp.hpp>
#include <mbot_lcm_msgs/twist2D_t.hpp>

#include "socket/server_socket.hpp"
#include "socket/message_socket/client_message_socket.hpp"
#include "messages/twist.hpp"
#include "mros_json.hpp"

static bool status = true;
void ctrlc(int)
{
    status = false;
}

int main() {
    // Setup LCM.
    lcm::LCM lcmInstance("udpm://239.255.76.67:7667?ttl=0");
    if(!lcmInstance.good()) return 1;
    mbot_lcm_msgs::twist2D_t cmd_vel{0, 0, 0, 0};
    int64_t utime;
    cmd_vel.utime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    lcmInstance.publish("MBOT_VEL_CMD", &cmd_vel);

    // Set up Message Sockets.
    std::unique_ptr<ServerSocket> server = std::make_unique<ServerSocket>(AF_INET, "127.0.0.1", 13340, 1);
    std::shared_ptr<ConnectionMessageSocket> connection;
    do {
        connection = server->acceptConnection<ConnectionMessageSocket>();
    } while (!connection);
    
    // Receive from Message Socket and send over LCM.
    while (status) {
        Json twist_json = Json::fromString(connection->receiveMessage());
        Messages::Twist2d twist_mros = twist_json;
        
        cmd_vel.vx = twist_mros.dx;
        cmd_vel.vy = twist_mros.dy;
        cmd_vel.wz = twist_mros.dtheta;
        utime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        cmd_vel.utime = utime;
        lcmInstance.publish("MBOT_VEL_CMD", &cmd_vel);
    }
    cmd_vel.vx = 0;
    cmd_vel.vy = 0;
    cmd_vel.wz = 0;
    utime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    cmd_vel.utime = utime;
    lcmInstance.publish("MBOT_VEL_CMD", &cmd_vel);
    return 0;
}
