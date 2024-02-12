# Project.
cmake_minimum_required(VERSION 3.25)
project(mtcp)
set(CMAKE_CXX_STANDARD 20)

# Dependencies.
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Libraries.
add_library(socket_lib SHARED
        src/socket/socket.cpp
        src/socket/server_socket.cpp
        src/socket/client_socket.cpp
        src/socket/connection_socket.cpp
        src/socket/message_socket/message_socket.cpp
        src/socket/message_socket/client_message_socket.cpp
        src/socket/message_socket/connection_message_socket.cpp
)
target_include_directories(socket_lib PUBLIC include)

add_library(json_lib SHARED src/mros_json.cpp)
target_include_directories(json_lib PUBLIC include)

# Executables.
add_executable(controller src/controller.cpp)
target_link_libraries(controller PRIVATE socket_lib json_lib)

add_executable(system src/system.cpp)
target_link_libraries(system PRIVATE socket_lib json_lib)

# Automated tests.
enable_testing()
add_executable(test_mros_json test/test_mros_json.cpp)
target_link_libraries(test_mros_json GTest::gtest_main json_lib)
gtest_discover_tests(test_mros_json)

add_executable(test_pose test/test_pose.cpp)
target_link_libraries(test_pose PRIVATE GTest::gtest_main json_lib)
gtest_discover_tests(test_pose)

add_executable(test_twist test/test_twist.cpp)
target_link_libraries(test_twist PRIVATE GTest::gtest_main json_lib)
gtest_discover_tests(test_twist)

add_executable(test_message_socket test/test_message_socket.cpp)
target_link_libraries(test_message_socket PRIVATE GTest::gtest_main socket_lib)
gtest_discover_tests(test_message_socket)

# Manual tests.
add_executable(test_bridge_and_simulator test_manual/test_bridge_and_simulator.cpp)
target_link_libraries(test_bridge_and_simulator PRIVATE socket_lib json_lib)

add_executable(test_json_parsing test_manual/test_json_parsing.cpp)
target_link_libraries(test_json_parsing PRIVATE json_lib)

set(SOCKET_HEADERS
    include/socket/client_socket.hpp
    include/socket/connection_socket.hpp
    include/socket/server_socket.hpp
    include/socket/socket.hpp
)

set(MSG_SOCKET_HEADERS
    include/socket/message_socket/message_socket.hpp
    include/socket/message_socket/client_message_socket.hpp
    include/socket/message_socket/connection_message_socket.hpp
)

set(SOCKET_UTLS_HEADERS
   include/socket/utils/peer_closed_exception.hpp
   include/socket/utils/socket_errno_exception.hpp
   include/socket/utils/socket_exception.hpp
)

set(JSON_HEADERS
    include/mros_json.hpp
)

# Install the headers to different locations.
install(FILES ${SOCKET_HEADERS} DESTINATION include/socket)
install(FILES ${MSG_SOCKET_HEADERS} DESTINATION include/socket/message_socket)
install(FILES ${SOCKET_UTLS_HEADERS} DESTINATION include/socket/utils)
install(FILES ${JSON_HEADERS} DESTINATION include)

install(TARGETS socket_lib json_lib
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

# CPack configuration
set(CPACK_PACKAGE_NAME "MTCP-lib")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "Broderick Riopelle broderio@umich.edu")
set(CPACK_PACKAGE_DESCRIPTION "A package containing the socket_lib and json_lib libraries.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A package containing the socket_lib and json_lib libraries.")

# Choose the type of package you want to generate. For example, if you want to generate a .deb package, use "DEB"
set(CPACK_GENERATOR "TGZ")

# This must always be last
include(CPack)