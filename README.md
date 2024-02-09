# Project 1

# Overview
Project 1 is an introduction to messaging which will provide the foundations for writing a complete publisher-subscriber package
in Project 2. There are five main components to this project. You will implement a JSON parser to allow for the serialization of
complex data types into string format. You will implement two message types that utilize this JSON parser to provide methods for
serializing the data they hold. Additionally, you will implement a small hierarchy of socket objects the wrap the Linux socket 
interface with a simple protocol that sends framed string messages over TCP/IP. Combining all of these parts, you will implement 
two processes in the back end of a highly simplified simulator that communicate via JSON formatted messages sent as framed strings.

## Project Components
This section provides a summary of each part of the project you must implement, and provides some advice that will be useful. Of 
course, since the test cases are provided to you, you may refer to them in determining the exect requirement you will be evaluated 
on.

### 1. Build System
The most fundamental part of this project is getting it to build. You will do this by implementing the `CMakeLists.txt` in the 
root directory. Any implementation of this file that allows all of the test cases to be built and run, builds `controller.cpp` and
`system.cpp` as two separate executables, and uses the provided file structure will be marked for full points. However, some 
guideance is provided below.

- Set your C++ standard to 20. Some of the header files include modern features.
- Find two external packages: GTest and Threads. Don't worry about Threads for now, it is simply used for testing.
- Build everything under `src/socket` as a single library and link against it in any other target that uses the sockets.
- Build `src/mros_json.cpp` as a single library and link against it in any other target that uses the JSON parser.
- Build `controller.cpp` and `system.cpp` as executables. **You will need to do this to run your code for part 5**.
- Build everything under `/test` as an automated test. **Make sure all four test suites are runnable via CTest**.
- Build `test_manual/test_bridge_and_simulator` as an executable. This is entirely optional and ungraded.

### 2. JSON Parser
The JSON parser should be implemented in `src/mros_json.cpp` according to the definitions in `include/mros_json.hpp` and the test 
cases in `test/test_mros_json.cpp`. Public member functions should not be changed as this will likely cause build issues with the
test cases. Private member functions and variables are provided as a suggestion and mirror our reference implementation. These 
can be changed if you feel it would ease your implementation. In addition to supporting JSON parsing, this JSON object also allows 
for natural conversion functions between certain structs and `Json` instances. Take some time to read about `std::variant`, C++ 20 
concepts, and any other unfamiliar features used in the header file to understand the design of the parser and inform your 
implementation.

### 3. Message Definitions
Two message definitions are required for this project. A struct called `Pose2d` with `double` type members `x`, `y`, and `theta` 
should be implemented in `include/messages/pose.hpp`. A struct called `Twist2d` with `double` type members `dx`, `dy`, and `dtheta` 
should be implemented in `include/messages/twist.hpp`. Both message structs should be implemented within a C++ namespace called `Messages`,
and should implement `toJson` and `fromJson` functions such that they can be used with the conversion features of the JSON parser. 
These functions will also need to use the JSON parser in their implementation. Notice how this arrangement is extensible to any 
message of supported data types that you could define.

### 4. Message Sockets
The `src/socket` and `include/socket` directories implement an inheritance hierarchy of socket classes to model the functionality 
provided by the Linux socket libraries and abstract this functionality into a simpler and less feature-rich interface. This hierarchy
will be expaned upon in future assignments. For this project you will implement the four base classes `Socket`, `ServerSocket`, 
`ClientSocket`, and `ConnectionSocket`, which provide a basic outline of how the socket classes will interact. You will then implement 
the first usable set of subclasses `MessageSocket`, `ClientMessageSocket`, and `ConnectionMessageSocket`, which provide an abstraction 
for sending and receiving framed strings of any length. View the header files under `include/socket` and the tests in `test/test_message_socket.cpp` and complete your implementations in the files under `src/socket`. Strings should be framed with `\n` newline characters for the message sockets. 
You may add private members as you see fit for implementing `MessageSocket`. Modifying private members of other socket classes is likely to cause more harm than help.

### 5. Controller and System
The controller and system should be implemented in `src/controller.cpp` and `src/system.cpp`, respectively. These should be built as separate 
executables. The controller should have a `ClientMessageSocket` that connects to a `ServerSocket` on the system. The system should also have a 
`ClientMessageSocket` that connects to the Python bridge on port 8002 (see P1 Simulator below). The controller will send control commands to the system process, which will use these commands to update a robot pose over time. The system will also send this pose to the simulator for visualization. 
The controller should read data from `data/control_inputs.txt` and load each line into a `Twist2d` message in (dx, dy, dtheta) order. After loading each line, the controller should send the twist command to the system in serial JSON form. The controller should limit its sending rate to 10Hz. See 
std::this_thread::sleep_for() for implementation. The system should initialize a `Pose2d` at (0, 0, 0), receive each message, use it to update the pose 
with the helper provided in `include/system_utils.hpp`, and send the updated pose to the Python bridge. The processes should run smoothly when the simulator is started, then the system is started, then the controller is started. 

## File Reference
A list of the purpose of each file in the template, provided for clarity and convenience (in alphabetical order). Files that you will need to implement
in are marked **TODO**. You may also make modifications to private members in some of the header files not marked, as described above.

- `data/control_inputs.txt`: Twist commands to read into the controller.
- `include/messages/pose.hpp`: Definition of pose message. **TODO**
- `include/messages/twist.hpp`: Definition of twist message. **TODO**
- `include/socket/message_socket/client_message_socket.hpp`: Definition of client message socket.
- `include/socket/message_socket/connection_message_socket.hpp`: Definition of connection message socket.
- `include/socket/message_socket/message_socket.hpp`: Definition of message socket.
- `include/socket/utils/peer_closed_exception.hpp`: Definition of exception thown when a peer socket closes.
- `include/socket/utils/socket_errno_exception.hpp`: Definition of exception collecting error messages from errno.
- `include/socket/utils/socket_exception.hpp`: Definition of exception thrown for general socket errors.
- `include/socket/client_socket.hpp`: Definition of abstract client socket class.
- `include/socket/connection_socket.hpp`: Definition of abstract connection socket class.
- `include/socket/server_socket.hpp`: Definition of server socket class.
- `include/socket/socket.hpp`: Definition of abstract socket class.
- `include/mros_json.hpp`: Definition of JSON object with parsing and conversion functionality.
- `include/system_utils.hpp`: Definition of utility function for updating a pose with a twist.
- `simulator/bridge.py`: Bridge program to connect the system to the simulator webpage.
- `simulator/index.html`: HTML file for the simulator webpage.
- `simulator/main.js`: JavaScript file for the simulator webpage.
- `src/socket/message_socket/client_message_socket.cpp`: Source of client message socket. **TODO**
- `src/socket/message_socket/connection_message_socket.cpp`: Source of connection message socket. **TODO**
- `src/socket/message_socket/message_socket.cpp`: Source of message socket. **TODO**
- `src/socket/client_socket.cpp`: Source of abstract client socket class. **TODO**
- `src/socket/connection_socket.cpp`: Source of abstract connection socket class. **TODO**
- `src/socket/server_socket.cpp`: Source of server socket class. **TODO**
- `src/socket/socket.cpp`: Source of abstract socket class. **TODO**
- `src/controller.cpp`: Source of controller executable. **TODO**
- `src/mros_json.cpp`: Source of JSON object with parsing and conversion functionality. **TODO**
- `src/system.cpp`: Source of system executable. **TODO**
- `test/test_message_socket.cpp`: Tests for all the concrete socket classes.
- `test/test_mros_json.cpp`: Tests for the JSON object.
- `test/test_pose.cpp`: Tests for the pose message.
- `test/test_twist.cpp`: Tests for the twist message.
- `test_manual/test_bridge_and_simulator.cpp`: Source for the optional executable for feeding data to the simulator.
- `CMakeLists.txt`: Build system generation file for building the files above. **TODO**

## P1 Simulator  
The P1 Simulator is an extremely basic visualizer that you can use to see the updates made to the system's pose.
It is composed of an HTML and JavaScript webpage that connects to a Python bridge script. The bridge and the webpage
connect via WebSockets, which implement a communication protocol on top of TCP/IP just like MessageSockets - Only much 
more complicated. Your C++ code can interact with the simulator by connecting to the Python bridge on port 8002 with a 
`ClientMessageSocket`, and sending JSON formatted `Pose2d` messages. We call the Python script a bridge because its only
function is to communicate between two other programs in different ways.

1. Go into `/simulator` and run `python3 -m http.server`. This serves the simulator to your browser on port 8000.
2. Open a new terminal and run `python3 bridge.py` to start the bridge.
3. Type `localhost:8000` into your browser search bar to load the web page. You should see output from the bridge that it has connected.
4. Run any C++ program with a `ClientMessageSocket` that connects to the bridge on port 8002.

The webpage attempts to connect to the bridge when it is loaded or refreshed. (It has a *client* websocket.) The bridge 
should be in its initial state with the output "refresh the P1 simulator in your browser" when the webpage attempts to connect. 
No C++ programs should attempt to connect to the bridge before the bridge and webpage are connected.

If you want to verify that the bridge and webpage are working, you can run an executable built from `test_manual/test_bridge_and_simulator`. 
If your `ClientMessageSocket`, `Pose2d` message, and `Json` object are all implemented correctly, you should see the cylinder bot 
spin on the webpage.

# Testing
The build system will not be tested directly. You will need to have built the project at least enough to run test cases for some of 
the components to receive any points on the project.

The JSON parser, message definitions, and message sockets can be tested by invoking the unit tests under ```/test```. After 
properly implementing the CMakeLists.txt, you should be able to invoke your unit tests by running ```ctest``` in the root of 
the build tree. Refer to the testing usage discussion in Lab 1. All tests should pass after completion.

The controller and system can be tested manually by running the simulator, running the system, and then running the controller. The result
should be that the white cylinder robot begins to move around the simulator page in a relatively smooth motion.

# Grading and Submission
Points for this project will be assigned via test case results and behaviors as follows.

- 1 - All tests run, the original file structure is maintained, and the controller and system are built as separate executables.
- 0.25 - MrosJson.TestBracketOperator passes.
- 0.25 - MrosJson.TestEqualityOperator passes.
- 0.25 - MrosJson.TestToStringInt passes.
- 0.5 - MrosJson.TestToStringDouble passes.
- 0.5 - MrosJson.TestToStringString passes.
- 0.5 - MrosJson.TestToStringArray passes.
- 0.25 - MrosJson.TestFromStringInt passes.
- 0.5 - MrosJson.TestFromStringDouble passes.
- 0.5 - MrosJson.TestFromStringString passes.
- 0.5 - MrosJson.TestFromStringArray passes.
- 0.25 - Pose2d.TestToJson passes.
- 0.25 - Pose2d.TestFromJson passes.
- 0.25 - Twist2d.TestToJson passes.
- 0.25 - Twist2d.TestFromJson passes.
- 1.25 - MessageSocketTest.SendReceive passes.
- 0.5 - MessageSocketTest.MultipleSendSingleReceive passes.
- 0.5 - MessageSocketTest.StressSendReceive passes.
- 0.25 - MessageSocketTest.CloseReopen passes.
- 0.25 - MessageSocketTest.ThrowClosed passes.
- 0.25 - MessageSocketTest.ThrowMessageContainsDelimiter passes.
- 0.25 - The controller rate is limited to 10Hz.
- 0.75 - The controller and system produce the appropriate movement of the robot in the simulator when started in the order described.

To submit, **push your code** before the deadline and **submit a link** to your repository in the assignment on Canvas.
